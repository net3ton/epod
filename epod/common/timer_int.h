#ifndef __COMMON_TIMER_INT_H__
#define __COMMON_TIMER_INT_H__


class TimerInt
{
public:
  TimerInt(): m_maxTime(100), m_t(0), m_cycled(false), m_reverce(true), m_justEnded(false) {}

  void setMaxTime(int time) { m_maxTime = time; }

  void start(int time) { setMaxTime(time); restart(); }
  void startCycled(int time) { start(time); m_cycled = true; }

  void reverce(bool reverce) { m_reverce = reverce; }

  int getMaxTime() const { return m_maxTime; }
  int getTime() const { return m_t; }

  template<class T>
  T getVal(T max) const { return (m_t * max / m_maxTime); }
  template<class T>
  T getInvVal(T max) const { return ((m_maxTime - m_t) * max / m_maxTime); }

  float getProgress() const { return (m_maxTime - m_t) / (float)m_maxTime; }

  void restart()
  {
    if (m_reverce)
      m_t = m_maxTime;
    else
      m_t = 0;
  }

  void end()
  {
    if (m_reverce)
      m_t = 0;
    else
      m_t = m_maxTime;
  }

  bool isWorking() const { return (m_reverce ? (m_t != 0) : (m_t != m_maxTime)) || m_justEnded; }
  bool isEnd() const { return !isWorking(); }
  bool isReverce() const { return m_reverce; }

  bool isDirectEnd() const { return m_justEnded && !m_reverce; }
  bool isReverceEnd() const { return m_justEnded && m_reverce; }

  bool update(int dt)
  {
    m_justEnded = false;

    if (m_reverce)
    {
      if (!m_t)
        return false;

      m_t -= dt;
      if (m_t <= 0)
      {
        m_justEnded = true;

        if (m_cycled)
          m_t += m_maxTime;
        else
          m_t = 0;
        return true;
      }
    }
    else
    {
      if (m_t == m_maxTime)
        return false;

      m_t += dt;
      if (m_t >= m_maxTime)
      {
        m_justEnded = true;

        if (m_cycled)
          m_t -= m_maxTime;
        else
          m_t = m_maxTime;
        return true;
      }
    }

    return false;
  }

private:
  bool m_cycled;
  bool m_reverce;
  bool m_justEnded;
  int m_maxTime;
  int m_t;
};


#endif // __COMMON_TIMER_INT_H__
