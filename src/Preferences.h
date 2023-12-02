/*
 * Copyright 2004-2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Thomas Thiriez (code from his EasyMove app)
 *		Oscar Lesta
 *		Humdinger
 */
#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <SupportDefs.h>
#include <View.h>

#define	TOGGLE_CHANGED	'kcTC'
#define	ENABLED_CHANGED	'kcEC'
#define	ACCEL_CHANGED	'kcAC'
#define	PREFS_CHANGED	'kcPC'

#define KEY_CURSOR_PREFS_PORT_NAME		"KeyCursor PrefsPort"

#define kSettingsFileName "KeyCursor"

class BFile;

class Preferences
{
public:
			Preferences();
			~Preferences();

	void	Update();

	uint32	GetToggleModMask() { return fToggleModMask; }
	uint32	GetClickKeyMask() { return fClickKeyMask; }

	void	SetToggleModMask(uint32 mask) { fToggleModMask = mask; }
	void	SetClickKeyMask(uint32 mask) { fClickKeyMask = mask; }

	uint32	GetDefaultToggleModMask() { return B_OPTION_KEY; }
	uint32	GetDefaultClickKeyMask() { return B_SPACE; }

	bool	GetEnabled() { return fEnabled; }
	void	SetEnabledToDefault() { fEnabled = true; }

	float	GetAcceleration() { return fAcceleration; }
	void	SetDefaultAcceleration() { fAcceleration = 20.0; }

	void	SetToDefault();

#ifdef I_AM_THE_PREFLET // if compiling the preflet...
	void	Save();

	void	SetEnabled(bool enabled) { fEnabled = enabled; }
	void	SetAcceleration(float accel) {fAcceleration = accel; }

	BPoint	WindowCorner() const { return fCorner; }
	void	SetWindowCorner(BPoint corner);
#endif

protected:
	BFile*	OpenFile(uint32 openMode);
	void	Load();

private:
	uint32	fToggleModMask;
	uint32	fClickKeyMask;
	bool	fEnabled;
	float	fAcceleration;

#ifdef I_AM_THE_PREFLET
	BPoint	fCorner;
#endif
};

#endif // PREFERENCES_H
