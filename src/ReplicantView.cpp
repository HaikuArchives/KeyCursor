/*
 * Copyright 1999-2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Michał Kowalski
 *		Humdinger
 */


#include "ReplicantView.h"

#include <Alert.h>
#include <Bitmap.h>
#include <Catalog.h>
#include <Deskbar.h>
#include <IconUtils.h>
#include <MenuItem.h>
#include <Roster.h>
#include <Resources.h>
#include <Screen.h>

#include <stdio.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Replicant"

status_t our_image(image_info& image);


BView*
instantiate_deskbar_item(float maxWidth, float maxHeight)
{
	return new ReplicantView(BRect(0, 0, maxHeight - 1, maxHeight - 1));
}


status_t
our_image(image_info& image)
{
	int32 cookie = 0;
	while (get_next_image_info(B_CURRENT_TEAM, &cookie, &image) == B_OK) {
		if ((char*)our_image >= (char*)image.text
			&& (char*)our_image <= (char*)image.text + image.text_size)
			return B_OK;
	}
	BAlert* alert = new BAlert("image", "Image NOT OK", "NOT");
	alert->Show();
	return B_ERROR;
}


ReplicantView::ReplicantView(BRect rect)
	:
	BView(rect, REPLICANT_VIEW_NAME, B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FRAME_EVENTS)
{
	_Init();
}


ReplicantView::ReplicantView(BMessage* archive)
	:
	BView(archive)
{
	_Init();
}


void
ReplicantView::AttachedToWindow()
{
	SetViewColor(B_TRANSPARENT_COLOR);

	// Build popup menu
	fPopup = new BPopUpMenu("PopUpMenu", false, false);
	fPopup->AddItem(new BMenuItem(B_TRANSLATE("Preferences" B_UTF8_ELLIPSIS), new BMessage(OPEN_PREFS)));
	fPopup->AddSeparatorItem();
	fPopup->AddItem(new BMenuItem(B_TRANSLATE("About" B_UTF8_ELLIPSIS), new BMessage(ABOUT)));
	fPopup->SetTargetForItems(this);
}


void
ReplicantView::Draw(BRect rect)
{
	if (fIconOn == NULL || fIconOff == NULL) {
		// At least display something...
		rgb_color lowColor = LowColor();
		SetLowColor(0, 113, 187, 255);
		FillRoundRect(Bounds().InsetBySelf(3.f, 0.f), 5.f, 7.f, B_SOLID_LOW);
		SetLowColor(lowColor);
	} else {
		SetDrawingMode(B_OP_ALPHA);
		DrawBitmap(fState == true ? fIconOn : fIconOff);
		SetDrawingMode(B_OP_COPY);
	}
}


void
ReplicantView::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case ABOUT:
			ShowAbout();
			break;

		case OPEN_PREFS:
			be_roster->Launch(APP_SIGNATURE);
			break;

		case STATE:
			message->FindBool("status", &fState);
			Invalidate();
			break;

		default:
			BView::MessageReceived(message);
			break;
	}
}


void
ReplicantView::MouseDown(BPoint point)
{
	BPoint cursor;
	uint32 buttons = 0;
	GetMouse(&cursor, &buttons, true);

	if (buttons & B_SECONDARY_MOUSE_BUTTON) {
		ConvertToScreen(&point);
		fPopup->Go(point, true, true, BRect(point, point + BPoint(20, 20)), true);
	}

	if (buttons & B_PRIMARY_MOUSE_BUTTON)
		_SendMessageToFilter(TOGGLE);
}


status_t
ReplicantView::Archive(BMessage* dataMsg, bool deep) const
{
	status_t status = BView::Archive(dataMsg, deep);

	if (status == B_OK)
		status = dataMsg->AddString("add_on", APP_SIGNATURE);

	if (status == B_OK)
		status = dataMsg->AddString("class", REPLICANT_SIGNATURE);

	return status;
}


ReplicantView*
ReplicantView::Instantiate(BMessage* dataMsg)
{
	if (!validate_instantiation(dataMsg, REPLICANT_SIGNATURE))
		return NULL;

	return new ReplicantView(dataMsg);
}


void
ReplicantView::_Init()
{
	fIconOff = NULL;
	fIconOn = NULL;

	image_info info;
	if (our_image(info) != B_OK)
		return;

	BFile file(info.name, B_READ_ONLY);
	if (file.InitCheck() < B_OK)
		return;

	BResources resources(&file);
	if (resources.InitCheck() < B_OK)
		return;

	size_t size;
	const void* data = resources.LoadResource(B_VECTOR_ICON_TYPE, "tray_icon_off", &size);
	if (data != NULL) {
		BBitmap* icon = new BBitmap(Bounds(), B_RGBA32);
		if (icon->InitCheck() == B_OK
			&& BIconUtils::GetVectorIcon((const uint8*)data, size, icon) == B_OK)
			fIconOff = icon;
		else
			delete icon;
	}

	data = resources.LoadResource(B_VECTOR_ICON_TYPE, "tray_icon_on", &size);
	if (data != NULL) {
		BBitmap* icon = new BBitmap(Bounds(), B_RGBA32);
		if (icon->InitCheck() == B_OK
			&& BIconUtils::GetVectorIcon((const uint8*)data, size, icon) == B_OK)
			fIconOn = icon;
		else
			delete icon;
	}
}


void
ReplicantView::_SendMessageToFilter(int32 code)
{
	port_id port = find_port(KEY_CURSOR_PREFS_PORT_NAME);

	if (port < 0)
		return;

	write_port(port, code, NULL, 0);
}
