#include <zephyr/kernel.h>

#define STACK_SIZE 1024U

#define PRIO_THREAD_A	3U
#define PRIO_THREAD_B	44U

extern void Thread_B(void *, void *, void *);
extern void Thread_A(void *, void *, void *);

K_THREAD_DEFINE(thread_a, STACK_SIZE,
				Thread_A, NULL, NULL, NULL,
				PRIO_THREAD_A, 0, K_TICKS_FOREVER);

K_THREAD_DEFINE(thread_b, STACK_SIZE,
				Thread_B, NULL, NULL, NULL,
				PRIO_THREAD_B, 0, K_TICKS_FOREVER);


int main(void)
{
	
	k_thread_start(thread_a);
	k_thread_start(thread_b);
    return 0;
}