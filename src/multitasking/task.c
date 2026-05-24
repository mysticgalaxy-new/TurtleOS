//to clashnewbme: sorry for the inconsistent line breaks

#include <stdint.h>
#include "task.h"
#include "../memory/main.h"
#include "../libc/main.h"
#define MAX_TASKS 16

typedef struct {
	int pid;
	int active;
	uint32_t esp;
	char name[32];
}task_t;

//uint8_t task_stacks[MAX_TASKS][STACK_SIZE];
const int TASK_SIZE = sizeof(task_t);

int current_task = -1;
int tasks_count = 0;

task_t tasks[MAX_TASKS];

int task_init(void (*func), int id, int stack_size) {
	if (tasks_count < MAX_TASKS) {
		uint32_t* stack = (uint32_t*)((uint8_t*)malloc(stack_size) + stack_size);
		if (!stack) {
			return -1;
		}
		*--stack = 0x202;        // EFLAGS
		*--stack = 0x08;         // CS
		*--stack = (uint32_t)func;   // EIP
		*--stack = 0; // EAX
		*--stack = 0; // ECX
		*--stack = 0; // EDX
		*--stack = 0; // EBX
		*--stack = 0; // ESP dummy
		*--stack = 0; // EBP
		*--stack = 0; // ESI
		*--stack = 0; // EDI
		tasks[id].esp = (uint32_t)stack;
		tasks[id].active = 1;
		tasks[id].pid = id;
		tasks_count++;
		return id;
	} else {
		return -1;
	}
}

void schedule() {
    int old = current_task;

    do {
        current_task = (current_task + 1) % tasks_count;
    } while (tasks[current_task].active == 0);
}

//Explaination of return codes for devs
//0 means everything's good
//-1 means something failed

int add_task(void (*func), int stack_size) {
	return task_init(func, tasks_count, stack_size);
}

int disable_task(int id) {
	if (id < tasks_count) {
		tasks[id].active = 0;
		return 0;
	} else {
		return -1;
	}
}

int enable_task(int id) {
	if (id < tasks_count) {
		tasks[id].active = 1;
		return 0;
	} else {
		return -1;
	}
}

int task_get_info(task_info_t* out, int max) {
    int j = 0;

    for (int i = 0; i < MAX_TASKS && j < max; i++) {
        if (tasks[i].active) {
            out[j].pid = tasks[i].pid;
            out[j].active = tasks[i].active;
            memcpy(out[j].name, tasks[i].name, 32);
            j++;
        }
    }

    return j;
}
