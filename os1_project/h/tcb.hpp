
#ifndef TCB_HPP
#define TCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"


// Thread Control Block
class TCB
{
    enum STATE {CREATED, READY, BLOCKED};
public:
    TCB(){this->id = ID++;}
    ~TCB() { delete[] stack; }

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    uint64 getTimeSlice() const { return timeSlice; }

    using Body = void (*)(void*);

    static TCB *createThreadNoStart(Body body, void* args, uint64* stackSpace);
    static TCB *createThread(Body body, void* args, uint64* stackSpace);
    int start();

    //static void yield();

    static TCB *running;

private:
    TCB(Body body, void* args, uint64 timeSlice) :
            body(body),
            args(args),
            stack(body != nullptr ? new uint64[STACK_SIZE] : nullptr),
            context({(uint64) &threadWrapper,
                     stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0
                    }),
            timeSlice(timeSlice),
            finished(false)
    {
        this->id=ID++;
        //if (body != nullptr) { Scheduler::put(this); }
    }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    void* args;
    uint64 *stack;
    Context context;
    uint64 timeSlice;
    bool finished;
    static int ID;
    int id;
    STATE state;

    friend class Riscv;
    friend class sem;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    static uint64 timeSliceCounter;

    static uint64 constexpr STACK_SIZE = 1024;
    static uint64 constexpr TIME_SLICE = 2;
};

#endif //TCB_HPP
