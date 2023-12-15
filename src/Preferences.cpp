/*
 * Copyright 2004-2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Thomas Thiriez (code from his EasyMove app)
 *		Oscar Lesta
 *		Humdinger
 */


#include <Directory.h>
#include <File.h>
#include <FindDirectory.h>
#include <Path.h>

#include "Preferences.h"


Preferences::Preferences()
{
	Load();
}


Preferences::~Preferences()
{
#ifdef I_AM_THE_PREFLET
	Save();
#endif
}


void
Preferences::Update()
{
	Load();
}


void
Preferences::SetToDefault()
{
	SetToggleModMask(GetDefaultToggleModMask());
	SetClickKeyMask(GetDefaultClickKeyMask());
	SetReplicantToDefault();
	// SetEnabledToDefault();
	SetDefaultAcceleration();

#ifdef I_AM_THE_PREFLET
	fCorner.x = 10000; // the preflet will center itself.
	fCorner.y = 10000;
#endif
}


BFile
Preferences::OpenFile(uint32 openMode)
{
	BPath path;
	find_directory(B_USER_SETTINGS_DIRECTORY, &path);
	BDirectory dir(path.Path());
	BFile file(&dir, kSettingsFileName, openMode);

	return file;
}


void
Preferences::Load()
{
	SetToDefault();

	BFile file = OpenFile(B_READ_ONLY);

	if (file.InitCheck() != B_OK)
		return;

	BMessage settings;
	if (settings.Unflatten(&file) == B_OK) {
		settings.FindUInt32("togglemodmask", &fToggleModMask);
		settings.FindUInt32("clickkeymask", &fClickKeyMask);
		settings.FindBool("replicant", &fReplicant);
		settings.FindFloat("acceleration", &fAcceleration);
#ifdef I_AM_THE_PREFLET
		if (settings.FindPoint("corner", &fCorner) != B_OK) {
			fCorner.x = 50;
			fCorner.y = 50;
		}
#endif
	}
}

#ifdef I_AM_THE_PREFLET // if compiling the preflet...


void
Preferences::Save()
{
	BFile file = OpenFile(B_WRITE_ONLY | B_CREATE_FILE);

	if (file.InitCheck() != B_OK)
		return;

	BMessage settings;
	settings.AddUInt32("togglemodmask", fToggleModMask);
	settings.AddUInt32("clickkeymask", fClickKeyMask);
	settings.AddBool("replicant", fReplicant);
	settings.AddFloat("acceleration", fAcceleration);
	settings.AddPoint("corner", fCorner);

	if (file.InitCheck() == B_OK)
		settings.Flatten(&file);
}


void
Preferences::SetWindowCorner(BPoint corner)
{
	fCorner = corner;
}

#endif // if compiling the preflet...
