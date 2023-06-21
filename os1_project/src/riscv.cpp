

#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
#include "../lib/mem.h"
#include "../h/print.hpp"
#include "../h/sem.hpp"


void Riscv::popSppSpie()
{
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap()
{
    using Body = void (*)(void*);
    uint64 scause = r_scause();
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        uint64 opCode;
        opCode=r_a0();
        switch (opCode)
        {
            case(0x01):{ // mem_alloc
                uint64 size = r_a1();
                void* p = __mem_alloc((size_t)size);
                w_return_val((uint64)p);
                break;
            }
            case(0x02): { // mem_free
                int returnValue;
                uint64 p = r_a1();
                returnValue = __mem_free((void *)p);
                w_return_val(returnValue);
            }
            case 0x11:
            { //thread_create
                uint64 body;
                Body start_routine;

                void *args;
                uint64 arg;

                uint64 handle_arg;
                thread_t* handle;
                uint64 stack;

                handle_arg=r_a1();
                body=r_a2();
                arg=r_a3();
                stack=r_a4();

                handle = (thread_t*) handle_arg;
                start_routine=(Body)body;
                args=(void*)arg;

                *handle = TCB::createThread(start_routine, args, (uint64*)stack);

                if(!handle) w_return_val(-1);
                else {
                    TCB** h = (TCB**)r_a1();
                    *(h) = *handle;
                    w_return_val(0);
                }

                break;
            }
            case 0x14:
            { //thread_create_no_start
                uint64 body;
                Body start_routine;

                void *args;
                uint64 arg;

                uint64 handle_arg;
                thread_t* handle;
                uint64 stack;

                handle_arg=r_a1();
                body=r_a2();
                arg=r_a3();
                stack=r_a4();

                handle = (thread_t*) handle_arg;
                start_routine=(Body)body;
                args=(void*)arg;

                *handle = (thread_t)TCB::createThreadNoStart(start_routine, args, (uint64*)(stack));
                if(handle)w_return_val(0);//uspesno
                else w_return_val(-1);//neuspesno
                break;
            }
            case 0x15:{
                //thread_start
                thread_t thread = (thread_t)r_a1();
                if(!thread) w_return_val(-1);
                else {
                    uint64 res= thread->start();
                    w_return_val(res);
                }

                break;
            }
            case 0x13:
            { //thread_dispatch
                TCB::dispatch();
                break;
            }
            case 0x12:
            { //thread_exit
                TCB *cur = TCB::running;
                cur->setFinished(true);
                if (!(cur->isFinished())){
                    w_return_val(-1);
                    break;
                }
                TCB::dispatch();
                w_return_val(0);

                break;

            }
            case 0x21:
            {//sem_open
                sem_t* handle;
                int init;
                handle = (sem_t*)r_a2();
                init = (int)r_a1();
                *handle = new sem(init);
                if(!handle) w_return_val(-1);
                w_return_val(0);
                break;
            }
            case 0x22:
            {//sem_close
                sem_t handle;

                handle = (sem_t)r_a1();
                int res = sem::sem_close(handle);
                w_return_val(res);
                break;
            }
            case 0x23:
            {//sem_wait
                sem_t handle;
                handle = (sem_t)r_a1();
                int res = handle->wait();
                w_return_val(res);
                break;
            }
            case 0x24:
            {//sem_signal
                sem_t handle;
                handle = (sem_t)r_a1();
                int res = handle->signal();
                w_return_val(res);
                break;
            }
            case 0x41:
            {//getc
                char c = __getc();
                c=(uint64)c;
                w_return_val(c);
                break;
            }
            case 0x42:
            {//putc
                char c;
                c = (char)r_a1();
                __putc(c);
                break;
            }
        }

        w_sstatus(sstatus);
        w_sepc(sepc);
    } else if (scause == 0x8000000000000001UL)
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
        TCB::timeSliceCounter++;
        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {
            uint64 volatile sepc = r_sepc();
            uint64 volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            //TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
    } else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    } else
    {
        // unexpected trap cause
    }
}

