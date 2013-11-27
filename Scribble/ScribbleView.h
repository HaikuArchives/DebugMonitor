#ifndef ScribbleView_h
#define ScribbleView_h

#include <View.h>

class ScribbleView : public BView
{
	BPoint from;
	bool drawing;
public:
	ScribbleView(const BRect &aRect);
	
	virtual void MessageReceived(BMessage *message);
	virtual	void MouseDown(BPoint where);
	virtual	void MouseUp(BPoint where);
	virtual	void MouseMoved(BPoint where,
					uint32 code,
					const BMessage *a_message);
};


#endif