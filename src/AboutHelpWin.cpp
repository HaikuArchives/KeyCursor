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
		"KeyCursor lets you control the mouse pointer with the keyboard.\n"
		"In the preference window, you choose the modifier key combination "
		"that will toggle control on/off.\n"
		"When toggled on, the cursor keys will move the mouse pointer.\n"
		"Hit SPACE for a left-click, SHIFT+SPACE for a right-click."
		));

	const char* thanks[] = {
		B_TRANSLATE("Nathan Schrenk for his BeNewsletter article"),
		B_TRANSLATE("Thomas Thiriez for his EasyMove app"),
		NULL
	};
	aboutwindow->AddSpecialThanks(thanks);

	// Approximate nice looking window size
	font_height fh;
	BFont font;
	font.GetHeight(&fh);
	float height = static_cast<float>(ceil(fh.ascent + fh.descent + fh.leading));
	float width = font.StringWidth(
		"KeyCursor lets you control the mouse pointer with the keyboard.") * 1.3;

	aboutwindow->ResizeTo(width, height * 22);
	aboutwindow->Show();
}