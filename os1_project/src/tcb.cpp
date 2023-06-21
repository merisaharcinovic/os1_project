

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"


TCB *TCB::running = nullptr;
int TCB::ID = 0;


uint64 TCB::timeSliceCounter = 0;

TCB *TCB::createThreadNoStart(Body body, void* args, uint64* stackSpace)
{
    return new TCB(body, args, TIME_SLICE);
}

//void TCB::yield()
//{
//    __asm__ volatile ("ecall");
//}

void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished() && old->state!=BLOCKED) { Scheduler::put(old); }
    running = Scheduler::get();

    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->args);
    running->setFinished(true);
    thread_dispatch();
}

TCB *TCB::createThread(TCB::Body body, void *args, uint64* stackSpace) {
    thread_t thread = createThreadNoStart(body, args, stackSpace);
    if(thread->id != 0)thread->start();
    return thread;
}

int TCB::start() {
    Scheduler::put((TCB*)this);
    return 0;
}
