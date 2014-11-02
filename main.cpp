//
//  main.cpp
//  Sim
//
//  Created by Farzad Shafiee on 4/30/13.
//  Copyright (c) 2013 Farzad Shafiee. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include "Platform.h"
#include <string>
#include <algorithm>

using namespace std;

//Global vars

ifstream config, testcase;
Platform* simPlat;
deque<Process*> *mainQueue;
int simulationTime;
//----


Platform* MakePlatform(int numOfCores) {

    simulationTime = DEF_SIMTIM;
    int agePeriod = 0, agePrio = 0;
    bool optimize = false;
    int quantums[numOfCores];
    bool enabled[numOfCores];
    for (int i = 0; i < numOfCores; i++) {
        quantums[i] = DEF_QNT;
        enabled[i] = false;
    }
    string line, t;
    
    line.reserve(30);
    while (getline(config, line)) {
        transform(line.begin(), line.end(), line.begin(), ::tolower);
        stringstream ss;
        ss << line;
        ss >> t;
        if (t.find("duration") != string::npos) {
            ss >> simulationTime;
        }
        else if (t.find("optimization") != string::npos) {
            int opt = 0;
            ss >> opt;
            if (opt) optimize = true;
        }
        else if (t.find("cores") != string::npos) {
            int en;
            for (int i = 0; i < numOfCores; i++) {
                if(ss >> en) {
                    if (en) enabled[i] = true;
                }
                else break;
            }
            
        }
        else if (t.find("c23aging") != string::npos) {
            ss >> agePeriod >> agePrio;
        }
        else if (t.find("q") != string::npos) {
            ss >> quantums[atoi(&line[1])-1];
        }
        flush(ss);
    }
    
    Platform* simPlatform = new Platform(numOfCores);
    for (int i = 0; i < numOfCores; i++) {
        simPlatform->addCore(i, new Core(quantums[i], agePeriod, agePrio, !enabled[i], i+1));
    }
    if (optimize) {
        simPlatform->setOptimization();
        
    }
    
    return simPlatform;
}

deque<Process*>* QueueProcess() {
    deque<Process*> *processQueue = new deque<Process*>();
    string line;
    Process* p;
    int pid, arriveTime, priority, type;
    while (testcase >> line) {
        transform(line.begin(), line.end(), line.begin(), ::tolower);
        string terminatingTag;
        if (line.compare("<cp>") == 0) {
            terminatingTag = "</cp>";
            type = CP;
        }
        else if (line.compare("<gp>") == 0) {
            terminatingTag = "</gp>";
            type = GP;
        }
        else if (line.compare("<op>") == 0) {
            terminatingTag = "</op>";
            type = OP;
        }
        else continue;
        
        testcase >> pid >> arriveTime >> priority;
        p = new Process(pid, arriveTime, priority, type);
        
        testcase >> line;
        transform(line.begin(), line.end(), line.begin(), ::tolower);
        while (line.compare(terminatingTag) != 0) {
            
            int runTime = atoi(&line[2]);
            switch (line[0]) {
                case 'x':
                    p->AddJob(CPU_INTENSE, runTime);
                    break;
                case 'o':
                    p->AddJob(IO_INTENSE, runTime);
                    break;
                default:
                    break;
            }
            
            testcase >> line;
            transform(line.begin(), line.end(), line.begin(), ::tolower);
        }
        processQueue->push_back(p);

    }
    sort(processQueue->begin(), processQueue->end(), ArrivalCompare());
    Process::setDuration(simulationTime);
    return processQueue;
}

void ScheduleAndRun(){
    Scheduler *s1, *s23, *s4;
    deque<Process*> scheduledProcesses = deque<Process*>();
    
    if (simPlat->isOptimized()) {
        s1 = new C14OPT(simPlat->getCore(0)->getQuantum());
        s23 = new C23(simPlat->getCore(1)->getQuantum(), simPlat->getCore(1)->getPrDecrease(), simPlat->getCore(1)->getAgingPeriod());
        s4 = new C14OPT(simPlat->getCore(3)->getQuantum());
    }
    else {
        s1 = new C14(simPlat->getCore(0)->getQuantum());
        s23 = new C23(simPlat->getCore(1)->getQuantum(), simPlat->getCore(1)->getPrDecrease(), simPlat->getCore(1)->getAgingPeriod());
        s4 = new C14(simPlat->getCore(3)->getQuantum());
        
    }
    
    simPlat->getCore(0)->setScheduler(s1);
    simPlat->getCore(1)->setScheduler(s23);
    simPlat->getCore(2)->setScheduler(s23);
    simPlat->getCore(3)->setScheduler(s4);
    int time = 0;
    while (time <= simulationTime) {
        Process* p;
        if (!mainQueue->empty())
            while (mainQueue->front()->getArrival() == time) {
                p = mainQueue->front();
                mainQueue->pop_front();
                scheduledProcesses.push_back(p);
                int t = p->getProcessType();
                switch (t) {
                    case CP:
                        s1->addProcess(p);
                        break;
                    case GP:
                        s23->addProcess(p);
                        break;
                    case OP:
                        s4->addProcess(p);
                        break;
                    default:
                        break;
                }
                if (mainQueue->empty()) break;
            }
        simPlat->run(time);
        deque<Process*>::iterator it;
        for (it = scheduledProcesses.begin(); it != scheduledProcesses.end(); it++) {
            (*it)->turnAround();
        }
        
        time++;
    }
    
    //Log
    simPlat->dispTestEnvr(simulationTime);
    simPlat->getThroughput();
    simPlat->turnaroundInfo(scheduledProcesses);
    simPlat->waitingtimeInfo(scheduledProcesses);
    simPlat->responsetimeInfo(scheduledProcesses);
    flush(cout);
    
}

int main(int argc, const char * argv[])
{
    
    config.open("config.txt");
    testcase.open("testcase.txt");
    simPlat = MakePlatform(DEF_CORES);
    mainQueue = QueueProcess();
    ScheduleAndRun();
    
    
    return 0;
}

