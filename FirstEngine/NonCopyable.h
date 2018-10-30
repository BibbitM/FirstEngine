#pragma once

namespace NonCopyablePriv  // protection from unintended ADL
{
	class NonCopyable
	{
	protected:
		constexpr NonCopyable() = default;
		~NonCopyable() = default;

		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator=(const NonCopyable&) = delete;
	};
}

typedef NonCopyablePriv::NonCopyable NonCopyable;
