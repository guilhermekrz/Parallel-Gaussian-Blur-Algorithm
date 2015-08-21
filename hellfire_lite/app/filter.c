// Guilherme Krzisch e Matheus Cavalca
#include <prototypes.h>
#include "my_image.h"

uint8_t *finalImage; // Usado pelo mestre para montar a imagem final

/*  Usado pelo mestre pra sincronizar o recebimento das imagens processadas pelos escravos. Comeca com valor 0. 
 *  A cada rececimento de resposta com o processamento dos escravos, faz um post para aumentar o valor em 1.
 *  Na tarefa principal do mestre, faz N waits, onde N = numero de processamentos enviados ao escravo. Nesse caso N = 16.
 *  Assim a tarefa principal do mestre ira esperar o recebimento de todas as N partes antes de dar a resposta na saida padrao.
*/
semaphore n;


uint8_t gausian(uint8_t buffer[7][7]){
	int32_t sum=0, mpixel;
	uint8_t x,y;

	int8_t kernel[7][7] =	{{1, 1, 2, 2, 2, 1, 1},
				{1, 3, 4, 5, 4, 3, 1},
				{2, 4, 7, 8, 7, 4, 2},
				{2, 5, 8, 10, 8, 5, 2},
				{2, 4, 7, 8, 7, 4, 2},
				{1, 3, 4, 5, 4, 3, 1},
				{1, 1, 2, 2, 2, 1, 1}};
	for(y=0;y<7;y++)
		for(x=0;x<7;x++)
			sum += ((int16_t)buffer[y][x] * (int16_t)kernel[y][x]);
	mpixel = (int32_t)(sum/171);

	return (uint8_t)mpixel;
}

void do_gausian(uint8_t *imgFinal, uint8_t *imgReceived, int height, int width){
	int32_t x,y,u,v;
	uint8_t image_buffer[7][7];
	
	for(y=0;y<height;y++){
		if (y > 2 || y < height-2){
			for(x=0;x<width;x++){
				if (x > 2 || x < width-2){
					for(v=0;v<7;v++)
						for(u=0;u<7;u++)
							image_buffer[v][u] = imgReceived[(((y+v-3)*width)+(x+u-3))];

					imgFinal[((y*width)+x)] = gausian(image_buffer);
				}else{
					imgFinal[((y*width)+x)] = imgReceived[((y*width)+x)];
				}
			}
		}else{
			imgFinal[((y*width)+x)] = imgReceived[((y*width)+x)];
		}
	}

	// Ajustando a imagem
	for(x=0;x<32;x++){
		for(y=0;y<32;y++){
			if(x < 3 && y < 3){
				imgFinal[x + y *width] = imgFinal[3  + 3 * width];
			} else if(x < 3 && y >= (32 - 3)){
				imgFinal[x + y *width] = imgFinal[3 + (32 - 1 - 3) * width];
			} else if(y < 3 && x >= (32 - 3)){
				imgFinal[x + y *width] = imgFinal[(32 - 1 - 3) + 3 * width];
			} else if(y >= (32 - 3) && x >= (32 - 3)){
				imgFinal[x + y *width] = imgFinal[(32 - 1 - 3) + (32- 1 - 2) * width];
			} else if(x < 3){
				imgFinal[x + y *width] = imgFinal[3 + y * width];
			} else if(y < 3){
				imgFinal[x + y *width] = imgFinal[x + 3 * width];
			} else if(x >= (32 - 3)){
				imgFinal[x + y *width] = imgFinal[(32 - 1 -  3) + y * width];
			} else if(y >= (32 - 3)){
				imgFinal[x + y *width] = imgFinal[x + (32 - 1 - 3) * width];
			}
		}
	}
}

