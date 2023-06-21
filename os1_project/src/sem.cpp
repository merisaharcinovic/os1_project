//
// Created by os on 9/11/22.
//

#include "../h/sem.hpp"
sem::sem(int val) {
    isClosed=false;
    value=val;
}

int sem::signal() {
    if(isClosed) return -1; //error: signal on closed
    if(value<0 && waitList.peekFirst()) {
        TCB* thread = waitList.removeFirst();
        thread->state=TCB::READY;
        Scheduler::put(thread);
    }
    value++;
    return 0;
}

int sem::wait() {
    if(isClosed) return -2; //error: wait on closed
    value--;
    if(value<0){
        this->waitList.addLast(TCB::running);
        TCB::running->state = TCB::BLOCKED;
        TCB::dispatch();
        if(isClosed) return -2;
        return 0;
    }
    return 0;

}

int sem::sem_close(sem* handle) {
    if(!handle) return -1;
    handle->isClosed = true;
    while(handle->waitList.peekFirst()) {
        TCB* thread = handle->waitList.removeFirst();
        thread->state = TCB::READY;
        Scheduler::put(thread);
    }
    return 0;
}
