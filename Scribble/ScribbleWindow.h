#ifndef ScribbleWindow_h
#define ScribbleWindow_h

#include <Window.h>
#include "ScribbleView.h"

class ScribbleWindow : public BWindow
{
private:
	ScribbleView *v;
public:
	ScribbleWindow();
	virtual bool QuitRequested();
};


#endif