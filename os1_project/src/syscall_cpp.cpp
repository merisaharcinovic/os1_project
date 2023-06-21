//
// Created by os on 9/9/22.
//

#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.hpp"

Thread::Thread(void (*body)(void *), void *arg){
    thread_create_no_start(&this->myHandle, body, arg);
}

void Thread::thread_wrapper(void *thread){
    if(thread) ((Thread*)thread)->run();
}

Thread::Thread(){

    thread_create_no_start(&this->myHandle, thread_wrapper, this);
}

void Thread::dispatch(){
    thread_dispatch();
}
int Thread::start(){
    return thread_start(myHandle);
}
Thread::~Thread(){
    delete myHandle;
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}
