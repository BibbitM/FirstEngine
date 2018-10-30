#pragma once

#include <stdint.h>

class Timer
{
public:
	Timer();
	~Timer();

	void Update();

	float GetTimeSeconds() const { return static_cast<float>(m_time) / static_cast<float>(m_frequency); }
	float GetDeltaTimeSeconds() const { return static_cast<float>(m_deltaTime) / static_cast<float>(m_frequency); }

	int64_t GetTimeTicks() const { return m_time; }
	int64_t GetDeltaTimeTicks() const { return m_deltaTime; }

private:
	int64_t m_frequency;
	int64_t m_time;
	int64_t m_deltaTime;
	int64_t m_lastTicks;

	const static int64_t s_maxDeltaTimeMs = 100;
};

