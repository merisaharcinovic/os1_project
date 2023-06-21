//
// Created by os on 9/11/22.
//

#ifndef PROJECT_BASE_V1_1_1_SEM_HPP
#define PROJECT_BASE_V1_1_1_SEM_HPP

#include "list.hpp"
#include "tcb.hpp"
class sem{

public:
    sem(int val);
    int signal();
    int wait();
    static int sem_close(sem* handle);
    ~sem();

    friend class Riscv;
private:
    int value;
    bool isClosed;
    List<TCB> waitList;

};


#endif //PROJECT_BASE_V1_1_1_SEM_HPP
