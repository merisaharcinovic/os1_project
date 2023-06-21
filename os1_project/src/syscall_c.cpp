//
// Created by os on 8/23/22.
//

#include "../h/syscall_c.hpp"
#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/mem.h"
#include "../h/sem.hpp"

//void* mem_alloc (size_t size){
//    uint64 sz = size;
//    __asm__ volatile("mv a2, %0" :: "r"(sz));
//    __asm__ volatile("mv a0, %0" :: "r"(0x01));
//
//
//    __asm__ volatile ("ecall");
//
//    uint64 ptr;
//    __asm__ volatile ("mv %0, a0" : "=r"(ptr));
//    return (void*)ptr;
//}
//int mem_free (void* addr){
//    __asm__ volatile("mv a0, %0" :: "r"(0x02));
//    __asm__ volatile("mv a1, %0" :: "r"((uint64)addr));
//
//    __asm__ volatile ("ecall");
//
//    uint64 returnValue;
//    __asm__ volatile ("mv %0, a0" : "=r"(returnValue));
//    return (int)returnValue;
//}

int thread_exit(){
    __asm__ volatile("mv a0, %0" :: "r"(0x12));
    __asm__ volatile("ecall");
    int returnValue;
    __asm__ volatile("mv %0, a0" : "=r" (returnValue));
    return returnValue;
}

void thread_dispatch(){
    __asm__ volatile("mv a0, %0" :: "r"(0x13));
    __asm__ volatile("ecall");
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg){
    //stack space
    void* stack;
    if(!start_routine) stack = nullptr;
    else {
        stack = __mem_alloc(DEFAULT_STACK_SIZE * sizeof (uint64));
        if(!stack) return -1;
    }
    __asm__ volatile("mv a4, %0" :: "r"((uint64)stack));
    __asm__ volatile("mv a3, %0" :: "r"((uint64)arg));
    __asm__ volatile("mv a2, %0" :: "r"((uint64)start_routine));
    __asm__ volatile("mv a1, %0" :: "r"((uint64)handle));
    __asm__ volatile("mv a0, %0" :: "r"(0x11));

    __asm__ volatile("ecall");
    int returnValue;
    __asm__ volatile("mv %0, a0" : "=r" (returnValue));
    return returnValue;
}
int thread_create_no_start(thread_t* handle, void(*start_routine)(void*), void* arg){
    void* stack;
    if(!start_routine) stack = nullptr;
    else {
        stack = __mem_alloc(DEFAULT_STACK_SIZE * sizeof (uint64));
        if(!stack) return -1;
    }
    __asm__ volatile("mv a4, %0" :: "r"((uint64)stack));
    __asm__ volatile("mv a3, %0" :: "r"((uint64)arg));
    __asm__ volatile("mv a2, %0" :: "r"((uint64)start_routine));
    __asm__ volatile("mv a1, %0" :: "r"((uint64)handle));
    __asm__ volatile("mv a0, %0" :: "r"(0x14));

    __asm__ volatile("ecall");
    int returnValue;
    __asm__ volatile("mv %0, a0" : "=r" (returnValue));
    return returnValue;
}
int thread_start(thread_t thread){
    __asm__ volatile("mv a1, %0" :: "r"(thread));
    __asm__ volatile("mv a0, %0" :: "r"(0x15));
    __asm__ volatile("ecall");
    int returnValue;
    __asm__ volatile("mv %0, a0" : "=r" (returnValue));
    return returnValue;
}
int sem_open (sem_t* handle,unsigned init){
    __asm__ volatile("mv a2, %0" :: "r"((uint64)handle));
    __asm__ volatile("mv a1, %0" :: "r"((uint64)init));
    __asm__ volatile("mv a0, %0" :: "r"(0x21));
    __asm__ volatile ("ecall");

    int returnValue;
    __asm__ volatile ("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}
int sem_close (sem_t handle){
    __asm__ volatile("mv a1, %0" :: "r"((uint64)handle));
    __asm__ volatile("mv a0, %0" :: "r"(0x22));
    __asm__ volatile ("ecall");

    int returnValue;
    __asm__ volatile ("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}
int sem_wait (sem_t id){
    __asm__ volatile("mv a1, %0" :: "r"((uint64)id));
    __asm__ volatile("mv a0, %0" :: "r"(0x23));
    __asm__ volatile ("ecall");

    int returnValue;
    __asm__ volatile ("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}
int sem_signal (sem_t id){
    __asm__ volatile("mv a1, %0" :: "r"((uint64)id));
    __asm__ volatile("mv a0, %0" :: "r"(0x24));
    __asm__ volatile ("ecall");

    int returnValue;
    __asm__ volatile ("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}


char getc(){
    __asm__ volatile("mv a0, %0" :: "r"(0x41));
    __asm__ volatile ("ecall");

    uint64 returnValue;
    __asm__ volatile ("mv %0, a0" : "=r"(returnValue));
    return (char)returnValue;
}
void putc(char c){
    __asm__ volatile("mv a1, %0" :: "r"((uint64)c));
    __asm__ volatile("mv a0, %0" :: "r"(0x42));
    __asm__ volatile ("ecall");
}

