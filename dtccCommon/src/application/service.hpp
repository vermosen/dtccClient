#ifndef APPLICATION_SERVICE_WINSERVICE_HPP_
#define APPLICATION_SERVICE_WINSERVICE_HPP_

#ifdef _WIN32

#include "pattern/singleton.hpp"
#include <Windows.h>

namespace dtcc
{
	template<typename Child>
	class service : public singleton<service<Child>>
	{
	public:

		// Register the executable for a service with the Service Control Manager 
		// (SCM). After you call Run(ServiceBase), the SCM issues a Start command, 
		// which results in a call to the OnStart method in the service. This 
		// method blocks until the service has stopped.
		static BOOL run(service &);

		// Service object constructor. The optional parameters (fCanStop, 
		// fCanShutdown and fCanPauseContinue) allow you to specify whether the 
		// service can be stopped, paused and continued, or be notified when 
		// system shutdown occurs.
		service();

		virtual ~service(void) {}

		// Stop the service.
		void stop();

	protected:
		void onStart(DWORD dwArgc, LPSTR *pszArgv) { static_cast<Child*>(this)->onStartImpl(dwArgc, pszArgv); };
		void onStop() 
		{ 
			static_cast<Child*>(this)->onStopImpl();
		};
		void onPause() 
		{ 
			static_cast<Child*>(this)->onPauseImpl();
		};
		void onContinue() 
		{ 
			static_cast<Child*>(this)->onContinueImpl();
		};
		void onShutdown() 
		{ 
			static_cast<Child*>(this)->onShutdownImpl();
		};

		// Set the service status and report the status to the SCM.
		void setServiceStatus(DWORD dwCurrentState,
			DWORD dwWin32ExitCode = NO_ERROR,
			DWORD dwWaitHint = 0);

		// Log a message to the Application event log.
		void writeEventLogEntry(LPSTR pszMessage, WORD wType);

		// Log an error message to the Application event log.
		void writeErrorLogEntry(LPSTR pszFunction,
			DWORD dwError = GetLastError());

	private:
		static void WINAPI ServiceMain(DWORD dwArgc, LPSTR *lpszArgv);
		static void WINAPI ServiceCtrlHandler(DWORD dwCtrl);
		void start(DWORD dwArgc, LPSTR *pszArgv);
		void pause();
		void _continue();
		void shutdown();

		LPSTR name_;									// The name of the service
		SERVICE_STATUS status_;							// The status of the service
		SERVICE_STATUS_HANDLE statusHandle_;			// The service status handle
	};

#pragma region Static Members

	template<typename Child>
	BOOL service<Child>::run(service &service)
	{
		SERVICE_TABLE_ENTRY serviceTable[] =
		{
			{ instance().name_, ServiceMain },
			{ NULL, NULL }
		};

		// Connects the main thread of a service process to the service control 
		// manager, which causes the thread to be the service control dispatcher 
		// thread for the calling process. This call returns when the service has 
		// stopped. The process should simply terminate when the call returns.
		return StartServiceCtrlDispatcher(serviceTable);
	}

	//
	//   FUNCTION: service::ServiceMain(DWORD, PWSTR *)
	//
	//   PURPOSE: Entry point for the service. It registers the handler function 
	//   for the service and starts the service.
	//
	//   PARAMETERS:
	//   * dwArgc   - number of command line arguments
	//   * lpszArgv - array of command line arguments
	//
	template<typename Child>
	void WINAPI service<Child>::ServiceMain(DWORD dwArgc, LPSTR *pszArgv)
	{
		// Register the handler function for the service
		instance().statusHandle_ = RegisterServiceCtrlHandler(
			instance().name_, ServiceCtrlHandler);
		if (instance().statusHandle_ == NULL)
		{
			throw GetLastError();
		}

		// Start the service.
		instance().start(dwArgc, pszArgv);
	}

