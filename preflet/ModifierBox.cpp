/*
 * Copyright 2004-2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Thomas Thiriez (code from his EasyMove app)
 *		Oscar Lesta
 *		Humdinger
 */


#include "ModifierBox.h"
#include "ModifierView.h"

#include <Catalog.h>
#include <LayoutBuilder.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "ModifierBox"


const ModifierBox::MaskListType ModifierBox::ModifierList[] =
{
	{ B_SHIFT_KEY,   B_TRANSLATE("Shift") },
	{ B_CONTROL_KEY, B_TRANSLATE("Control") },
	{ B_COMMAND_KEY, B_TRANSLATE("Command") },
	{ B_OPTION_KEY,  B_TRANSLATE("Option") },
	{ B_MENU_KEY,    B_TRANSLATE("Menu") }
};


ModifierBox::ModifierBox(const char* label, uint32 modifier, uint32 /*button*/)
	:
	BBox(label),
	fDefaultModifier(0)
{
	if (label)
		SetLabel(label);

	fModifierViewList = new ModifierView*[5];

	fModifierViewList[0] = new ModifierView(ModifierList[0].mask,
		ModifierList[0].label, ModifierList[0].mask & modifier, new BMessage(MODIFIER_CHANGED));
	fModifierViewList[1] = new ModifierView(ModifierList[1].mask,
		ModifierList[1].label, ModifierList[1].mask & modifier, new BMessage(MODIFIER_CHANGED));
	fModifierViewList[2] = new ModifierView(ModifierList[2].mask,
		ModifierList[2].label, ModifierList[2].mask & modifier, new BMessage(MODIFIER_CHANGED));
	fModifierViewList[3] = new ModifierView(ModifierList[3].mask,
		ModifierList[3].label, ModifierList[3].mask & modifier, new BMessage(MODIFIER_CHANGED));
	fModifierViewList[4] = new ModifierView(ModifierList[4].mask,
		ModifierList[4].label, ModifierList[4].mask & modifier, new BMessage(MODIFIER_CHANGED));

	BLayoutBuilder::Group<>(this, B_HORIZONTAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_ITEM_INSETS)
		.AddGroup(B_VERTICAL, 0)
			.SetInsets(0, B_USE_DEFAULT_SPACING, 0, 0)
			.Add(fModifierViewList[0])
			.Add(fModifierViewList[1])
			.Add(fModifierViewList[2])
		.End()
		.AddGroup(B_VERTICAL, 0)
			.SetInsets(0, B_USE_DEFAULT_SPACING, 0, 0)
			.Add(fModifierViewList[3])
			.Add(fModifierViewList[4])
			.AddGlue()
		.End();
}


ModifierBox::~ModifierBox()
{
	delete fModifierViewList;
}


void
ModifierBox::MessageReceived(BMessage* message)
{
	uint32 mask = 0;

	switch (message->what) {
		case MODIFIER_CHANGED:
			for (int32 i = 0; i < 5; i++)
				mask |= fModifierViewList[i]->GetMask();
			if (!mask) {
				mask = fDefaultModifier;
				for (int32 i = 0; i < 5; i++)
					fModifierViewList[i]->SetValue(ModifierList[i].mask & mask);
			}
			InvokeNotify(Message());
			break;

		default:
			BBox::MessageReceived(message);
	}
}


void
ModifierBox::AttachedToWindow()
{
	for (int32 i = 0; i < 5; i++)
		fModifierViewList[i]->SetTarget(this);
}


uint32
ModifierBox::GetModifierMask()
{
	uint32 mask = 0;

	for (int32 i = 0; i < 5; i++)
		mask |= fModifierViewList[i]->GetMask();

	return mask;
}


void
ModifierBox::SetModifierMask(uint32 mask)
{
	for (int32 i = 0; i < 5; i++)
		fModifierViewList[i]->SetValue(ModifierList[i].mask & mask);
}


void
ModifierBox::SetDefaultModifierMask(uint32 mask)
{
	fDefaultModifier = mask;
}


void
ModifierBox::SetEnabled(bool enabled)
{
	for (int i = 0; i < 5; i++)
		fModifierViewList[i]->SetEnabled(enabled);

	fControlsEnabled = enabled;
}
