/**
 * @file TimeCounter.h
 * @brief һ����ʱ���࣬�������ʹ��boost��Ӧ��ʹ��boost::timer::cpu_timer�����
 *
 * @author zsl, sanlo.zhang@gmail.com
 */

#ifndef TIME_COUNTER_H__
#define TIME_COUNTER_H__

#include <Windows.h>
/**
 * @brief CTimeCounter �࣬�Ժ���Ϊ��λ��������ȡ���߾��ȣ�����ʹ��QueryPerformanceCounter 
 */
class CTimeCounter{
public:
    CTimeCounter();

    void          setTime(long time);

    /**
     * @brief �жϼ�ʱ���Ƿ�ֹͣ
     */
    bool          isStopped() const;

    /**
     * @brief ���شӵ���start()��ʼ�������������ĺ�����
     */
    long          elapsed()const; 

    /**
     * @brief ��ʼ��ʱ
     */
    void          start();

    /**
     * @brief ������ʱ
     */
    void          stop();

    /**
     * @brief �������м�ʱ
     *
     * �����ʱ��ֹͣ������ô˺����󣬼�ʱ����һ�ε���stop()�ĵط��������У�
     * �����ʱ��û��ֹͣ������ô˺���û��Ӱ��
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
        // ԭ���resume
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
    // ��ˣ����� m_time = t1 - curTimePeriod
    // future_period = future_time - m_time
    if (m_bStopped){
        long tmpTime = m_time;

        m_bStopped = false;
        m_time = GetTickCount();

        m_time -= tmpTime;
    }
}

#endif
