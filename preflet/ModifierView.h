/*
 * Copyright 2004-2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Thomas Thiriez (code from his EasyMove app)
 *		Oscar Lesta
 */
#ifndef MODIFIERVIEW_H
#define MODIFIERVIEW_H

#include <CheckBox.h>

class ModifierView : public BCheckBox
{
public:
			ModifierView(BRect frame, uint32 modifierMask, const char* label,
				bool initialValue, BMessage* msg);
			~ModifierView();

	uint32	GetMask();

private:
	uint32	fModifierMask;
};

#endif // MODIFIERVIEW_H
