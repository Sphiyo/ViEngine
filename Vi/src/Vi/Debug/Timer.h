#pragma once
#include "Vi.h"

struct ProfileResult {

	const char* Name;
	float Time;
};

class ProfileResults {
public:
	static std::vector<ProfileResult>& Get() { return m_ProfileResults; }

	static std::vector<ProfileResult> m_ProfileResults;

};



template<typename Fn>
class Timer {

public:
	Timer(const char* name, Fn&& func);

	~Timer();

	void Stop();


private:
	const char* m_Name;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
	Fn m_Func;
};

#define PROFILE_SCOPE(name) Timer Customtimer##__LINE__(name, [&](ProfileResult profileResult) { ProfileResults::Get().push_back(profileResult); })


