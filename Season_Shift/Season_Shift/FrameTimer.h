#pragma once

#include <chrono>

class FrameTimer
{
private:
	class InternalTimer
	{
		friend FrameTimer;
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_start, m_end;
		std::chrono::microseconds m_duration;

		static InternalTimer s_timer;

	private:
		InternalTimer();
		~InternalTimer() = default;

		InternalTimer(const InternalTimer&) = delete;
		InternalTimer& operator=(const InternalTimer&) = delete;

		void start();
		void stop();
	};



public:
	FrameTimer() = default;
	~FrameTimer() = default;

	enum class Duration
	{
		SECONDS,
		MILLISECONDS,
		MICROSECONDS
	};


	static void frameStart();
	static void frameStop();

	// Print duration in milliseconds and seconds
	static void print(std::wstring headerMsg);

	// Get duration: MILLISECONDS, SECONDS
	static long double getTime(Duration duration);

	// Shorthand for get duration seconds
	static long double dt();
};

