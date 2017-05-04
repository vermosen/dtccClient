#ifndef APPLICATION_SERVICE_WINSERVICE_HPP_
#define APPLICATION_SERVICE_WINSERVICE_HPP_

#ifdef _WIN32
#include <sstream>
#include <windows.h>

namespace dtcc
{
	class service
	{
	public:

		// Register the executable for a service with the Service Control Manager 
		// (SCM). After you call Run(ServiceBase), the SCM issues a Start command, 
		// which results in a call to the OnStart method in the service. This 
		// method blocks until the service has stopped.
		static BOOL run(service &);

		// Service object constructor.
		service(LPSTR	name,
			BOOL	canStop = true,
			BOOL 	canShutdown = false,
			BOOL	canPauseContinue = false);

		service() = delete;
		virtual ~service(void) {}

		// Stop the service.
		void stop();

	protected:
		// interfaces
		virtual void onStart(DWORD dwArgc, LPSTR *pszArgv) = 0;
		virtual void onStop() = 0;
		virtual void onPause() = 0;
		virtual void onContinue() = 0;
		virtual void onShutdown() = 0;

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

		static service * instance_;						// the service instance
	};
}

#endif
#endif
