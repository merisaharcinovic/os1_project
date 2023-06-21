//
// Created by os on 8/23/22.
//

#ifndef SYSCALL_C_HPP
#define SYSCALL_C_HPP

#include "../h/sem.hpp"

typedef TCB* thread_t;
typedef sem* sem_t;

void thread_dispatch();
int thread_exit();
int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_create_no_start(thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_start(thread_t thread);


int sem_open (sem_t* handle,unsigned init);
int sem_close (sem_t handle);
int sem_wait (sem_t id);
int sem_signal (sem_t id);

void putc(char c);
char getc();

//void* mem_alloc (size_t size);
//int mem_free (void* addr);

#endif //SYSCALL_C_HPP

