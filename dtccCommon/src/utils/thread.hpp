#pragma once
#ifndef UTILS_THREAD_HPP_
#define UTILS_THREAD_HPP_

#include <boost/thread.hpp>

#include <Windows.h>

const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push, 8)
typedef struct THREADNAME_INFO
{
	DWORD dwType; // Must be 0x1000.
	LPCSTR szName; // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

inline void _setThreadName(DWORD threadId, const char* threadName)
{
	THREADNAME_INFO info;

	info.dwType = 0x1000;
	info.szName = threadName;
	info.dwThreadID = threadId;
	info.dwFlags = 0;

	__try
	{
		RaiseException(
			MS_VC_EXCEPTION, 0,
			sizeof(info) / sizeof(ULONG_PTR),
			(ULONG_PTR*)&info);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {}
}
inline void setThreadName(boost::thread::id threadId, std::string name)
{
	// convert string to char*
	const char* cchar = name.c_str();
	// convert HEX string to DWORD
	unsigned int dwThreadId;
	std::stringstream ss;
	ss << std::hex << threadId;
	ss >> dwThreadId;
	// set thread name
	_setThreadName((DWORD)dwThreadId, cchar);
}

#endif