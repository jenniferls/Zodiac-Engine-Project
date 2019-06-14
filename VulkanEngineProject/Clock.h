#pragma once

namespace Zodiac {
	class Clock {
	public:
		Clock();
		~Clock();

		void Tick();
		double GetDeltaTime();

	private:

	};
}