/*
 * Copyright 2004-2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Oscar Lesta
 *		Humdinger
 */


#include <Application.h>
#include <Box.h>
#include <Catalog.h>
#include <LayoutBuilder.h>
#include <Screen.h>
#include <SeparatorView.h>
#include <Slider.h>
#include <String.h>
#include <TextView.h>
#include <View.h>

#include "ModifierBox.h"
#include "ModifierView.h"
#include "SettingsWin.h"

#include <stdlib.h>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "SettingsWindow"

SettingsWin::SettingsWin()
	:
	BWindow(BRect(100, 100, 500, 300), B_TRANSLATE("KeyCursor settings"), B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS | B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_AUTO_UPDATE_SIZE_LIMITS)
{
	fEnabled = new BCheckBox("enable", B_TRANSLATE("Enable KeyCursor"),
		new BMessage(ENABLED_CHANGED));
	fEnabled->SetValue(fPrefs.GetEnabled());
	fEnabled->MakeFocus(true);

	fModBox = new ModifierBox(B_TRANSLATE("Toggle keys"), fPrefs.GetToggleModMask(),
		fPrefs.GetClickKeyMask());
	fModBox->SetDefaultModifierMask(fPrefs.GetDefaultClickKeyMask());
	fModBox->SetMessage(new BMessage(TOGGLE_CHANGED));
	fModBox->SetTarget(this);
	fModBox->SetEnabled(fPrefs.GetEnabled());

	fAccelSlider = new BSlider("accel_slider", B_TRANSLATE("Acceleration"),
			new BMessage(ACCEL_CHANGED), 0, 40, B_HORIZONTAL);

	fAccelSlider->SetKeyIncrementValue(10);
	fAccelSlider->SetHashMarks(B_HASH_MARKS_BOTTOM);
	fAccelSlider->SetHashMarkCount(5);
	fAccelSlider->SetValue(fPrefs.GetAcceleration());
	fAccelSlider->SetLimitLabels(B_TRANSLATE("Min"), B_TRANSLATE("Max"));
	fAccelSlider->SetEnabled(fPrefs.GetEnabled());

	BFont font = *be_plain_font;
	fAccelSlider->SetExplicitMinSize(
		BSize(font.StringWidth("Quite a long string as window min width"),B_SIZE_UNSET));

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.AddGroup(B_VERTICAL)
			.SetInsets(B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS, 0)
			.Add(fEnabled)
		.End()
		.Add(new BSeparatorView(B_HORIZONTAL))
		.AddGroup(B_VERTICAL)
			.SetInsets(B_USE_WINDOW_INSETS, 0, B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS)
			.Add(fModBox)
			.Add(fAccelSlider)
		.End();

	CenterOnScreen();
}


SettingsWin::~SettingsWin()
{
}


void
SettingsWin::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case TOGGLE_CHANGED:
		{
			fPrefs.SetToggleModMask(fModBox->GetModifierMask());

			fPrefs.Save();
			SendUpdateMessage();
		} break;

		case ENABLED_CHANGED:
		{
			fPrefs.SetEnabled(fEnabled->Value());
			fPrefs.Save();
			fModBox->SetEnabled(!fModBox->IsEnabled());
			fAccelSlider->SetEnabled(!fAccelSlider->IsEnabled());
			SendUpdateMessage();
		} break;

		case ACCEL_CHANGED:
		{
			float rate = fAccelSlider->Value();
			if ((rate < 0) || (rate > 40))
				rate = 10;

			fPrefs.SetAcceleration(rate);
			fPrefs.Save();
			SendUpdateMessage();
		} break;

		default:
			BWindow::MessageReceived(message);
	}
}


bool
SettingsWin::QuitRequested()
{
	fPrefs.SetWindowCorner(BPoint(Frame().left, Frame().top));

	be_app_messenger.SendMessage(B_QUIT_REQUESTED);

	return true;
}


void
SettingsWin::SendUpdateMessage()
{
	port_id port = find_port(KEY_CURSOR_PREFS_PORT_NAME);

	if (port < 0)
		return;

	write_port(port, PREFS_CHANGED, NULL, 0);
}
