// $Id$

/*
 * $Log$
 *
 */

#include "DebugClient.h"

#include <Application.h>
#include <OS.h>
#include <time.h>

const int32 ALI_DEBUG_MSG = 'aldb';
const char *ALI_DEBUG_MONITOR_NAME = "application/x-vnd.ali-DebugMonitor";

// static data member definitions
BMessenger DebugClient::server(ALI_DEBUG_MONITOR_NAME);
BString DebugClient::signature;

void DebugClient::GetSignature(void)
{
	if (signature == BString())
	{
		app_info ai;
		be_app->GetAppInfo(&ai); 
		signature = ai.signature;
	}	
}

void DebugClient::GetBasicInfo(BMessage *message)
{
	message->AddInt32("Time", time(NULL));
	
	thread_id callingthreadid = find_thread(NULL);
	message->AddInt32("Thread", callingthreadid);

	thread_info tinfo;
	get_thread_info(callingthreadid, &tinfo);
	message->AddString("Threadname", tinfo.name);
}

void DebugClient::handletrace(const char *message, 
        const char *group, 
        const uint level,
		const char *file,
		const uint line,
		const char *function)
{
	GetSignature();
		
	BMessage m(ALI_DEBUG_MSG);
	GetBasicInfo(&m);
		
	m.AddString("App", signature.String());	
	m.AddString("Message", message);
	m.AddString("Group", group);
	m.AddInt32("Level", level);
	m.AddString("File", file);
	m.AddInt32("Line", line);
	m.AddString("Function", function);

	// m.PrintToStream();
	server.SendMessage(&m);
}
	
		