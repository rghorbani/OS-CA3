

#include "Schedulers.h"



Process* Scheduler::getNextProcess(Process *prevProc, int coreID) {
    return NULL;
}

void Scheduler::addProcess(Process *p) {
    readyQueue.push_back(p);
}

void Scheduler::evalWaitings() {
    deque<Process*>::iterator it;
    for(it = readyQueue.begin(); it != readyQueue.end(); it++) {
        (*it)->waitUp();
    }
}

Process* C14::getNextProcess(Process *prevProc, int coreID) {
    
    sysTime++;
    Process* p = NULL;
  
    while (!ioQueue.empty()) {
        if (ioQueue.top()->getTotalTime() > sysTime) break;
        readyQueue.push_back(ioQueue.top());
        ioQueue.pop();
    }
    
    if (prevProc != NULL) {
        if (prevProc->getIntensity() == IO_INTENSE) {
            prevProc->runIO();
            ioQueue.push(prevProc);
        }
        else {
            if (prevProc->jobRemains()) {
                if (rq > 1) {
                    rq--;
                    evalWaitings();
                    return prevProc;
                }
                else if(readyQueue.size())
                    readyQueue.push_back(prevProc);
                else {
                    rq = quantum;
                    evalWaitings();
                    return prevProc;
                }
            }
            else if (prevProc->hasJob()) readyQueue.push_back(prevProc);
        }
    }
    rq = quantum;
    if (!readyQueue.empty()) {
        p = readyQueue.front();
        readyQueue.pop_front();
        rq--; //Context switch cost
    }
    evalWaitings();
    return p;
}

Process* C14::migrateProcess(Process* p) {
    Process* mp = NULL;
    if (p == NULL) {
        if (readyQueue.size()) {
            mp = readyQueue.front();
            readyQueue.pop_front();
        }
        return mp;
    }
    addProcess(p);
    return mp;
}

Process* C23::getNextProcess(Process *prevProc, int coreID) {
    
    sysTime++;
    
    Process* p = NULL;
    
    while (!ioQueue.empty()) {
        if (ioQueue.top()->getTotalTime() > sysTime) break;
        readyQueue.push_back(ioQueue.top());
        ioQueue.pop();
    }
    
    if (sysTime%agingPeriod == 0) AgeUp();
    if(coreID == 2)
        stable_sort(readyQueue.begin(), readyQueue.end(), PriorityCompare());
    else
        stable_sort(readyQueue.begin(), readyQueue.end(), JobCompare());
    
    if (prevProc != NULL) {
        if (prevProc->getIntensity() == IO_INTENSE) {
            prevProc->runIO();
            ioQueue.push(prevProc);
        }
        else {
            
            if (prevProc->jobRemains()) {
                evalWaitings();
                return prevProc;
            }
            if (prevProc->hasJob()) readyQueue.push_back(prevProc);
        }
    }
    
    if (!readyQueue.empty()) {
        p = readyQueue.front();
        readyQueue.pop_front();
    }
    
    evalWaitings();
    return p;
}

void C23::AgeUp() {

    deque<Process*>::iterator it;
    for (it = readyQueue.begin() ; it != readyQueue.end(); it++) {
        (*it)->MakePrior(downgradeVal);
    }
}


Process* C14OPT::getNextProcess(Process *prevProc, int coreID){
    sysTime++;
    Process* p = NULL;
    bool sort = false;
    
    while (!ioQueue.empty()) {
        if (ioQueue.top()->getTotalTime() > sysTime) break;
        readyQueue.push_back(ioQueue.top());
        ioQueue.pop();
        sort = true;
    }
    
    if (prevProc != NULL) {
        if (prevProc->getIntensity() == IO_INTENSE) {
            prevProc->runIO();
            ioQueue.push(prevProc);
        }
        else {
            if (prevProc->jobRemains()) {
                if (rq > 1) {
                    rq--;
                    evalWaitings();
                    return prevProc;
                }
                else if(readyQueue.size()){
                    readyQueue.push_back(prevProc);
                    sort = true;
                }
                else {
                    rq = quantum;
                    evalWaitings();
                    return prevProc;
                }
            }
            else if (prevProc->hasJob()) {
                readyQueue.push_back(prevProc);
                sort = true;
            }
        }
    }
    
    if(sort)
        stable_sort(readyQueue.begin(), readyQueue.end(), FactorCompare());
    
    rq = quantum;
    if (!readyQueue.empty()) {
        p = readyQueue.front();
        readyQueue.pop_front();
        rq--;
    }
    
    evalWaitings();
    return p;
}

void C14OPT::addProcess(Process *p) {
    readyQueue.push_back(p);
    sort(readyQueue.begin(), readyQueue.end(), FactorCompare());
}