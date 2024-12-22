#ifndef __SIMPLE_TIMER_H__
#define __SIMPLE_TIMER_H__

#include "../platform.h"

class SimpleTimer
{
private:
	long m_startTime;
	long m_pauseTime;
	bool m_paused;

public:
    SimpleTimer();
    
    void reset();
    void setMillis(long millis);
    long getMillis();
    void start();
    void pause();
    bool paused();

	static unsigned long getMillisecond();
};

class CountdownTimer
{
private:
    long m_millisecond;
    long m_startTime;
    long m_pauseTime;
    bool m_paused;
    
public:
    CountdownTimer(long millis=0);
    void reset();
    void timeout();
    void setMillis(long millis);
    long getMillis();
    void setSurplusMillis(long millis);
    long getSurplusMillis();
    void start();
    void pause();
    bool paused();
};

#endif