void escravoTask(void){
	uint32_t x,y,z;
	uint8_t *imgFinal, *imgReceived;
	uint32_t time;
	int width, height;
	width = 32;
	height = 32;
	
	while(1) {
		while(HF_PacketsQueued(HF_CurrentTaskId()) == 0);
		imgFinal = (uint8_t *) malloc(height * width);
		imgReceived = (uint8_t *) malloc(height * width);
		if (imgFinal == NULL || imgReceived == NULL){
			printf("\nmalloc() failed!\n");
			for(;;);
		}

		// Receive
		uint16_t source_cpu;
		uint8_t source_id;
		uint16_t size;
		HF_Receive(&source_cpu, &source_id, imgReceived, &size);
		printf("Received %d data size\n", size);

		// Process
		time = MemoryRead(COUNTER_REG);
		do_gausian(imgFinal, imgReceived, height, width);
		time = MemoryRead(COUNTER_REG) - time;
		printf("Processamento feito em %d clock cycles.\n\n", time);

		// Send
		HF_Send(source_cpu, source_id, imgFinal, 1024);
		free(imgReceived);
		free(imgFinal);
		printf("end of processing!\n");
	}	
}

// uma imagem de 128x128
void mestreTask(void){
	uint32_t time;
	while(1) {
		finalImage = (uint8_t *) malloc(height * width);
		if (finalImage == NULL){
			printf("malloc() failed!\n");
			for(;;);
		}
		printf("Start mestre processing. Esperando escravos responderem...\n");
		time = MemoryRead(COUNTER_REG);

		// Wait
		int i;
		for(i=0;i<16;i++){
			HF_SemWait(&n);
		}

		// Time
		time = MemoryRead(COUNTER_REG) - time;
		printf("Processamento feito em %d clock cycles.\n\n", time);

		// Print
		printf("\n\nint32_t width = %d, height = %d;\n", width, height);
		printf("uint8_t image[] = {\n");
		int x, y, z;
		for(y=0;y<height;y++){
			for(x=0;x<width;x++){
				printf("0x%x", finalImage[y*width+x]);
				if ((y < height-1) || (x < width-1)) printf(", ");
				if ((++z % 16) == 0) printf("\n");
			}
		}
		printf("};\n");

		// End
		free(finalImage);
		for(;;);
	}
}

void sendReceiveEscravoTask(int i, char *escravoName){
	uint8_t *img;
	int width, height;
	width = 32;
	height = 32;
	while(1) {
		int idTaskI;
		int posX, posY;
		uint8_t buf[1024]; //32*32 = 1024
		// Enviar tarefas para os escravos
		// Enviando primeira parte para o escravo i
		idTaskI = HF_QueryTaskId(HF_Core(i), escravoName);

		int newPosXAdded, newPosYAdded;
		newPosXAdded = ((32 * (i - 1)) % 128);
		newPosYAdded = ((32 * (i - 1)) / 128) * 32;
		printf("[1] %d = %d / %d\n", i, newPosXAdded, newPosYAdded);
		for(posX = 0; posX < 32; posX++){
			for(posY = 0;posY<32;posY++){
				int newPosX, newPosY;
				newPosX = posX + newPosXAdded;
				newPosY = posY + newPosYAdded;
				buf[posX + posY*width] = image[newPosX + newPosY*128];
			}
		}
		HF_Send(HF_Core(i), idTaskI, buf, 1024);
		// Enviando segunda parte para o escravo i
		newPosXAdded = ((32 * (i - 1)) % 128);
		newPosYAdded = ((32 * (i - 1)) / 128) * 32 + 64;
		printf("[2] %d = %d / %d\n", i, newPosXAdded, newPosYAdded);
		for(posX = 0; posX < 32; posX++){
			for(posY = 0;posY<32;posY++){
				int newPosX, newPosY;
				newPosX = posX + newPosXAdded;
				newPosY = posY + newPosYAdded;
				buf[posX + posY*width] = image[newPosX + newPosY*128];
			}
		}
		HF_Send(HF_Core(i), idTaskI, buf, 1024);

		// Receber
		int count;
		count = 0;
		while(1){
			while(HF_PacketsQueued(HF_CurrentTaskId()) == 0);
			img = (uint8_t *) malloc(height * width);
			if (img == NULL){
				printf("malloc() failed!\n");
				for(;;);
			}
			uint16_t source_cpu;
			uint8_t source_id;
			uint16_t size;
			HF_Receive(&source_cpu, &source_id, img, &size);

			newPosXAdded = ((32 * (i - 1)) % 128);
			newPosYAdded = ((32 * (i - 1)) / 128) * 32;
			if(count != 0){
				newPosYAdded += 64;
			}

			for(posX = 0; posX < 32; posX++){
				for(posY = 0;posY<32;posY++){
					int newPosX, newPosY;
					newPosX = posX + newPosXAdded;
					newPosY = posY + newPosYAdded;	
					finalImage[newPosX + newPosY * 128] = img[posX + posY*width];
				}
			}
			printf("Received %d data size from escravo %d - %d\n", size, source_cpu, source_id);
			free(img);
			HF_SemPost(&n);
			count++;
		}
	}
}

