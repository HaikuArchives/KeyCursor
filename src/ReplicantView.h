/*
 * Copyright 1999, Michał Kowalski
 * Copyright 2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Michał Kowalski
 *		Humdinger
 */
#ifndef REPLICANTVIEW_H
#define REPLICANTVIEW_H

#include "KeyCursor.h"

#include <PopUpMenu.h>
#include <View.h>


class ReplicantView : public BView
{
public:
					ReplicantView(BRect rect);
					ReplicantView(BMessage* archive);

	virtual void 	AttachedToWindow();
	virtual void 	Draw(BRect rect);
	virtual void 	MessageReceived(BMessage* message);
	virtual void 	MouseDown(BPoint point);

	virtual status_t Archive(BMessage* dataMsg, bool deep = true) const;
	static ReplicantView* 	Instantiate(BMessage* dataMsg);

private:
			void	_Init();
			void 	_SendMessageToFilter(int32 code);

	bool			fState;
	BBitmap*		fIconOff;
	BBitmap*		fIconOn;
	BPopUpMenu*		fPopup;
};

#endif // REPLICANTVIEW_H

