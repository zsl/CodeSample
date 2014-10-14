/**
 * @file TimeCounter.h
 * @brief 一个计时器类，如果可以使用boost，应该使用boost::timer::cpu_timer类替代
 *
 * @author zsl, sanlo.zhang@gmail.com
 */

#ifndef TIME_COUNTER_H__
#define TIME_COUNTER_H__

#include <Windows.h>
/**
 * @brief CTimeCounter 类，以毫秒为单位。如果想获取更高精度，可以使用QueryPerformanceCounter 
 */
class CTimeCounter{
public:
    CTimeCounter();

    void          setTime(long time);

    /**
     * @brief 判断计时器是否停止
     */
    bool          isStopped() const;

    /**
     * @brief 返回从调用start()开始到现在所经历的毫秒数
     */
    long          elapsed()const; 

    /**
     * @brief 开始计时
     */
    void          start();

    /**
     * @brief 结束计时
     */
    void          stop();

    /**
     * @brief 继续进行计时
     *
     * 如果计时器停止，则掉用此函数后，计时在上一次调用stop()的地方继续进行；
     * 如果计时器没有停止，则调用此函数没有影响
     */
    void          resume();

private:
    long         m_time;
    bool          m_bStopped;

};

inline CTimeCounter::CTimeCounter(): m_bStopped(true), m_time(0){}

inline void CTimeCounter::setTime(long time)
{ 
    if (!m_bStopped){
        // 原理见resume
        m_time = GetTickCount() - time;
    }
    else
        m_time = time;
}

inline bool CTimeCounter::isStopped()const{ return m_bStopped; }

inline void CTimeCounter::start() { m_bStopped = false; m_time = GetTickCount(); }

inline void CTimeCounter::stop() 
{ 
    if (m_bStopped)
        return;

    m_bStopped = true;
    m_time = GetTickCount() - m_time;
}

inline long CTimeCounter::elapsed()const
{
    if ( m_bStopped )
        return m_time;

    return GetTickCount() - m_time;
}

inline void CTimeCounter::resume()
{
    // curTimePeriod
    // t1 = m_time
    // t2 = future_time
    // future_period = t2 - t1 + curTimePeriod
    // 因此，设置 m_time = t1 - curTimePeriod
    // future_period = future_time - m_time
    if (m_bStopped){
        long tmpTime = m_time;

        m_bStopped = false;
        m_time = GetTickCount();

        m_time -= tmpTime;
    }
}

#endif
