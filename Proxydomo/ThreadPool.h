﻿/**
*	@file	ThreadPool.h
*/

#pragma once

#ifdef _WIN64

#include <atomic>
#include <list>
#include <functional>
#include <atlbase.h>
#include <atlsync.h>

class CThreadPool
{
public:
	CThreadPool() 
	{
		::InitializeThreadpoolEnvironment(&m_CallBackEnviron);
		m_pool = ::CreateThreadpool(NULL);
		if (m_pool == NULL)
			throw std::runtime_error("CreateThreadpool failed");

#ifndef _DEBUG
		BOOL bRet = ::SetThreadpoolThreadMinimum(m_pool, 30);
		if (bRet == FALSE) {
			::CloseThreadpool(m_pool);
			::DestroyThreadpoolEnvironment(&m_CallBackEnviron);
			throw std::runtime_error("SetThreadpoolThreadMinimum failed");
		}
#endif

		::SetThreadpoolCallbackPool(&m_CallBackEnviron, m_pool);
	}

	~CThreadPool() {
		CloseAllThread();

		::CloseThreadpool(m_pool);
		::DestroyThreadpoolEnvironment(&m_CallBackEnviron);
	}

	void	CreateThread(std::function<void()> func)
	{
		CCritSecLock lock(m_csThreadDataList);
		m_threadDataList.emplace_front(&m_csThreadDataList, &m_threadDataList);
		auto itThis = m_threadDataList.begin();

		itThis->work = ::CreateThreadpoolWork(WorkCallback, (PVOID)&*itThis, &m_CallBackEnviron);
		if (itThis->work == NULL) {
			m_threadDataList.erase(itThis);
			throw std::runtime_error("CreateThreadpoolWork failed");
		}
		itThis->func = func;
		itThis->itThis = itThis;

		::SubmitThreadpoolWork(itThis->work);
	}

	void	CloseAllThread()
	{
		CCritSecLock lock(m_csThreadDataList);
		for (auto& threadData : m_threadDataList) {
			threadData.active = false;
		}
		lock.Unlock();

		for (auto& threadData : m_threadDataList) {
			::WaitForThreadpoolWorkCallbacks(threadData.work, FALSE);
			::CloseThreadpoolWork(threadData.work);
		}
		m_threadDataList.clear();
	}

private:
	static VOID CALLBACK WorkCallback(
		_Inout_      PTP_CALLBACK_INSTANCE Instance,
		_Inout_opt_  PVOID Context,
		_Inout_      PTP_WORK Work
		)
	{
		auto data = (ThreadData*)Context;
		try {
			data->func();
		}
		catch (...) {

		}
		CCritSecLock lock(*data->pcsList);
		if (data->active) {
			data->pthreadDataList->erase(data->itThis);
			lock.Unlock();
			::DisassociateCurrentThreadFromCallback(Instance);
			::CloseThreadpoolWork(Work);			
		}
	}

	TP_CALLBACK_ENVIRON	m_CallBackEnviron;
	PTP_POOL	m_pool;


	struct ThreadData {
		std::atomic_bool active;
		PTP_WORK	work;
		std::function<void()> func;
		std::list<ThreadData>::iterator itThis;
		CCriticalSection*	pcsList;
		std::list<ThreadData>*	pthreadDataList;

		ThreadData(CCriticalSection* pcs, std::list<ThreadData>* plist) : work(nullptr), pcsList(pcs), pthreadDataList(plist)
		{
			active = true;
		}
	};
	CCriticalSection	m_csThreadDataList;
	std::list<ThreadData>	m_threadDataList;
};


#endif
