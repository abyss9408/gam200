/******************************************************************************/
/*!
\file		PerformanceViewer.cpp
\author		Team Popplio
\author		Val Tay Yun Ying (100%)
\co-author
\par		Course : CSD2400 / CSD2401
\par		Section : A
\date		2024/10/25
\brief
		This source file defines the PerformanceViewer class defintions

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>
#include "PerformanceViewer.h"

#include "../Engine/Engine.h"

#define POPPLIO_PERFORMANCE_REFRESH 2.0f // i dunno why its not in secs since i tested it

#define POPPLIO_PERFORMANCE_TIMEOUT 0.5 // secs

namespace Popplio
{
	const std::string PerformanceViewer::_MainEngine{ "engine" };
	PerformanceViewer* PerformanceViewer::_instance = nullptr;

	static float refreshRate = 0.f;

	// pointer to track where 

	PerformanceViewer* Popplio::PerformanceViewer::GetInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new PerformanceViewer();
		}

		return _instance;
	}

	void PerformanceViewer::DestroyInstance()
	{
		delete _instance;
	}

	void PerformanceViewer::start(std::string sysType)
	{
		bool isNew = false;
		if (!systemIntervalPair.contains(sysType))
			isNew = true;

		systemStartTimePair[sysType] = std::chrono::steady_clock::now();

		if (isNew)
		{
			while (systemIntervalPair[sysType].size() < maxPolls)
			{
				systemIntervalPair[sysType].push(std::chrono::duration<double, std::micro>{0});
				microSystemIntervalPair[sysType].push(0);
			}
		}
	}

	std::chrono::microseconds PerformanceViewer::end(std::string sysType)
	{
		// refresh rate to display / update the performance viewer
		if (sysType == _MainEngine) refreshRate += static_cast<float>(Engine::timer.GetSystemFixedDeltaTime());
		if (refreshRate >= POPPLIO_PERFORMANCE_REFRESH) refreshRate = 0.0;
		//if (refreshRate < POPPLIO_PERFORMANCE_REFRESH)
		//	return std::chrono::duration_cast<std::chrono::microseconds>(systemIntervalPair[sysType].front());
		//else refreshRate = 0.0;


		const std::chrono::duration<double> diff = std::chrono::steady_clock::now() - systemStartTimePair[sysType];

		// would this affect performance?
		while (systemIntervalPair[sysType].size() >= maxPolls)
		{
			// remove the front stored data
			systemIntervalPair[sysType].pop();
			microSystemIntervalPair[sysType].pop();
		}

		// update if refreshed
		if (refreshRate <= 0.0)
		{
			systemIntervalPair[sysType].push(diff);
			microSystemIntervalPair[sysType].push(static_cast<int>(std::chrono::duration_cast<std::chrono::microseconds>(diff).count()));
		}

		const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		std::vector<std::string> toDelete{};
		if (sysType == _MainEngine)
		{
			for (auto& it : systemIntervalPair)
			{
				if (std::chrono::duration<double>(now - systemStartTimePair[it.first]) >
					std::chrono::duration<double>(POPPLIO_PERFORMANCE_TIMEOUT))
				{
					toDelete.push_back(it.first);
				}
			}
		}
		for (auto& sys : toDelete)
		{
			systemStartTimePair.erase(sys);
			systemIntervalPair.erase(sys);
			microSystemIntervalPair.erase(sys);
		}
		toDelete.clear();

		return std::chrono::duration_cast<std::chrono::microseconds>(diff);
	}

	std::chrono::microseconds PerformanceViewer::GetSystemTime(std::string sysType)
	{
		if (systemIntervalPair[sysType].size() <= 0)
			return std::chrono::microseconds{};
		else
			return duration_cast<std::chrono::microseconds>(systemIntervalPair[sysType].back());
	}

	std::queue<int> const& PerformanceViewer::GetSystemTimeQueue(std::string sysName)
	{
		// TODO: insert return statement here
		return microSystemIntervalPair[sysName];
	}

	double PerformanceViewer::GetSystemTimePercentage(std::string sysName)
	{
		return GetSystemTime(_MainEngine).count() <= 0 ? 0.0 : ((double)(GetSystemTime(sysName).count()) / (double)(GetSystemTime(_MainEngine).count())) * 100.0;
	}

	size_t PerformanceViewer::GetNumSystemsRegistered()
	{
		return systemIntervalPair.size();
	}

	std::unordered_map<std::string, std::queue<int>> const& PerformanceViewer::GetAllSystemTimeMap()
	{
		return microSystemIntervalPair;
	}

	void PerformanceViewer::StartRealTime(const std::string& name)
	{
		timers[name] = std::chrono::system_clock::now();
	}

	std::chrono::duration<double> PerformanceViewer::EndRealTime(const std::string& name)
	{
		auto it = timers.find(name);
		if (it == timers.end()) {
			throw std::runtime_error("Timer '" + name + "' was not started");
		}

		auto end_time = std::chrono::system_clock::now();
		std::chrono::duration<double> duration = end_time - it->second;
		timers.erase(it);

		std::stringstream time;
		time << name << ": " << duration;

		Logger::Info(time.str());

		return duration;
	}


}