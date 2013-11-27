#ifndef ScribbleApp_h
#define ScribbleApp_h

#include <Application.h>

class ScribbleApp : public BApplication
{
public:
	ScribbleApp() : BApplication("application/x-vnd.ali-Scribble")
	{}	
};

#endif