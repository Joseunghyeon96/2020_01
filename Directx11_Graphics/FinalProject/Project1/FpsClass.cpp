#include "FpsClass.h"

FpsClass::FpsClass()
{
}
FpsClass::FpsClass(const FpsClass& other)
{
}
FpsClass::~FpsClass()
{
}
//The Initialize function sets all the counters to zero and starts the timer.
void FpsClass::Initialize()
{
	m_fps = 0;
	m_count = 0;
	m_startTime = timeGetTime();
	return;
}

void FpsClass::Frame()
{
	m_count++;
	if (timeGetTime() >= (m_startTime + 1000))
	{
		m_fps = m_count;
		m_count = 0;
		m_startTime = timeGetTime();
	}
}

int FpsClass::GetFps()
{
	return m_fps;
}