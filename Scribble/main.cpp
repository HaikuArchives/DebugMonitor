#include "ScribbleApp.h"
#include "ScribbleWindow.h"

int main(void)
{
	ScribbleApp app;
	
	(new ScribbleWindow)->Show();	
	
	app.Run();

	return 0;
}