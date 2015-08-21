#include <prototypes.h>

void task_handler1(void){
	uint16_t source_cpu;
	uint8_t source_id;
	uint16_t size;

	uint8_t buf[4096];
	int i;
	for(i=0;i<4096;i++){
		buf[i] = 'a';
	}
	uint32_t time;

	while(1){
		printf("\nthread %d -> sendingggggggggggggggggggggggggggggggggggggggggggggg", HF_CurrentTaskId());
		int idTask2 = HF_QueryTaskId(HF_Core(1), "task 2");
		int idTask3 = HF_QueryTaskId(HF_Core(1), "task 3");
		int idTask4 = HF_QueryTaskId(HF_Core(2), "task 4");
		int idTask5 = HF_QueryTaskId(HF_Core(2), "task 5");
		HF_Send(HF_Core(1), idTask2, buf, 256);
		HF_Send(HF_Core(1), idTask3, buf, 64);
		HF_Send(HF_Core(2), idTask4, buf, 64);
		HF_Send(HF_Core(2), idTask5, buf, 64);
		for(;;);
	}
}

void task_handler2(void){
	uint16_t source_cpu;
	uint8_t source_id;
	uint16_t size;

	uint8_t buf[4096];
	uint32_t time;

	while(1){
		while(HF_PacketsQueued(HF_CurrentTaskId()) == 0);
		time = MemoryRead(COUNTER_REG);
		HF_Receive(&source_cpu, &source_id, buf, &size);
		time = MemoryRead(COUNTER_REG) - time;
		printf("\nthread %d -> message from node %d thread %d size %d, time %d cycles -> %s", HF_CurrentTaskId(), source_cpu, source_id, size, time, buf);

		int idTask6 = HF_QueryTaskId(HF_Core(3), "task 6");
		int idTask7 = HF_QueryTaskId(HF_Core(3), "task 7");
		int idTask8 = HF_QueryTaskId(HF_Core(4), "task 8");
		HF_Send(HF_Core(3), idTask6, buf, 64);
		HF_Send(HF_Core(3), idTask7, buf, 320);
		HF_Send(HF_Core(4), idTask8, buf, 320);
		for(;;);
	}
}

void task_handler3(void){
	uint16_t source_cpu;
	uint8_t source_id;
	uint16_t size;

	uint8_t buf[4096];
	uint32_t time;

	while(1){
		while(HF_PacketsQueued(HF_CurrentTaskId()) == 0);
		time = MemoryRead(COUNTER_REG);
		HF_Receive(&source_cpu, &source_id, buf, &size);
		time = MemoryRead(COUNTER_REG) - time;
		printf("\nthread %d -> message from node %d thread %d size %d, time %d cycles -> %s", HF_CurrentTaskId(), source_cpu, source_id, size, time, buf);

		int idTask7 = HF_QueryTaskId(HF_Core(3), "task 7");
		int idTask8 = HF_QueryTaskId(HF_Core(4), "task 8");
		HF_Send(HF_Core(3), idTask7, buf, 320);
		HF_Send(HF_Core(4), idTask8, buf, 64);
		for(;;);
	}
}

void task_handler4(void){
	uint16_t source_cpu;
	uint8_t source_id;
	uint16_t size;

	uint8_t buf[4096];
	uint32_t time;

	while(1){
		while(HF_PacketsQueued(HF_CurrentTaskId()) == 0);
		time = MemoryRead(COUNTER_REG);
		HF_Receive(&source_cpu, &source_id, buf, &size);
		time = MemoryRead(COUNTER_REG) - time;
		printf("\nthread %d -> message from node %d thread %d size %d, time %d cycles -> %s", HF_CurrentTaskId(), source_cpu, source_id, size, time, buf);

		int idTask8 = HF_QueryTaskId(HF_Core(4), "task 8");
		HF_Send(HF_Core(4), idTask8, buf, 64);
		for(;;);
	}
}

void task_handler5(void){
	uint16_t source_cpu;
	uint8_t source_id;
	uint16_t size;

	uint8_t buf[4096];
	uint32_t time;

	while(1){
		while(HF_PacketsQueued(HF_CurrentTaskId()) == 0);
		time = MemoryRead(COUNTER_REG);
		HF_Receive(&source_cpu, &source_id, buf, &size);
		time = MemoryRead(COUNTER_REG) - time;
		printf("\nthread %d -> message from node %d thread %d size %d, time %d cycles -> %s", HF_CurrentTaskId(), source_cpu, source_id, size, time, buf);

		int idTask8 = HF_QueryTaskId(HF_Core(4), "task 8");
		HF_Send(HF_Core(4), idTask8, buf, 640);
		for(;;);
	}
}

