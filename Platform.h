//
//  Platform.h
//  Sim
//
//  Created by Farzad Shafiee on 5/1/13.
//  Copyright (c) 2013 Farzad Shafiee. All rights reserved.
//

#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <climits>
#include "Core.h"

#define DEF_CORES 4
#define DEF_SIMTIM 200

using namespace std;

class Platform {

private:
    int numOfCores;
    bool optimized;
    vector<Core*> cores;
    int startFirstProcess, endLastProcess;
    int totalProcess;
    ofstream logfile;
    
public:
    Platform(int noc);
    int addCore(int index, Core* core);
    Core* getCore(int index) { return cores[index];}
    void setOptimization() { optimized = true;}
    bool isOptimized(){return optimized;}
    void run(int time);
    
    //Log functions
    void dispTestEnvr(int totalTime);
    float getThroughput();
    void turnaroundInfo(deque<Process*> allProcess);
    void waitingtimeInfo(deque<Process*> allProcess);
    void responsetimeInfo(deque<Process*> allProcess);
    void longTermScheduler();
};
