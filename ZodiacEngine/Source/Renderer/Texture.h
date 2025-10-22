#pragma once

namespace Zodiac {
	class Texture {
	public:
		virtual ~Texture() = default;

		//virtual uint32_t GetWidth() const = 0;
		//virtual uint32_t GetHeight() const = 0;

		virtual void Bind() = 0;

	private:

	};
}