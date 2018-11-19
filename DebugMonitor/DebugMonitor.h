/*
 * $Log: DebugMonitor.h,v $
 * Revision 1.3  1999/01/08 05:04:22  alister
 * Integration with RCS
 *
 */

#ifndef DebugMonitor_h
#define DebugMonitor_h

#include "Tracer.h"
#include <Application.h>
#include <SupportKit.h>
#include <InterfaceKit.h>
#include <map>
#include <String.h>


typedef std::map<BString, TracerWindow *> StringWindowMap; 

class DebugMonitorApp : public BApplication
{
	StringWindowMap windows;
public: 
	DebugMonitorApp(void);
	virtual void MessageReceived(BMessage *message);
};

#endif
