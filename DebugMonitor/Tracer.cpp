/*
 * $Log: Tracer.cpp,v $
 * Revision 1.3  1999/01/08 05:06:42  alister
 * Integration with RS
 *
 */

#include "Tracer.h"

#include <TextView.h>
#include <Application.h>
#include <strstream.h>
#include <String.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <ScrollView.h>
#include <ScrollBar.h>
#include <assert.h>

const unsigned MENU_ATTR_APP   = 'mAap';
const unsigned MENU_ATTR_FILE  = 'mAfl';
const unsigned MENU_ATTR_FUNC  = 'mAfn';
const unsigned MENU_ATTR_GROUP = 'mAgp';
const unsigned MENU_ATTR_LINE  = 'mAln';
const unsigned MENU_ATTR_LEVEL = 'mAlv';
const unsigned MENU_ATTR_MSG   = 'mAms';
const unsigned MENU_ATTR_TID   = 'mAti';
const unsigned MENU_ATTR_TIME  = 'mAtm';
const unsigned MENU_ATTR_TNAME = 'mAtn';
const unsigned MENU_CLEAR      = 'mClr';
	
TracerWindow::TracerWindow(const BRect &frame, const char* name) 
	: BWindow(frame, name, B_DOCUMENT_WINDOW, 0)
{
	BMenuBar *pMenuBar = new BMenuBar(BRect(), "Menu Bar");

	// Debug Menu
	BMenu* pDebugMenu = new BMenu("Debug");
	pDebugMenu->AddItem(new BMenuItem("Clear", new BMessage(MENU_CLEAR)));
	pDebugMenu->AddSeparatorItem();

	BMenuItem* pAboutItem = new BMenuItem("About...",
		new BMessage(B_ABOUT_REQUESTED));
	pAboutItem->SetTarget(NULL, be_app);
	pDebugMenu->AddItem(pAboutItem);

	pDebugMenu->AddItem(new BMenuItem("Close", new BMessage(B_QUIT_REQUESTED), 'q'));
	
	pMenuBar->AddItem(pDebugMenu);
	
	// Attributes menu
	pAttrMenu = new BMenu("Attributes");
	BMenuItem *item;
	pAttrMenu->AddItem(item = new BMenuItem("Function", new BMessage(MENU_ATTR_FUNC)));
	item->SetMarked(true);
	pAttrMenu->AddItem(item = new BMenuItem("Line number", new BMessage(MENU_ATTR_LINE)));
	pAttrMenu->AddItem(item = new BMenuItem("Message", new BMessage(MENU_ATTR_MSG)));
	item->SetMarked(true);
	pAttrMenu->AddItem(new BMenuItem("Source file", new BMessage(MENU_ATTR_FILE)));
	pAttrMenu->AddItem(new BMenuItem("Group", new BMessage(MENU_ATTR_GROUP)));
	pAttrMenu->AddItem(new BMenuItem("Level", new BMessage(MENU_ATTR_LEVEL)));
	pAttrMenu->AddItem(new BMenuItem("Timestamp", new BMessage(MENU_ATTR_TIME)));
	pAttrMenu->AddItem(new BMenuItem("Thread name", new BMessage(MENU_ATTR_TNAME)));
	pAttrMenu->AddItem(new BMenuItem("Thread id", new BMessage(MENU_ATTR_TID)));
	pAttrMenu->AddItem(new BMenuItem("Application", new BMessage(MENU_ATTR_APP)));
	pMenuBar->AddItem(pAttrMenu);
				
	// Groups menu
	pGroupsMenu = new BMenu("Groups");
	pMenuBar->AddItem(pGroupsMenu);
				
	// main menu bar		
	AddChild(pMenuBar);

	float menuHeight = pMenuBar->Bounds().Height();
	// set up a rectangle and instantiate a new view
	BRect r( Bounds() );
	r.Set(r.left, 
		r.top+menuHeight+1, 
		r.right-B_V_SCROLL_BAR_WIDTH, 
		r.bottom-B_H_SCROLL_BAR_HEIGHT);
	
	aView = new BListView(r, "tracer", B_SINGLE_SELECTION_LIST,
			B_FOLLOW_ALL_SIDES); 
   	AddChild(new BScrollView("scroll_tracer", aView, 
            B_FOLLOW_ALL_SIDES, 0, true, true));
}

