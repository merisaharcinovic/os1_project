
#include "../h/tcb.hpp"
#include "../h/workers.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"
#include "../test/printing.hpp"

#include "../test/Threads_C_API_test.hpp" // zadatak 2, niti C API i sinhrona promena konteksta
//#include "../test/Threads_CPP_API_test.hpp" // zadatak 2., niti CPP API i sinhrona promena konteksta

#include "../test/ConsumerProducer_C_API_test.h" // zadatak 3., kompletan C API sa semaforima, sinhrona promena konteksta
//#include "../test/ConsumerProducer_CPP_Sync_API_test.hpp" // zadatak 3., kompletan CPP API sa semaforima, sinhrona promena konteksta

//#include "../test/ThreadSleep_C_API_test.hpp" // thread_sleep test C API
//#include "../test/ConsumerProducer_CPP_API_test.hpp" // zadatak 4. CPP API i asinhrona promena konteksta
sem_t sem;



void workerA(void* arg){
    sem_wait(sem);
    for(int i=0; i<5;i++){
        printString("A");
        printInt(i);
        printString("\n");
    }
}
void workerB(void* arg){
    for(int i=0; i<10;i++){
        printString("B");
        printInt(i);
        printString("\n");
    }
    sem_signal(sem);
}
void userMain(void* arg) {
    Threads_C_API_test(); // zadatak 2., niti C API i sinhrona promena konteksta
    //Threads_CPP_API_test(); // zadatak 2., niti CPP API i sinhrona promena konteksta

    producerConsumer_C_API(); // zadatak 3., kompletan C API sa semaforima, sinhrona promena konteksta

//    producerConsumer_CPP_Sync_API(); // zadatak 3., kompletan CPP API sa semaforima, sinhrona promena konteksta

    //testSleeping(); // thread_sleep test C API
    //ConsumerProducerCPP::testConsumerProducer(); // zadatak 4. CPP API i asinhrona promena konteksta, kompletan test svega
//    sem_open(&sem, 0);
//    thread_t workerAthread;
//    thread_t workerBthread;
//    thread_create(&workerAthread, workerA, nullptr);
//    thread_create(&workerBthread,workerB, nullptr);
//
//    while(!(workerBthread->isFinished() && workerAthread->isFinished())){
//        thread_dispatch();
//    }
}

int main()
{
    TCB::running = TCB::createThreadNoStart(nullptr, nullptr, nullptr);


    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    printString("MAIN START\n");

    thread_t user;
    thread_create(&user, userMain, nullptr);

    while(!user->isFinished()) {
        thread_dispatch();
    } // join user main

    delete user;

    printString("Finished\n");

    delete TCB::running;

    return 0;
}
