/*
 * Copyright 2004-2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Oscar Lesta
 *		Humdinger
 */
#ifndef SETTINGSWIN_H
#define SETTINGSWIN_H

#include <Window.h>

#include "Preferences.h"


class BView;
class BSlider;
class BStringView;
class BCheckBox;
class ModifierView;
class ModifierBox;

class SettingsWin : public BWindow
{
public:
					SettingsWin();
					~SettingsWin();

	virtual void 	MessageReceived(BMessage* message);
	virtual bool 	QuitRequested();

private:
			void 	_SendMessageToFilter(int32 code);

	BStringView*	fStatusView;
	ModifierBox* 	fModBox;
	BCheckBox* 		fEnabled;
	BSlider* 		fAccelSlider;

	Preferences 	fPrefs;
};

#endif // SETTINGSWIN_H
