#pragma once
#include <chrono>

namespace Zodiac {
	class Clock {
	public:
		Clock();
		~Clock();

		void Tick();
		float GetDeltaTime() const;
		double GetElapsedTime() const;

	private:
		float m_deltaTime;
		double m_elapsedTime;
		double m_lastTime;
	};
}