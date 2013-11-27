#ifndef debug_client_h
#define debug_client_h

// $Id: DebugClient.h,v 1.3 1999/01/08 00:08:36 alister Exp alister $

/*
 * $Log: DebugClient.h,v $
 * Revision 1.3  1999/01/08 00:08:36  alister
 * Set up RCS integration
 *
 * Revision 1.2
 * Removed assert (better handled by Be)
 * Added warn
 * Added support for stream messages
 * Added support for groups and levels
 * Modified approach to static members
 * 
 * Revision 1.1  1999/01/07
 * First release to Beware
 *
 */

// #include <stdlib.h>
#include <Messenger.h>
#include <String.h>
#include <strstream.h>

class DebugClient 
{
	// BMessager to our DebugMonitor server application
	static BMessenger server;	// does this need to be guarded?
	
	// Holds the client application signature so we don't have to 
	// get it every time
	static BString signature;

	// Gets the app signature unless already available
	static void GetSignature(void);
	// Stuffs basic information into message
	static void GetBasicInfo(BMessage *message);

public:	
	static void handletrace(const char *message, 
			const char* group, const uint level,
			const char *file,
			const unsigned int line,
			const char *function);
};


#ifndef NDEBUG

#define trace(msg)                                          \
  {                                                         \
     ostrstream _alidbgout;                                 \
     _alidbgout << msg << ends;                             \
     DebugClient::handletrace(_alidbgout.str(), "Def", 1,   \
            __FILE__, __LINE__, __PRETTY_FUNCTION__);       \
  }

#define warn(expr,msg)                                      \
  if(expr)                                                  \
  {                                                         \
     ostrstream _alidbgout;                                 \
     _alidbgout << msg << ends;                             \
     DebugClient::handletrace(_alidbgout.str(), "Def", 1,   \
            __FILE__, __LINE__, __PRETTY_FUNCTION__);       \
  }

#define tracex(grp,lvl,msg)                                 \
  {                                                         \
     ostrstream _alidbgout;                                 \
     _alidbgout << msg << ends;                             \
     DebugClient::handletrace(_alidbgout.str(), grp, lvl,   \
            __FILE__, __LINE__, __PRETTY_FUNCTION__);       \
  }

#define warnx(expr,grp,lvl,msg)                             \
  if(expr)                                                  \
  {                                                         \
     ostrstream _alidbgout;                                 \
     _alidbgout << msg << ends;                             \
     DebugClient::handletrace(_alidbgout.str(), grp, lvl,   \
            __FILE__, __LINE__, __PRETTY_FUNCTION__);       \
  }

// By putting these here, I'm hinting at some kind of standard.
// Imagine how a standard for groups would make it easier to
// understand a program through its debug output.
enum { dbgFORCE = 0, dbgMAJOR = 5, dbgMINOR = 10, dbgVERBOSE = 15 };
 
#else

#define trace(msg)                ((void)0)  
#define warn(expr,msg)            ((void)0)
#define tracex(grp,lvl,msg)       ((void)0)
#define warnx(expr,grp,lvl,msg)   ((void)0)   

#endif


#endif