#include "ScribbleWindow.h"

#include <Menu.h>
#include <MenuItem.h>
#include <MenuBar.h>
#include <Application.h>

#include "../DebugClient/DebugClient.h"

ScribbleWindow::ScribbleWindow() 
		: BWindow(BRect(100,100,300,400), "Scribble", B_TITLED_WINDOW, 0)
{
	BMenuBar *pMenuBar = new BMenuBar(BRect(), "Mainmenu");
	BMenu *pMenu = new BMenu("File");
	BMenuItem *pCloseItem = 
		new BMenuItem("Close", new BMessage(B_QUIT_REQUESTED), 'q');	
	pMenu->AddItem(pCloseItem);
	pMenuBar->AddItem(pMenu);
	
	AddChild(pMenuBar);

	float menuHeight = pMenuBar->Bounds().Height();

	ScribbleView *aView;
	// set up a rectangle and instantiate a new view
	BRect aRect( Bounds() );
	aRect.top = aRect.top+menuHeight+1;
	aView = new ScribbleView(aRect);
	// add view to window
	AddChild(aView);
}

bool ScribbleWindow::QuitRequested()
{
	tracex("App",dbgMAJOR,"QuitRequested");
	
	be_app->PostMessage(B_QUIT_REQUESTED);
	return(true);
}