	//
	//   FUNCTION: service::ServiceCtrlHandler(DWORD)
	//
	//   PURPOSE: The function is called by the SCM whenever a control code is 
	//   sent to the service. 
	//
	//   PARAMETERS:
	//   * dwCtrlCode - the control code. This parameter can be one of the 
	//   following values: 
	//
	//     SERVICE_CONTROL_CONTINUE
	//     SERVICE_CONTROL_INTERROGATE
	//     SERVICE_CONTROL_NETBINDADD
	//     SERVICE_CONTROL_NETBINDDISABLE
	//     SERVICE_CONTROL_NETBINDREMOVE
	//     SERVICE_CONTROL_PARAMCHANGE
	//     SERVICE_CONTROL_PAUSE
	//     SERVICE_CONTROL_SHUTDOWN
	//     SERVICE_CONTROL_STOP
	//
	//   This parameter can also be a user-defined control code ranges from 128 
	//   to 255.
	//
	template<typename Child>
	void WINAPI service<Child>::ServiceCtrlHandler(DWORD dwCtrl)
	{
		switch (dwCtrl)
		{
		case SERVICE_CONTROL_STOP: instance().stop(); break;
		case SERVICE_CONTROL_PAUSE: instance().pause(); break;
		case SERVICE_CONTROL_CONTINUE: instance()._continue(); break;
		case SERVICE_CONTROL_SHUTDOWN: instance().shutdown(); break;
		case SERVICE_CONTROL_INTERROGATE: break;
		default: break;
		}
	}

#pragma endregion

#pragma region Service Constructor and Destructor
	//
	//   FUNCTION: service::service(PWSTR, BOOL, BOOL, BOOL)
	//
	//   PURPOSE: The constructor of service. It initializes a new instance 
	//   of the service class. The optional parameters (fCanStop, 
	///  fCanShutdown and fCanPauseContinue) allow you to specify whether the 
	//   service can be stopped, paused and continued, or be notified when system 
	//   shutdown occurs.
	//
	//   PARAMETERS:
	//   * pszServiceName - the name of the service
	//   * fCanStop - the service can be stopped
	//   * fCanShutdown - the service is notified when system shutdown occurs
	//   * fCanPauseContinue - the service can be paused and continued
	//
	template<typename Child>
	service<Child>::service()
	{
		// Service name must be a valid string and cannot be NULL.
		name_ = (Child::name() == "") ? "" : Child::name().c_str();

		statusHandle_ = NULL;

		// The service runs in its own process.
		status_.dwServiceType = SERVICE_WIN32_OWN_PROCESS;

		// The service is starting.
		status_.dwCurrentState = SERVICE_START_PENDING;

		// The accepted commands of the service.
		DWORD dwControlsAccepted = 0;
		if (static_cast<Child*>(this)->canStop())
			dwControlsAccepted |= SERVICE_ACCEPT_STOP;
		if (static_cast<Child*>(this)->canShutdown())
			dwControlsAccepted |= SERVICE_ACCEPT_SHUTDOWN;
		if (static_cast<Child*>(this)->canPauseContinue())
			dwControlsAccepted |= SERVICE_ACCEPT_PAUSE_CONTINUE;
		status_.dwControlsAccepted = dwControlsAccepted;

		status_.dwWin32ExitCode = NO_ERROR;
		status_.dwServiceSpecificExitCode = 0;
		status_.dwCheckPoint = 0;
		status_.dwWaitHint = 0;
	}

#pragma endregion

#pragma region Service Start, Stop, Pause, Continue, and Shutdown

	//
	//   FUNCTION: service::Start(DWORD, PWSTR *)
	//
	//   PURPOSE: The function starts the service. It calls the OnStart virtual 
	//   function in which you can specify the actions to take when the service 
	//   starts. If an error occurs during the startup, the error will be logged 
	//   in the Application event log, and the service will be stopped.
	//
	//   PARAMETERS:
	//   * dwArgc   - number of command line arguments
	//   * lpszArgv - array of command line arguments
	//
	template<typename Child>
	void service<Child>::start(DWORD dwArgc, LPSTR *pszArgv)
	{
		try
		{
			// Tell SCM that the service is starting.
			setServiceStatus(SERVICE_START_PENDING);

			// Perform service-specific initialization.
			onStart(dwArgc, pszArgv);

			// Tell SCM that the service is started.
			setServiceStatus(SERVICE_RUNNING);
		}
		catch (DWORD & dwError)
		{
			// Log the error.
			writeErrorLogEntry("Service Start", dwError);

			// Set the service status to be stopped.
			setServiceStatus(SERVICE_STOPPED, dwError);
		}
		catch (...)
		{
			// Log the error.
			writeEventLogEntry("Service failed to start.", EVENTLOG_ERROR_TYPE);

			// Set the service status to be stopped.
			setServiceStatus(SERVICE_STOPPED);
		}
	}

	template<typename Child>
	void service<Child>::stop()
	{
		DWORD dwOriginalState = status_.dwCurrentState;
		try
		{
			// Tell SCM that the service is stopping.
			setServiceStatus(SERVICE_STOP_PENDING);

			// Perform service-specific stop operations.
			onStop();

			// Tell SCM that the service is stopped.
			setServiceStatus(SERVICE_STOPPED);
		}
		catch (DWORD dwError)
		{
			// Log the error.
			writeErrorLogEntry("Service Stop", dwError);

			// Set the orginal service status.
			setServiceStatus(dwOriginalState);
		}
		catch (...)
		{
			// Log the error.
			writeEventLogEntry("Service failed to stop.", EVENTLOG_ERROR_TYPE);

			// Set the orginal service status.
			setServiceStatus(dwOriginalState);
		}
	}

	template<typename Child>
	void service<Child>::pause()
	{
		try
		{
			// Tell SCM that the service is pausing.
			setServiceStatus(SERVICE_PAUSE_PENDING);

			// Perform service-specific pause operations.
			onPause();

			// Tell SCM that the service is paused.
			setServiceStatus(SERVICE_PAUSED);
		}
		catch (DWORD dwError)
		{
			// Log the error.
			writeErrorLogEntry("Service Pause", dwError);

			// Tell SCM that the service is still running.
			setServiceStatus(SERVICE_RUNNING);
		}
		catch (...)
		{
			// Log the error.
			writeEventLogEntry("Service failed to pause.", EVENTLOG_ERROR_TYPE);

			// Tell SCM that the service is still running.
			setServiceStatus(SERVICE_RUNNING);
		}
	}

