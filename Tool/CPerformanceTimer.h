// CTerrain.h에 추가
#include <WTypesbase.h>
class CPerformanceTimer
{
private:
    LARGE_INTEGER m_frequency;
    LARGE_INTEGER m_startTime;
    LARGE_INTEGER m_endTime;

    float m_fTimeDelta;

public:
	CPerformanceTimer() : m_frequency(), m_startTime(), m_endTime(),m_fTimeDelta(0.f)
    {
        QueryPerformanceFrequency(&m_frequency);
    }

    void Start() 
    {
        QueryPerformanceCounter(&m_startTime);
    }

    float Get_DeltaTime() const { return m_fTimeDelta; }

    float End()
    {
        QueryPerformanceCounter(&m_endTime);
        m_fTimeDelta = (float)(m_endTime.QuadPart - m_startTime.QuadPart) / m_frequency.QuadPart * 1000.0;
        return (float)(m_endTime.QuadPart - m_startTime.QuadPart) / m_frequency.QuadPart * 1000.0; // ms 단위
    }
};