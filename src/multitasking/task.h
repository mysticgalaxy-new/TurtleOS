#include <stdint.h>

typedef struct {
    int pid;
    char name[32];
    int active;
} task_info_t;

int task_get_info(task_info_t* out, int max);
int add_task(void (*func), int stack_size);
int disable_task(int id);
int enable_task(int id);
