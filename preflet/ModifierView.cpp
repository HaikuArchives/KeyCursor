/*
 * Copyright 2004-2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Thomas Thiriez (code from his EasyMove app)
 *		Oscar Lesta
 */


#include "ModifierView.h"


ModifierView::ModifierView(
	BRect frame, uint32 modifierMask, const char* label, bool initialValue, BMessage* msg)
	:
	BCheckBox(frame, "", label, msg),
	fModifierMask(modifierMask)
{
	SetValue(initialValue);
}


ModifierView::~ModifierView()
{
}


uint32
ModifierView::GetMask()
{
	if (Value())
		return fModifierMask;

	return 0;
}
