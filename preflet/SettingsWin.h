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

#define ABOUT	'abut'

class BCheckBox;
class BButton;
class BView;
class BSlider;
class BStringView;
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
			void	_LoadIcons(BRect rect);
			void 	_SendMessageToFilter(int32 code);
			void	_SwitchState(bool state);

	BBitmap*		fIconOff;
	BBitmap*		fIconOn;
	BButton*		fIconButton;
	BCheckBox* 		fReplicantBox;
	// BCheckBox* 		fEnabled;
	BStringView*	fStatusView;
	ModifierBox* 	fModBox;
	BSlider* 		fAccelSlider;

	Preferences 	fPrefs;
};

#endif // SETTINGSWIN_H
