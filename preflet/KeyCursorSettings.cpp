#include <Application.h>

#include "SettingsWin.h"

////////////////////////////////////////////////////////////////////////////////

class SettingsWin;

class SettingsApp : public BApplication
{
public:
		SettingsApp();
		~SettingsApp();
	virtual void ReadyToRun();
		
private:
	SettingsWin* win;
};

////////////////////////////////////////////////////////////////////////////////

SettingsApp::SettingsApp()
	: BApplication("application/x-vnd.OscarL-KeyCursorSettings")
{
}

SettingsApp::~SettingsApp()
{
}

void SettingsApp::ReadyToRun()
{
	win = new SettingsWin;
	win->Show();
}

////////////////////////////////////////////////////////////////////////////////

int main()
{
	(new SettingsApp)->Run();
	
	return 0;
}

////////////////////////////////////////////////////////////////////////////////