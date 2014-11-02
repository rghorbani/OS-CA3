//
//  Process.h
//  Sim
//
//  Created by Farzad Shafiee on 4/30/13.
//  Copyright (c) 2013 Farzad Shafiee. All rights reserved.
//

#pragma once
#include <iostream>
#include <queue>

#define DONE 0
#define IO_INTENSE -1
#define CPU_INTENSE -2
#define CP 1
#define GP 2
#define OP 3

#define MAX_PR 10
#define PR_RATIO 55
#define ARR_RATIO 15
#define JB_RATIO 30

using namespace std;

class Process {
private:
    
    static int maxBurst;
    static int minBurst;
    static int duration;
    bool completed;
    int type;
    int pid;
    int arrival;
    int priority;
    int jobRemain;
    int elapsedTime;
    int estimatedTime;
    queue<int> jobRuntimes;
    queue<int> jobIntensions;
    
    //Log variables
    int waitingTime;    
    int responseTime;
    bool firstResponse;
    int turnAroundTime;

    
public:
    Process(int processID, int arriveTime, int pr, int t);
    int run();
    void runIO();
    void MakePrior(int upgradeVal);
    void AddJob(int intension, int runTime);
    
    int getPID() {
        return pid;
    }
    int getPriority() const {
        return priority;
    }
    int getArrival() const {
        return arrival;
    }
    int getJobRemain() const {
        return jobRemain;
    }
    int getProcessType() const {
        return type;
    }
    int getTotalTime() const {
        return elapsedTime;
    }
    int getIntensity() {
        return jobIntensions.front();
    }
    
    bool hasJob(){
        if (jobIntensions.size() > 0) {
            return true;
        }
        return false;
    }
    bool jobRemains() {
        if  (jobRemain > 0) return true;
        return false;
    }
    
    //Optimization
    
    float getPriorityFactor() const;
    
    //Log functions
    void waitUp() {
        waitingTime++;
        if(!firstResponse)  responseTime++;
    }

    void turnAround() {
        if (!completed) turnAroundTime++;
    }
    
    int getWaitingTime() {
        if (!completed) return 0;
        return waitingTime;
    }

    int getTurnaround() {
        if (!completed) return 0;
        return turnAroundTime;
    }
    
    int getRspTime() {
        return  responseTime;
    }
    
    bool terminated() {
        return completed;
    };
    bool responsed() {
        return firstResponse;
    }
    
    //
    static void setDuration(int d) {
    duration = d;
    }
};




