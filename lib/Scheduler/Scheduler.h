// create scheduler task
#pragma once
#include <stdint.h>
#include "Task.h"

#define MAX_SCHEDULER_COUNT 100
class Scheduler
{
private:
    uint16_t m_arTime[MAX_SCHEDULER_COUNT];
    Task     m_arTasks[MAX_SCHEDULER_COUNT];

    int m_nCount=0;
    int m_nCurrentIdx = 0;
public:
    Scheduler();
    ~Scheduler();
    void addTask(uint16_t nTime, Task& pTask);
    int start(uint16_t now);
    void update(uint16_t now);
private:    
    void sort();    
};


