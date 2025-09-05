/******************************************************************************/
/*!
\file		PerformanceViewer.h
\author		Team Popplio
\author		Val Tay Yun Ying (100%)
\co-author
\par		Course : CSD2400 / CSD2401
\par		Section : A
\date		2024/10/21
\brief
		This source file defines the PerformanceViewer class

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#ifndef PERFORMANCE_VIEWER_HEADER
#define PERFORMANCE_VIEWER_HEADER
#include "../Logging/Logger.h"

#include <chrono>
#include <unordered_map>
#include <string>
#include <typeindex>
#include <queue>

namespace Popplio
{
	class PerformanceViewer
	{
	private:

		static PerformanceViewer* _instance;

		// maximum number of polls the system can keep before deleting the oldest recorded poll
		size_t maxPolls;

		// map to store the start time of recording of each system
		std::unordered_map<std::string, std::chrono::steady_clock::time_point> systemStartTimePair;

		// map to store duration of each system's run time
		std::unordered_map<std::string, std::queue<std::chrono::duration<double>>> systemIntervalPair;

		// map to store system time in microseconds
		std::unordered_map<std::string, std::queue<int>> microSystemIntervalPair;

		std::unordered_map<std::string, std::chrono::system_clock::time_point> timers;

	public:
		static const std::string _MainEngine;

		// TODO: change maxPolls to be modifiable outside of this class, to change max num of polls per cycle
		PerformanceViewer() : maxPolls(100) {};
		~PerformanceViewer() { };

		// gets an instance of PerformanceViewer, creates a new instance if not yet existing
		static PerformanceViewer* GetInstance();

		static void DestroyInstance();

		// starts the timer for type sysType
		void start(std::string sysType);

		// ends the timer for type sysType & returns how long this system took in microseconds (ms)
		std::chrono::microseconds end(std::string sysType);

		// returns the most recent poll in microseconds (ms)
		std::chrono::microseconds GetSystemTime(std::string sysType);

		std::queue<int> const& GetSystemTimeQueue(std::string sysName);

		double GetSystemTimePercentage(std::string sysName);

		size_t GetNumSystemsRegistered();

		std::unordered_map<std::string, std::queue<int>> const& GetAllSystemTimeMap();

		void StartRealTime(const std::string& name);

		std::chrono::duration<double> EndRealTime(const std::string& name);
	};

}
#endif
