//
//  Process.cpp
//  Sim
//
//  Created by Farzad Shafiee on 4/30/13.
//  Copyright (c) 2013 Farzad Shafiee. All rights reserved.
//

#include "Process.h"

Process::Process(int processID, int arriveTime, int pr, int t) {
    type = t;
    pid = processID;
    arrival = arriveTime;
    priority = pr;
    jobRemain = 0;
    elapsedTime = arrival;
    estimatedTime = 0;
    jobRuntimes = queue<int>();
    jobIntensions = queue<int>();
    
    jobIntensions.push(0); //first run of a process pops one extra intension
    
    waitingTime = 0;
    responseTime = 0;
    firstResponse = false;
    turnAroundTime = 0;
    
    
}
int Process::run() {
    if (jobRemains()) jobRemain--;
    if (jobRemain  == 0) {
        jobIntensions.pop();
        if (jobRuntimes.size() == 0) {
            completed = true;
            return DONE;
        }
        
        while (jobRuntimes.front() <= 0) {
            jobRuntimes.pop();
            jobIntensions.pop();
            if (jobRuntimes.size() == 0) {
                completed = true;
                return DONE;
            }
        }
        jobRemain = jobRuntimes.front();
        jobRuntimes.pop();


    }    
    elapsedTime++;
    
    if(!firstResponse)  responseTime++;
    if (jobIntensions.front() == IO_INTENSE) {
        firstResponse = true;
    }
    
    return jobIntensions.front();
}

void Process::runIO() {
    firstResponse = true;
    elapsedTime += jobRemain;
    jobRemain = 0;
}
void Process::AddJob(int intension, int runTime) {
    jobIntensions.push(intension);
    jobRuntimes.push(runTime);
    estimatedTime += runTime;
    maxBurst = (runTime > maxBurst)? runTime : maxBurst;
    minBurst = (runTime > minBurst)? runTime : minBurst;
}

void Process::MakePrior(int upgradeVal) {
    priority = (priority-upgradeVal >= 0) ? priority-upgradeVal : 0;
}

float Process::getPriorityFactor() const{
    float pf;
    //y = 1 + (x-A)*(maxbound-minbound)/(B-A)
    float jb = (float)(jobRemain-minBurst)*(JB_RATIO)/(maxBurst-minBurst);
    float ar = (float)(arrival-0)*(ARR_RATIO)/(duration);
    float pr = (float)(priority-0)*(PR_RATIO)/(10);
    pf = jb+ar+pr;
    return pf;
}

int Process::maxBurst = 0;
int Process::minBurst = INT_MAX;
int Process::duration = 0;