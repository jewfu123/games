#include "SimpleTimer.h"
#include "cocos2d.h"

using namespace cocos2d;

SimpleTimer::SimpleTimer()
{
	reset();
}

void SimpleTimer::reset()
{
	m_paused=true;
	m_pauseTime=getMillisecond();
	m_startTime=m_pauseTime;
}

void SimpleTimer::setMillis(long millis)
{
	m_pauseTime=getMillisecond();
	m_startTime=m_pauseTime+millis;
}

long SimpleTimer::getMillis()
{
	long currentTime=getMillisecond();
	long surplusTime=currentTime-m_startTime-(m_paused?currentTime-m_pauseTime:0);
	return surplusTime>0?surplusTime:0;
}

void SimpleTimer::start()
{
	if(m_paused)
	{
		m_paused=false;
		m_startTime+=getMillisecond()-m_pauseTime;
	}
}

void SimpleTimer::pause()
{
	if(!m_paused)
	{
		m_paused=true;
		m_pauseTime=getMillisecond();
	}
}

bool SimpleTimer::paused()
{
	return m_paused;
}

unsigned long SimpleTimer::getMillisecond()
{
	/*cc_timeval now;
	CCTime::gettimeofdayCocos2d(&now, NULL);
	return (now.tv_sec * 1000 + now.tv_usec / 1000);*/

	struct cc_timeval now; 
	CCTime::gettimeofdayCocos2d(&now, NULL);
	unsigned long  time = ((unsigned long)now.tv_sec) * 1000+ now.tv_usec / 1000;
	return time ;
}


CountdownTimer::CountdownTimer(long millis)
{
	setMillis(millis);
	reset();
}

void CountdownTimer::reset()
{
	m_paused=true;
	m_pauseTime=SimpleTimer::getMillisecond();
	m_startTime=m_pauseTime;
}

void CountdownTimer::timeout()
{
	m_startTime=0;
}

void CountdownTimer::setMillis(long millis)
{
	m_millisecond=millis;
}

long CountdownTimer::getMillis()
{
	return m_millisecond;
}

void CountdownTimer::setSurplusMillis(long millis)
{
	m_pauseTime=SimpleTimer::getMillisecond();
	m_startTime=m_pauseTime+millis-m_millisecond;
}

long CountdownTimer::getSurplusMillis()
{
	long currentTime=SimpleTimer::getMillisecond();
	long surplusTime=m_startTime+m_millisecond-currentTime+(m_paused?currentTime-m_pauseTime:0);
	return surplusTime>0?surplusTime:0;
}

void CountdownTimer::start()
{
	if(m_paused)
	{
		m_paused=false;
		m_startTime+=SimpleTimer::getMillisecond()-m_pauseTime;
	}
}

void CountdownTimer::pause()
{
	if(!m_paused)
	{
		m_paused=true;
		m_pauseTime=SimpleTimer::getMillisecond();
	}
}

bool CountdownTimer::paused()
{
	return m_paused;
}