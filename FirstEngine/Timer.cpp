#include "Timer.h"
#include "PlatformDefinitions.h"
#include <algorithm>

Timer::Timer()
	: m_frequency(0)
	, m_time(0)
	, m_deltaTime(0)
	, m_lastTicks(0)
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	m_frequency = frequency.QuadPart;
}

Timer::~Timer() = default;

void Timer::Update()
{
	LARGE_INTEGER ticks;
	QueryPerformanceCounter(&ticks);

	if (m_lastTicks == 0)
		m_lastTicks = ticks.QuadPart;
	else
	{
		m_deltaTime = ticks.QuadPart - m_lastTicks;
		const int64_t maxDeltaTimeTicks = m_frequency * s_maxDeltaTimeMs / 1000;
		m_deltaTime = std::min(m_deltaTime, maxDeltaTimeTicks);
		m_time += m_deltaTime;
		m_lastTicks = ticks.QuadPart;
	}
}
