#include "ScribbleView.h"

#include "../DebugClient/DebugClient.h"
#include <AppKit.h>
#include <SupportKit.h>

ScribbleView::ScribbleView(const BRect &aRect) 
		: BView(aRect, "ScribbleView", B_FOLLOW_ALL_SIDES, 0)
{
	drawing = false;
}

std::ostream& operator<<(std::ostream &os, const BPoint &p)
{
	os << "(" << p.x << "," << p.y << ")";
	return os;	
}

void ScribbleView::MouseDown(BPoint where)
{
	BView::MouseDown(where);
	tracex("UI", dbgMINOR, "commence drawing: " << where);
	
	from = where;
	drawing = true;
}

void ScribbleView::MouseUp(BPoint where)
{
	BView::MouseUp(where);
	tracex("UI", dbgVERBOSE, "end drawing: " << where);
		
	drawing = false;
}

void ScribbleView::MouseMoved(BPoint where,
					uint32 code,
					const BMessage *a_message)
{
	BView::MouseMoved(where, code, a_message);

	if (drawing)
	{
		StrokeLine(from, where);
		from = where;
	}	
}
					
void ScribbleView::MessageReceived(BMessage *message)
{
	BView::MessageReceived(message);
}
