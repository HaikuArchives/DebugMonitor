/*
 * $Log: Tracer.h,v $
 * Revision 1.3  1999/01/08 05:06:42  alister
 * Integration with RS
 *
 */

#ifndef Tracer_h
#define Tracer_h

#include <Window.h>
#include <ListView.h>
#include <Menu.h>

class TracerWindow : public BWindow
{
	BListView *aView;
	BMenu *pAttrMenu;
	BMenu *pGroupsMenu;
	
	const BString FormatMessage(BMessage *message) const;
	void UpdateGroupMenu(const BMessage *message);
	bool PassesFilter(const BMessage *message) const;
public:
	TracerWindow(const BRect &frame, const char* name);	
	virtual bool QuitRequested();
	virtual void MessageReceived(BMessage *message);
};

#endif