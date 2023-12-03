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
#include "KeyCursor.h"


class SettingsWin;

class SettingsApp : public BApplication
{
public:
					SettingsApp();
					~SettingsApp();

	virtual void	ReadyToRun();
	virtual void 	MessageReceived(BMessage* message);

private:
	SettingsWin* 	fWindow;
};


SettingsApp::SettingsApp()
	:
	BApplication(APP_SIGNATURE)
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


void
SettingsApp::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case STATE:
			fWindow->PostMessage(message);
			break;

		default:
			BApplication::MessageReceived(message);
	}
}

int
main()
{
	SettingsApp app;
	app.Run();
	return 0;
}
