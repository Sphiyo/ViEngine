#include "Vipch.h"
#include "Timer.h"




std::vector<ProfileResult> ProfileResults::m_ProfileResults;



template<typename Fn>
Timer<Fn>::Timer(const char* name, Fn&& func) : m_Name(name), m_Func(func), m_Stopped(false) {

	m_StartTimepoint = std::chrono::high_resolution_clock::now();
}

template<typename Fn>
Timer<Fn>::~Timer() {

	if (!m_Stopped)
		Stop();
}

template<typename Fn>
void Timer<Fn>::Stop() {

	auto endTimepoint = std::chrono::high_resolution_clock::now();

	long long start = std::chrono::time_point_cast<std::chrono::milliseconds>(m_StartTimepoint).time_since_epoch().count();

	long long end = std::chrono::time_point_cast<std::chrono::milliseconds>(endTimepoint).time_since_epoch().count();

	m_Stopped = true;

	float duration = (end - start) / 1000.0f;
	m_Func({ m_Name, duration });

}

