

#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "list.hpp"

class TCB;

class Scheduler
{
private:
    static List<TCB> readyThreadQueue;

public:
    static TCB *get();

    static void put(TCB *ccb);
    static char* writeout();

};

#endif //SCHEDULER_HPP
