#pragma once
/*
#include "Vi.h"
struct ProfileResult {

	const char* Name;
	float Time;
};

class ProfileResults {
public:
	static std::vector<ProfileResult>& Get() {	return m_ProfileResults; }

	static std::vector<ProfileResult> m_ProfileResults;

};
std::vector<ProfileResult> m_ProfileResults;


template<typename Fn>
class Timer {

public:
	Timer(const char* name, Fn&& func) : m_Name(name), m_Func(func), m_Stopped(false) {

		m_StartTimepoint = std::chrono::high_resolution_clock::now();

	}

	~Timer() {

		if (!m_Stopped)
			Stop();
	}

	void Stop() {

		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::milliseconds>(m_StartTimepoint).time_since_epoch().count();

		long long end = std::chrono::time_point_cast<std::chrono::milliseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;

		float duration = (end - start) / 1000.0f;
		m_Func({ m_Name, duration });
	}


private:
	const char* m_Name;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
	Fn m_Func;
};

#define PROFILE_SCOPE(name) Timer customTimer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })
*/