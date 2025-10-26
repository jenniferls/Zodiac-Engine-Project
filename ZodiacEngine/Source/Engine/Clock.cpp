#include <Zodiacpch.h>
#include "Clock.h"
#include <GLFW/glfw3.h>

Zodiac::Clock::Clock()
{
	m_deltaTime = 0.0f;
	m_elapsedTime = 0.0;
	m_lastTime = 0.0f;
}

Zodiac::Clock::~Clock()
{

}

void Zodiac::Clock::Tick()
{
	m_elapsedTime = glfwGetTime();
	m_deltaTime = static_cast<float>(m_elapsedTime - m_lastTime);
	m_lastTime = m_elapsedTime;
}

float Zodiac::Clock::GetDeltaTime() const
{
	return m_deltaTime;
}

double Zodiac::Clock::GetElapsedTime() const
{
	return m_elapsedTime;
}
