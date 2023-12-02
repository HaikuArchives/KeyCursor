/*
 * Copyright 2004-2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Oscar Lesta
 *		Humdinger
 */


#include <Application.h>

#include "SettingsWin.h"


class SettingsWin;

class SettingsApp : public BApplication
{
public:
					SettingsApp();
					~SettingsApp();

	virtual void	ReadyToRun();

private:
	SettingsWin* 	fWindow;
};


SettingsApp::SettingsApp()
	:
	BApplication("application/x-vnd.OscarL-KeyCursorSettings")
{
}


SettingsApp::~SettingsApp()
{
}


void
SettingsApp::ReadyToRun()
{
	fWindow = new SettingsWin();
	fWindow->Show();
}


int
main()
{
	SettingsApp app;
	app.Run();
	return 0;
}