void sr1Task(){
	sendReceiveEscravoTask(1, "escravo1");
}

void sr2Task(){
	sendReceiveEscravoTask(2, "escravo2");
}

void sr3Task(){
	sendReceiveEscravoTask(3, "escravo3");
}

void sr4Task(){
	sendReceiveEscravoTask(4, "escravo4");
}

void sr5Task(){
	sendReceiveEscravoTask(5, "escravo5");
}

void sr6Task(){
	sendReceiveEscravoTask(6, "escravo6");
}

void sr7Task(){
	sendReceiveEscravoTask(7, "escravo7");
}

void sr8Task(){
	sendReceiveEscravoTask(8, "escravo8");
}

// 0 = mestre
// 1 ao 8 = escravos
void ApplicationMain(void) {
	HF_SemInit(&n,0); 

#if CPU_ID == 0
	HF_AddTask(mestreTask, "mestre", 2048, 10, TASK_DEFAULT);
	HF_AddTask(sr1Task, "sr1Task", 2048, 10, TASK_DEFAULT);
	HF_AddTask(sr2Task, "sr2Task", 2048, 10, TASK_DEFAULT);
	HF_AddTask(sr3Task, "sr3Task", 2048, 10, TASK_DEFAULT);
	HF_AddTask(sr4Task, "sr4Task", 2048, 10, TASK_DEFAULT);
	HF_AddTask(sr5Task, "sr5Task", 2048, 10, TASK_DEFAULT);
	HF_AddTask(sr6Task, "sr6Task", 2048, 10, TASK_DEFAULT);
	HF_AddTask(sr7Task, "sr7Task", 2048, 10, TASK_DEFAULT);
	HF_AddTask(sr8Task, "sr8Task", 2048, 10, TASK_DEFAULT);
#elif CPU_ID == 1
	HF_AddTask(escravoTask, "escravo1", 2048, 10, TASK_DEFAULT);
#elif CPU_ID == 2
	HF_AddTask(escravoTask, "escravo2", 2048, 10, TASK_DEFAULT);
#elif CPU_ID == 3
	HF_AddTask(escravoTask, "escravo3", 2048, 10, TASK_DEFAULT);
#elif CPU_ID == 4
	HF_AddTask(escravoTask, "escravo4", 2048, 10, TASK_DEFAULT);
#elif CPU_ID == 5
	HF_AddTask(escravoTask, "escravo5", 2048, 10, TASK_DEFAULT);
#elif CPU_ID == 6
	HF_AddTask(escravoTask, "escravo6", 2048, 10, TASK_DEFAULT);
#elif CPU_ID == 7
	HF_AddTask(escravoTask, "escravo7", 2048, 10, TASK_DEFAULT);
#else 
	HF_AddTask(escravoTask, "escravo8", 2048, 10, TASK_DEFAULT);
#endif

	HF_Start();
}

