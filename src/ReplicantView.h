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
			void	_Remove();
			void	_ShowAbout();

	BBitmap*		fIcon;
	BPopUpMenu*		fPopup;
};

#endif // REPLICANTVIEW_H

