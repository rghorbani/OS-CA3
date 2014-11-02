
#include "Core.h"

Core::Core(int qnt, int pd, int aper, bool dis, int idN) :
quantum(qnt), priorityDecrease(pd), agingPeriod(aper), disabled(dis), cid(idN) {
    coreScheduler = NULL;
    currentProcess = NULL;
    totalProcess = 0;
    startFirstProcess = INT_MAX;
    endLastProcess = 0;
    allProcess = deque<Process*>();
}
void Core::setScheduler(Scheduler* sch){
    coreScheduler = sch;
}


int Core::run(int time) {
    if (disabled) return DISABLED_CORE;
    if (currentProcess != NULL)
        if (currentProcess->run() == DONE) {
            allProcess.push_back(currentProcess);
            currentProcess = NULL;
            endLastProcess = time;
            totalProcess++;
            return PROCESS_DONE;
        }
    currentProcess =  coreScheduler->getNextProcess(currentProcess, cid);
    if (currentProcess != NULL) {
        startFirstProcess = (startFirstProcess == INT_MAX)? time :startFirstProcess;
        return IN_PORG;
    }
    return NO_PROCESS;
    
}

Process* Core::migrate(Process* p){
    if(p == NULL) {
        return ((C14*)coreScheduler)->migrateProcess(NULL);
    }
    
    return ((C14*)coreScheduler)->migrateProcess(p);
}


float Core::getThroughput() {
    if (disabled) {
        return 0;
    }
    if (startFirstProcess  == INT_MAX) return 0;
    if(endLastProcess-startFirstProcess == 0) return 0;
    return (float)(totalProcess*THR_RATE)/(endLastProcess-startFirstProcess);
}

void Core::dispTrunaroundInfo() {
    deque<Process*>::iterator it;
    int max = 0, min = INT_MAX;
    int total = 0, num = 0;
    for (it = allProcess.begin(); it != allProcess.end(); it++) {
        if ((*it)->terminated()) {
                
            int turn = (*it)->getTurnaround();
            num++;
            total += turn;
            max = (turn > max)? turn : max;
            min = (turn < min)? turn : min;
        }
    }
    if(!num){
        cout << left;
        cout << CORETAG <<  setw(W_1-sizeof(CORETAG)+1) << cid <<
        setw(W_2) << NOPTAG <<
        setw(W_2) << NOPTAG <<
        setw(W_2) << NOPTAG <<
        endl;
        return;
        
    }
    
    float avg = (num == 0)? 0 :(float)total/num;
    cout << left;
    cout << CORETAG <<  setw(W_1-sizeof(CORETAG)+1) << cid <<
    setw(W_2) << min <<
    setw(W_2) << max <<
    setw(W_2) << avg <<
    endl;
}

void Core::dispWatingtimeInfo() {
    deque<Process*>::iterator it;
    int max = 0, min = INT_MAX;
    int total = 0, num = 0;
    for (it = allProcess.begin(); it != allProcess.end(); it++) {
        if ((*it)->terminated()) {
            int waiting = (*it)->getWaitingTime();
            total += waiting;
            num++;
            max = (waiting > max)? waiting : max;
            min = (waiting < min)? waiting : min;
        }
    }
    if(!num){
        cout << left;
        cout << CORETAG <<  setw(W_1-sizeof(CORETAG)+1) << cid <<
        setw(W_2) << NOPTAG <<
        setw(W_2) << NOPTAG <<
        setw(W_2) << NOPTAG <<
        endl;
        return;
        
    }
    
    float avg = (num == 0)? 0 :(float)total/num;
    cout << left;
    cout << CORETAG <<  setw(W_1-sizeof(CORETAG)+1) << cid <<
    setw(W_2) << min <<
    setw(W_2) << max <<
    setw(W_2) << avg <<
    endl;
}
void Core::dispResponseInfo() {
    deque<Process*>::iterator it;
    int max = 0, min = INT_MAX;
    int total = 0, num = 0;
    for (it = allProcess.begin(); it != allProcess.end(); it++) {
        if ((*it)->responsed()) {
            int resp = (*it)->getRspTime();
            total += resp;
            num++;
            max = (resp > max)? resp : max;
            min = (resp < min)? resp : min;
        }
    }
    if(!num){
        cout << left;
        cout << CORETAG <<
            setw(W_1-sizeof(CORETAG)+1) << cid <<
            setw(W_2) << NOPTAG <<
            setw(W_2) << NOPTAG <<
            setw(W_2) << NOPTAG <<
            endl;
        return;
        
    }
    
    float avg = (num == 0)? 0 :(float)total/num;
    cout << left;
    cout << CORETAG <<  setw(W_1-sizeof(CORETAG)+1) << cid <<
            setw(W_2) << min <<
            setw(W_2) << max <<
            setw(W_2) << avg <<
            endl;
}