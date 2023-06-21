//
// Created by os on 9/9/22.
//

#ifndef SYSCALL_CPP_HPP
#define SYSCALL_CPP_HPP
//#include "syscall_c.hpp"


#include "tcb.hpp"
#include "sem.hpp"

typedef TCB* thread_t;
typedef sem* sem_t;
void* operator new (size_t);
void operator delete (void*);

class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    static void dispatch ();
    static void thread_wrapper(void* thread);
    //static int sleep (time_t);
protected:
    Thread ();
    virtual void run () {}
private:
    thread_t myHandle;
};

class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
private:
    sem_t myHandle;
};

class Console {
public:
    static char getc ();
    static void putc (char c);
};

#endif //SYSCALL_CPP_HPP
