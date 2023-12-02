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
	SetEnabledToDefault();
	SetDefaultAcceleration();

#ifdef I_AM_THE_PREFLET
	fCorner.x = 10000; // the preflet will center itself.
	fCorner.y = 10000;
#endif
}


BFile*
Preferences::OpenFile(uint32 openMode)
{
	BPath path;
	find_directory(B_USER_SETTINGS_DIRECTORY, &path);
	BDirectory dir(path.Path());
	BFile* file = new BFile(&dir, kSettingsFileName, openMode);
	status_t err;
	err = file->InitCheck();

	if (err != B_OK) {
		delete file;
		return 0;
	}

	return file;
}


void
Preferences::Load()
{
	SetToDefault();

	BFile* file = OpenFile(B_READ_ONLY);

	if (!file)
		return;

	file->Read(&fToggleModMask, sizeof(uint32));
	file->Read(&fClickKeyMask, sizeof(uint32));
	file->Read(&fEnabled, sizeof(bool));
	file->Read(&fAcceleration, sizeof(float));

#ifdef I_AM_THE_PREFLET
	if (file->Read(&fCorner, sizeof(BPoint)) != sizeof(BPoint)) {
		fCorner.x = 50;
		fCorner.y = 50;
	};
#endif

	delete file;
}

#ifdef I_AM_THE_PREFLET // if compiling the preflet...


void
Preferences::Save()
{
	BFile* file = OpenFile(B_WRITE_ONLY | B_CREATE_FILE);

	if (!file)
		return;

	file->Write(&fToggleModMask, sizeof(uint32));
	file->Write(&fClickKeyMask, sizeof(uint32));
	file->Write(&fEnabled, sizeof(bool));
	file->Write(&fAcceleration, sizeof(float));
	file->Write(&fCorner, sizeof(BPoint));

	delete file;
}


void
Preferences::SetWindowCorner(BPoint corner)
{
	fCorner = corner;
}

#endif // if compiling the preflet...
