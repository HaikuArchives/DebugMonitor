/*
 * $Log: DebugMonitor.cpp,v $
 * Revision 1.3  1999/01/08 05:04:22  alister
 * Integration with RCS
 *
 */

#include "DebugMonitor.h"

#include "Tracer.h"

#include <assert.h>

DebugMonitorApp::DebugMonitorApp(void) : BApplication("application/x-vnd.ali-DebugMonitor")
{}

void DebugMonitorApp::MessageReceived(BMessage *message)
{
	if (message->what == 'aldb')
	{
		TracerWindow *w = NULL;
		
		BString client(message->FindString("App"));
		
		if (windows.find(client) == windows.end())
		{
			BRect aRect(700,80,900,380);
			w = new TracerWindow(aRect, client.String());
			
			// make window visible
			w->Show();
			
			windows[client] = w;
		}
		else
		{
			w = windows[client];
		}
		assert(w != NULL);
		w->PostMessage(message);
	}
	
	// Also perform default message processing
	BApplication::MessageReceived(message);
}