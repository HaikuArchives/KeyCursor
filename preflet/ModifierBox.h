/*
 * Copyright 2004-2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Thomas Thiriez (code from his EasyMove app)
 *		Oscar Lesta
 */

#ifndef MODIFIERBOX_H
#define MODIFIERBOX_H

#include <Box.h>
#include <Invoker.h>

class ModifierView;

class ModifierBox : public BInvoker, public BBox
{
public:
					ModifierBox(BRect frame, const char* label = 0, uint32 modifier = 0,
						uint32 button = 0);
					~ModifierBox();

	virtual void 	MessageReceived(BMessage* message);
	virtual void 	AttachedToWindow();

			uint32	GetModifierMask();
			void	SetModifierMask(uint32 mask);
			void	SetDefaultModifierMask(uint32 mask);

			bool	IsEnabled() { return fControlsEnabled; };
			void	SetEnabled(bool enabled);

private:
	struct MaskListType
	{
		uint32 mask;
		const char* label;
	};

	static const 	MaskListType ModifierList[];
	ModifierView**	fModifierViewList;
	uint32			fDefaultModifier;

	static const uint32 MODIFIER_CHANGED = 'modc';

	bool			fControlsEnabled;
};

#endif // MODIFIERBOX_H