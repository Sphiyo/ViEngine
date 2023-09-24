#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Vi/Core/Log.h"	
#include "Vi/Debug/Instrumentor.h"	
#include "Vi/Debug/Timer.h"
#include "Vi/Core/Input.h"              // test 


#ifdef VI_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

/*
////////////////////////////				// temp 

#include <thread>
#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <future>

////////////////////////////				// temp 

// for srand() and rand()
#include <stdlib.h>
#include <time.h>
#include <chrono>

////////////////////////////				// temp 
*/