void task_handler6(void){
	uint16_t source_cpu;
	uint8_t source_id;
	uint16_t size;

	uint8_t buf[4096];
	uint32_t time;

	while(1){
		while(HF_PacketsQueued(HF_CurrentTaskId()) == 0);
		time = MemoryRead(COUNTER_REG);
		HF_Receive(&source_cpu, &source_id, buf, &size);
		time = MemoryRead(COUNTER_REG) - time;
		printf("\nthread %d -> message from node %d thread %d size %d, time %d cycles -> %s", HF_CurrentTaskId(), source_cpu, source_id, size, time, buf);

		int idTask9 = HF_QueryTaskId(HF_Core(5), "task 9");
		HF_Send(HF_Core(5), idTask9, buf, 640);
		for(;;);
	}
}

void task_handler7(void){
	uint16_t source_cpu;
	uint8_t source_id;
	uint16_t size;

	uint8_t buf[4096];
	uint32_t time;

	while(1){
		while(HF_PacketsQueued(HF_CurrentTaskId()) == 0);
		time = MemoryRead(COUNTER_REG);
		HF_Receive(&source_cpu, &source_id, buf, &size);
		time = MemoryRead(COUNTER_REG) - time;
		printf("\nthread %d -> message from node %d thread %d size %d, time %d cycles -> %s", HF_CurrentTaskId(), source_cpu, source_id, size, time, buf);

		int idTask9 = HF_QueryTaskId(HF_Core(5), "task 9");
		HF_Send(HF_Core(5), idTask9, buf, 640);
		for(;;);
	}
}

void task_handler8(void){
	uint16_t source_cpu;
	uint8_t source_id;
	uint16_t size;

	uint8_t buf[4096];
	uint32_t time;

	while(1){
		while(HF_PacketsQueued(HF_CurrentTaskId()) == 0);
		time = MemoryRead(COUNTER_REG);
		HF_Receive(&source_cpu, &source_id, buf, &size);
		time = MemoryRead(COUNTER_REG) - time;
		printf("\nthread %d -> message from node %d thread %d size %d, time %d cycles -> %s", HF_CurrentTaskId(), source_cpu, source_id, size, time, buf);

		int idTask9 = HF_QueryTaskId(HF_Core(5), "task 9");
		HF_Send(HF_Core(5), idTask9, buf, 640);
		for(;;);
	}
}

void task_handler9(void){
	uint16_t source_cpu;
	uint8_t source_id;
	uint16_t size;

	uint8_t buf[4096];
	uint32_t time;

	while(1){
		while(HF_PacketsQueued(HF_CurrentTaskId()) == 0);
		time = MemoryRead(COUNTER_REG);
		HF_Receive(&source_cpu, &source_id, buf, &size);
		time = MemoryRead(COUNTER_REG) - time;
		printf("\nthread %d -> message from node %d thread %d size %d, time %d cycles -> %s", HF_CurrentTaskId(), source_cpu, source_id, size, time, buf);
		for(;;);
	}
}

void ApplicationMain(void){
	int32_t i;

#if CPU_ID == 0
	HF_AddTask(task_handler1, "task 1", 6000, 10, TASK_DEFAULT);
	// sender 1 doesn't receive anything
#elif CPU_ID == 1
	HF_AddTask(task_handler2, "task 2", 6000, 10, TASK_DEFAULT);
	HF_AddTask(task_handler3, "task 3", 6000, 10, TASK_DEFAULT);
#elif CPU_ID == 2
	HF_AddTask(task_handler4, "task 4", 6000, 10, TASK_DEFAULT);
	HF_AddTask(task_handler5, "task 5", 6000, 10, TASK_DEFAULT);
#elif CPU_ID == 3
	HF_AddTask(task_handler6, "task 6", 6000, 10, TASK_DEFAULT);
	HF_AddTask(task_handler7, "task 7", 6000, 10, TASK_DEFAULT);
#elif CPU_ID == 4
	HF_AddTask(task_handler8, "task 8", 6000, 10, TASK_DEFAULT);
#else 
	HF_AddTask(task_handler9, "task 9", 6000, 10, TASK_DEFAULT);
#endif

	HF_Start();
}

