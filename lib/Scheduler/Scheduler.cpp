#include "Scheduler.h"

Scheduler::Scheduler()
{
}

Scheduler::~Scheduler()
{
}

void Scheduler::addTask(uint16_t nTime, Task& pTask)
{
    if (m_nCount < MAX_SCHEDULER_COUNT)
    {
        m_arTime[m_nCount] = nTime;
        m_arTasks[m_nCount] = pTask;
        m_nCount++;
    }
}

int Scheduler::start(uint16_t now)
{
    m_nCurrentIdx=0;
    sort();
    while ((m_nCurrentIdx)<m_nCount && m_arTime[m_nCurrentIdx]<=now)
        m_nCurrentIdx++;
    if (m_nCurrentIdx>=m_nCount)    
        m_nCurrentIdx = 0;
    return m_nCurrentIdx;    
}

void Scheduler::update(uint16_t now)
{
    if (m_nCurrentIdx<m_nCount)
    {
        if (m_arTime[m_nCurrentIdx]==now)
        {
            m_arTasks[m_nCurrentIdx].start();
            m_nCurrentIdx++;
            if (m_nCurrentIdx>=m_nCount)    
                m_nCurrentIdx = 0;
        }
    }
}

void Scheduler::sort()
{
    for (int i = 0; i < m_nCount; i++)
    {
        for (int j = i + 1; j < m_nCount; j++)
        {
            if (m_arTime[i] > m_arTime[j])
            {
                uint16_t nTemp = m_arTime[i];
                m_arTime[i] = m_arTime[j];
                m_arTime[j] = nTemp;

                Task pTemp = m_arTasks[i];
                m_arTasks[i] = m_arTasks[j];
                m_arTasks[j] = pTemp;
            }
        }
    }

}