//
//  Platform.cpp
//  Sim
//
//  Created by Farzad Shafiee on 5/1/13.
//  Copyright (c) 2013 Farzad Shafiee. All rights reserved.
//

#include "Platform.h"

Platform::Platform(int noc) {
    numOfCores = noc;
    cores = vector<Core*>(numOfCores);
    optimized = false;
    startFirstProcess = INT_MAX;
    endLastProcess = 0;
    totalProcess = 0;
    
    logfile.open("log.txt", ios::out | ios::trunc);
    logfile << left;
    logfile <<  setw(W_1) <<"TimeUnit" <<
                setw(W_1) << "Core#1" <<
                setw(W_1) << "Core#2" <<
                setw(W_1) << "Core#3" <<
                "Core#4\n" <<
                setw(W_1) <<"--------" <<
                setw(W_1) << "------" <<
                setw(W_1) << "------" <<
                setw(W_1) << "------" <<
                setw(W_1) << "------" <<
                endl;
}

int Platform::addCore(int index, Core* core) {
    if (index >= numOfCores || index < 0) {
        return -1;
    }
    cores[index] = core;
    return index;
}

void Platform::run(int time) {
    if (optimized) {
        longTermScheduler();
    }
    vector<Core*>::iterator it;
    int state = NO_PROCESS;
    logfile << "$" << setw(W_1-1) << time ;

    for (it = cores.begin(); it != cores.end(); it++) {
        state = (*it)->run(time);
        if (state == IN_PORG) {
            startFirstProcess = (startFirstProcess == INT_MAX)? time : startFirstProcess;
        }
        if (state == PROCESS_DONE) {
            totalProcess++;
            endLastProcess = time;
        }
        
        int cpid = (*it)->getCurrentProcessID();
        logfile << setw(W_1);
        if (cpid == -1) {
            logfile << NOPTAG;
        }
        else logfile << cpid;
    }
    
    logfile << endl;
}

float Platform::getThroughput() {
    if (startFirstProcess  == INT_MAX) return 0;
    float processorThr =  (float)(totalProcess*THR_RATE)/(endLastProcess-startFirstProcess);
    cout << left;
    cout << "\n[Throughput] |Processes/" << THR_RATE <<"tu(s)|\n";
    cout << setw(W_1) << "Processor" << setw(W_2) << processorThr << endl;
    
    vector<Core*>::iterator it;
    for (it = cores.begin(); it != cores.end(); it++) {
        cout << "Core#" << setw(10) << (*it)->getCoreID() << (*it)->getThroughput() << endl;
    }
    
    return processorThr;
}

void Platform::turnaroundInfo(deque<Process*> allProcess) {
    
    deque<Process*>::iterator it;
    int max = 0, min = INT_MAX;
    int total = 0, num = 0;
    for (it = allProcess.begin(); it != allProcess.end(); it++) {
        if((*it)->terminated()){
            int turn = (*it)->getTurnaround();
            
                total += turn;
                num++;
            
            max = (turn > max)? turn : max;
            min = (turn < min)? turn : min;
        }
    }
    float avg = (num == 0) ? 0 : (float)total/num;
    cout << left;
    cout << "\n[Turnaroud]\n";
    cout << setw(W_1) << "ID" <<
            setw(W_2) << "Min." <<
            setw(W_2) << "Max." <<
            setw(W_2) << "Avg." <<
            endl;
    cout << setw(W_1) << "Processor" <<
            setw(W_2) << min <<
            setw(W_2) << max <<
            setw(W_2) << avg <<
            endl;
    
    vector<Core*>::iterator cit;
    for (cit = cores.begin(); cit != cores.end(); cit++) {
        (*cit)->dispTrunaroundInfo();
    }
    
    
}
void Platform::waitingtimeInfo(deque<Process*> allProcess) {
    
    deque<Process*>::iterator it;
    
    int max = 0, min = INT_MAX;
    int total = 0, num = 0;
    for (it = allProcess.begin(); it != allProcess.end(); it++) {
        if ((*it)->terminated()) {
            int waitTime = (*it)->getWaitingTime();
            total += waitTime;
            num++;
            max = (waitTime > max)? waitTime : max;
            min = (waitTime < min)? waitTime : min;
        }
    }
    
    float avg = (num == 0) ? 0 : (float)total/num;
    cout << left;
    cout << "\n[Waiting Time]\n";
    cout << setw(W_1) << "ID" <<
            setw(W_2) << "Min." <<
            setw(W_2) << "Max." <<
            setw(W_2) << "Avg." <<
            endl;
    cout << setw(W_1) << "Processor" <<
            setw(W_2) << min <<
            setw
    (W_2) << max <<
            setw(W_2) << avg <<
            endl;
    
    vector<Core*>::iterator cit;
    for (cit = cores.begin(); cit != cores.end(); cit++) {
        (*cit)->dispWatingtimeInfo();
    }
}

void Platform::responsetimeInfo(deque<Process*> allProcess) {
    deque<Process*>::iterator it;
    
    int max = 0, min = INT_MAX;
    int total = 0, num = 0;
    for (it = allProcess.begin(); it != allProcess.end(); it++) {
        if ((*it)->responsed()) {
            int rsp = (*it)->getRspTime();
            total += rsp;
            num++;
            max = (rsp > max)? rsp : max;
            min = (rsp < min)? rsp : min;
        }
    }
    
    float avg = (num == 0) ? 0 : (float)total/num;
    cout << left;
    cout << "\n[Response Time]\n";
    cout << setw(W_1) << "ID" <<
            setw(W_2) << "Min." <<
            setw(W_2) << "Max." <<
            setw(W_2) << "Avg." <<
            endl;
    cout << setw(W_1) << "Processor" <<
            setw(W_2) << min <<
            setw(W_2) << max <<
            setw(W_2) << avg <<
            endl;
    
    vector<Core*>::iterator cit;
    for (cit = cores.begin(); cit != cores.end(); cit++) {
        (*cit)->dispResponseInfo();
    }
}

void Platform::longTermScheduler() {
    if (cores[3]->reportLoad() == 0){
        if (cores[0]->reportLoad() > 1)
            cores[3]->migrate(cores[0]->migrate(NULL));
    }
    else if(cores[0]->reportLoad() == 0){
        if (cores[3]->reportLoad() > 1)
            cores[0]->migrate(cores[3]->migrate(NULL));
    }
    
}
void Platform::dispTestEnvr(int totalTime){
    cout << "\n======= Test Environment Details =======\n";
    for (int i = 0; i < numOfCores; i++) {
        cout << "Core#" <<i+1 << ": ";
        if (cores[i]->getCoreID() == -1)
            cout << "Disabled";
        else
            cout << "Enabled";
        if (i != numOfCores-1) {
            cout <<",  ";
        }
        else cout << '\n';
    }
    cout << "Optimized: ";
    if (optimized) {
        cout << "True\n";
    }
    else cout << "False\n";
    cout << "Test Duration: " << totalTime << '\n';
    cout << "========================================\n";
    
}