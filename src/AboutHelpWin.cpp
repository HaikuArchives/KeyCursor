/*
 * Copyright 2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Humdinger
 */

#include <AboutWindow.h>
#include <Catalog.h>

#include "KeyCursor.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "AboutHelp"


void
ShowAbout()
{
	BAboutWindow* aboutwindow
		= new BAboutWindow(B_TRANSLATE_SYSTEM_NAME("KeyCursor"), APP_SIGNATURE);

	const char* authors[] = {
		"Oscar Lesta",
		"Humdinger",
		NULL
	};

	aboutwindow->AddCopyright(2004, "Oscar Lesta");
	aboutwindow->AddAuthors(authors);
	aboutwindow->AddDescription(B_TRANSLATE(
		"KeyCursor lets you control the mouse pointer with the keyboard."));

	const char* thanks[] = {
		B_TRANSLATE("Nathan Schrenk for his BeNewsletter article"),
		B_TRANSLATE("Thomas Thiriez for his EasyMove app"),
		NULL
	};
	aboutwindow->AddSpecialThanks(thanks);
	aboutwindow->Show();
}
