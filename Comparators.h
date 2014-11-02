//
//  Comparators.h
//  Sim
//
//  Created by Farzad Shafiee on 5/4/13.
//  Copyright (c) 2013 Farzad Shafiee. All rights reserved.
//



#pragma once
#include "Process.h"

using namespace std;

//Comparators
class JobCompare {
public:
    bool operator()(const Process* l, const Process* r) const {
        return l->getJobRemain() < r->getJobRemain();
    }
};

class PriorityCompare {
public:
    bool operator()(const Process* l, const Process* r) const {
        return l->getPriority() < r->getPriority();
    }
};

class ArrivalCompare {
public:
    bool operator()(const Process* l, const Process* r) const {
        return l->getArrival() < r->getArrival();
    }
};

class IOCompare {
public:
    bool operator()(const Process* l, const Process* r) const {
        return l->getTotalTime() < r->getTotalTime();
    }
    
};

class FactorCompare {
public:
    bool operator()(const Process* l, const Process* r) const {
        return l->getPriorityFactor() < r->getPriorityFactor();
    }
    
};