
#pragma once

#include <iostream>
#include <iomanip>
#include <climits>
#include "Schedulers.h"

#define DIS_CORE -11
#define DEF_QNT 5

using namespace std;

#define NO_PROCESS 2
#define PROCESS_DONE 1
#define IN_PORG 0

#define DISABLED_CORE -1
#define THR_RATE 100

#define W_1 15
#define W_2 10
#define CORETAG "Core#"
#define NOPTAG "nop"
class Core {
private:
    const int cid;
    const int quantum;
    const int priorityDecrease;
    const int agingPeriod;
    const bool disabled;
    Scheduler* coreScheduler;
    Process* currentProcess;
    int totalProcess;
    int startFirstProcess, endLastProcess;
    deque<Process*> allProcess;

public:
    Core(int qnt, int pd, int aper, bool dis, int idN);
    int getQuantum() { return quantum;}
    int getPrDecrease() { return priorityDecrease;}
    int getAgingPeriod() { return agingPeriod;}
    void setScheduler(Scheduler* sch);
    
    int run(int time);
    
    int getCoreID(){
        if(disabled) return -1;
        return cid;
    }
    
    //Log functions
    float getThroughput();
    
    void dispTrunaroundInfo();
    void dispWatingtimeInfo();
    void dispResponseInfo();
    int getCurrentProcessID() {
        if (currentProcess == NULL) return -1;
        return currentProcess->getPID();
    }
    
    size_t reportLoad() {
        return coreScheduler->report();
    }
    
    //Optimized
    Process* migrate(Process* p);
};

