//
//  Schedulers.h
//  Sim
//
//  Created by Farzad Shafiee on 5/2/13.
//  Copyright (c) 2013 Farzad Shafiee. All rights reserved.
//

#pragma once
#include <queue>
#include <algorithm>
#include "Process.h"
#include "Comparators.h"

using namespace std;

class Scheduler {
protected:
    int sysTime;
    deque<Process*> readyQueue;
    priority_queue<Process*, vector<Process*>, IOCompare> ioQueue;
public:
    Scheduler() {
        readyQueue = deque<Process*>();
        sysTime = 0;
    }
    virtual Process* getNextProcess(Process* prevProc, int coreID);
    virtual void addProcess(Process* p);
    void evalWaitings();
    void AgeUp();
    size_t report() { return readyQueue.size();}
    
};

class C14: public Scheduler {
protected:
    const int quantum;
    int rq;
public:
    C14(int qnt):quantum(qnt){
        rq = quantum;
        Scheduler();
    }
    virtual Process* getNextProcess(Process* prevProc, int coreID);
    Process* migrateProcess(Process* p);

};


class C23: public Scheduler {
protected:
    const int quantum;
    const int downgradeVal;
    const int agingPeriod;
//    priority_queue<Process*, vector<Process*>, JobCompare> prioQ;
    
public:
    
    C23(int qnt, int pd, int aper) :quantum(qnt), downgradeVal(pd),agingPeriod(aper){
        Scheduler();
        
    }
    virtual Process* getNextProcess(Process* prevProc, int coreID);
    virtual void AgeUp();

};


class C14OPT: public C14 {
private:
    const int quantum;
    int rq;
public:
    C14OPT(int qnt):quantum(qnt),C14(qnt){
        rq = quantum;
    }
    Process* getNextProcess(Process* prevProc, int coreID);
    void addProcess(Process* p);
};