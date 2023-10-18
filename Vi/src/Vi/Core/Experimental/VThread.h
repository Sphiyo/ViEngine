#pragma once
#include <thread>
#include <queue>
#include<functional>


namespace Vi {

	class VThread
	{
	public:

		VThread(std::string threadName = "Custom");


		void AddFunction(std::function<void()> function);

		void Run();

		void Shutdown();

		void ShutdownAfterFinish();

		void Join();

		void Detach();

		std::string GetName() const { return m_ThreadName; }

		std::thread::id GetID() const { return m_Thread.get_id(); }

		bool IsIdle() const { return m_DoneRunning; }

	private:

		std::string m_ThreadName = "Custom";
		std::queue<std::function<void()>> m_Functions;
		std::thread m_Thread;
		std::thread::id Id;
		bool m_DoneRunning = false;
		bool m_ShutdownState = true;
	};

}