bool TracerWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return(true);
}

const BString TracerWindow::FormatMessage(BMessage *message) const
{
	ostrstream os;
	
	if (pAttrMenu->FindItem("Timestamp")->IsMarked())
		os << "Time: " << message->FindInt32("Time") << "| ";
	if (pAttrMenu->FindItem("Application")->IsMarked())
		os << message->FindString("App") << " ";
	if (pAttrMenu->FindItem("Thread id")->IsMarked())
		os << "TID: " << message->FindInt32("Thread") << " ";
	if (pAttrMenu->FindItem("Thread name")->IsMarked())
		os << "Thread: " << message->FindString("Threadname") << " ";
	if (pAttrMenu->FindItem("Group")->IsMarked())
		os << message->FindString("Group") << " ";
	if (pAttrMenu->FindItem("Level")->IsMarked())
		os << message->FindInt32("Level") << "* ";
	if (pAttrMenu->FindItem("Source file")->IsMarked())
		os << message->FindString("File") << " ";
	if (pAttrMenu->FindItem("Function")->IsMarked())
		os << message->FindString("Function") << " ";
	if (pAttrMenu->FindItem("Line number")->IsMarked())
		os << "(" << message->FindInt32("Line") << ") ";
	if (pAttrMenu->FindItem("Message")->IsMarked())
		os << message->FindString("Message");
		
	os << ends;
			
	BString s(os.str());
	return s;
}

void TracerWindow::UpdateGroupMenu(const BMessage *message)
{
	BString group = message->FindString("Group");
	
	if (NULL == pGroupsMenu->FindItem(group.String()))
	{
		BMenuItem *item;
		BMenu *submenu = new BMenu(group.String());
		submenu->SetRadioMode(true);
		submenu->AddItem(new BMenuItem("None", NULL));
		submenu->AddItem(item = new BMenuItem("Major", NULL));
		item->SetMarked(true);
		submenu->AddItem(new BMenuItem("Minor", NULL));
		submenu->AddItem(new BMenuItem("Verbose", NULL));
		pGroupsMenu->AddItem(new BMenuItem(submenu)); 
	}
}

bool TracerWindow::PassesFilter(const BMessage *message) const
{
	assert(message->HasString("Group"));
	BString group = message->FindString("Group");
	
	BMenuItem *submenu = pGroupsMenu->FindItem(group.String());
	assert(submenu != NULL);
	
	BMenu *groupmenu = submenu->Submenu();
	assert(groupmenu != NULL);
	
	BMenuItem *item = groupmenu->FindMarked();
	assert(item != NULL);
	
	BString itemlabel = item->Label();
	assert(itemlabel != BString());
	
	int32 level = message->FindInt32("Level");
	assert(level >= 0 && level <= 15);
	 
	if (itemlabel == "None")
	{
		return level <= 0;
	} 
	if (itemlabel == "Major")
	{
		return level <= 5;
	}
	if (itemlabel == "Minor")
	{
		return level <= 10;
	}
	assert(itemlabel == "Verbose");
	return true;
}

void TracerWindow::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
	case 'aldb':
		UpdateGroupMenu(message);
		if (PassesFilter(message))
			aView->AddItem(new BStringItem(FormatMessage(message).String()));
		break;
	case MENU_ATTR_APP:
	case MENU_ATTR_FILE:
	case MENU_ATTR_FUNC:
	case MENU_ATTR_GROUP:
	case MENU_ATTR_LINE:
	case MENU_ATTR_LEVEL:
	case MENU_ATTR_MSG:
	case MENU_ATTR_TID:
	case MENU_ATTR_TIME:
	case MENU_ATTR_TNAME:
		{
			void *source = NULL;
			message->FindPointer("source", &source);
			BMenuItem *pItem = reinterpret_cast<BMenuItem*>(source);
			assert(pItem != NULL);	
			pItem->SetMarked(!pItem->IsMarked());
		}	
		break;
	case MENU_CLEAR:
		while (!aView->IsEmpty()) 
       		delete aView->RemoveItem((int32)0);
		break;
	default:
		BWindow::MessageReceived(message);
		break;
	}
}