	template<typename Child>
	void service<Child>::_continue()
	{
		try
		{
			// Tell SCM that the service is resuming.
			setServiceStatus(SERVICE_CONTINUE_PENDING);

			// Perform service-specific continue operations.
			onContinue();

			// Tell SCM that the service is running.
			setServiceStatus(SERVICE_RUNNING);
		}
		catch (DWORD dwError)
		{
			// Log the error.
			writeErrorLogEntry("Service Continue", dwError);

			// Tell SCM that the service is still paused.
			setServiceStatus(SERVICE_PAUSED);
		}
		catch (...)
		{
			// Log the error.
			writeEventLogEntry("Service failed to resume.", EVENTLOG_ERROR_TYPE);

			// Tell SCM that the service is still paused.
			setServiceStatus(SERVICE_PAUSED);
		}
	}

	template<typename Child>
	void service<Child>::shutdown()
	{
		try
		{
			// Perform service-specific shutdown operations.
			onShutdown();

			// Tell SCM that the service is stopped.
			setServiceStatus(SERVICE_STOPPED);
		}
		catch (DWORD dwError)
		{
			// Log the error.
			writeErrorLogEntry("Service Shutdown", dwError);
		}
		catch (...)
		{
			// Log the error.
			writeEventLogEntry("Service failed to shut down.", EVENTLOG_ERROR_TYPE);
		}
	}

#pragma region Helper Functions

	//
	//   FUNCTION: service::setServiceStatus(DWORD, DWORD, DWORD)
	//
	//   PURPOSE: The function sets the service status and reports the status to 
	//   the SCM.
	//
	//   PARAMETERS:
	//   * dwCurrentState - the state of the service
	//   * dwWin32ExitCode - error code to report
	//   * dwWaitHint - estimated time for pending operation, in milliseconds
	//
	template<typename Child>
	void service<Child>::setServiceStatus(DWORD dwCurrentState,
		DWORD dwWin32ExitCode,
		DWORD dwWaitHint)
	{
		static DWORD dwCheckPoint = 1;

		// Fill in the SERVICE_STATUS structure of the service.
		status_.dwCurrentState = dwCurrentState;
		status_.dwWin32ExitCode = dwWin32ExitCode;
		status_.dwWaitHint = dwWaitHint;

		status_.dwCheckPoint =
			((dwCurrentState == SERVICE_RUNNING) ||
			(dwCurrentState == SERVICE_STOPPED)) ?
			0 : dwCheckPoint++;

		// Report the status of the service to the SCM.
		::SetServiceStatus(statusHandle_, &status_);
	}


	//
	//   FUNCTION: service::WriteEventLogEntry(PWSTR, WORD)
	//
	//   PURPOSE: Log a message to the Application event log.
	//
	//   PARAMETERS:
	//   * pszMessage - string message to be logged.
	//   * wType - the type of event to be logged. The parameter can be one of 
	//     the following values.
	//
	//     EVENTLOG_SUCCESS
	//     EVENTLOG_AUDIT_FAILURE
	//     EVENTLOG_AUDIT_SUCCESS
	//     EVENTLOG_ERROR_TYPE
	//     EVENTLOG_INFORMATION_TYPE
	//     EVENTLOG_WARNING_TYPE
	//
	template<typename Child>
	void service<Child>::writeEventLogEntry(LPSTR pszMessage, WORD wType)
	{
		HANDLE hEventSource = NULL;
		LPCSTR lpszStrings[2] = { NULL, NULL };

		hEventSource = RegisterEventSource(NULL, name_);
		if (hEventSource)
		{
			lpszStrings[0] = name_;
			lpszStrings[1] = pszMessage;

			ReportEvent(hEventSource,  // Event log handle
				wType,                 // Event type
				0,                     // Event category
				0,                     // Event identifier
				NULL,                  // No security identifier
				2,                     // Size of lpszStrings array
				0,                     // No binary data
				lpszStrings,           // Array of strings
				NULL                   // No binary data
			);

			DeregisterEventSource(hEventSource);
		}
	}

	//
	//   FUNCTION: service::WriteErrorLogEntry(PWSTR, DWORD)
	//
	//   PURPOSE: Log an error message to the Application event log.
	//
	//   PARAMETERS:
	//   * pszFunction - the function that gives the error
	//   * dwError - the error code
	//
	template<typename Child>
	void service<Child>::writeErrorLogEntry(LPSTR pszFunction, DWORD dwError)
	{
		//char szMessage[260];
		//StringCchPrintf(szMessage, ARRAYSIZE(szMessage),
		//	"%s failed w/err 0x%08lx", pszFunction, dwError);
		//writeEventLogEntry(szMessage, EVENTLOG_ERROR_TYPE);
	}
#pragma endregion
}

#endif
#endif
