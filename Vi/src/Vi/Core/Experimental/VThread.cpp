#include "Vipch.h"
#include "VThread.h"


namespace Vi {



	VThread::VThread(std::string threadName) {

		m_ThreadName = threadName;
	}

	void VThread::AddFunction(std::function<void()> function) {

		m_Functions.push(function);
	}

	void VThread::Run()
	{
		m_DoneRunning = m_Functions.empty();
		m_ShutdownState = false;

		if (m_Thread.joinable())
			return;

		m_Thread = std::thread([&] {

			while (!m_ShutdownState) {

				m_DoneRunning = m_Functions.empty();

				if (!m_DoneRunning) {
					m_Functions.front()();
					m_Functions.pop();
				}
			}

			});
	}

	void VThread::Shutdown()
	{

		m_ShutdownState = true;
		Join();
	}

	void VThread::ShutdownAfterFinish()
	{
		while (true) {
			if (m_DoneRunning)
				break;
		}
		m_ShutdownState = true;
		Join();
	}

	void VThread::Join()
	{
		if (m_Thread.joinable())
			m_Thread.join();
	}

	void VThread::Detach()
	{
		if (m_Thread.joinable())
			m_Thread.detach();
	}



}