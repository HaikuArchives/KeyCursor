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
#include <StringView.h>
#include <TextView.h>
#include <View.h>

#include "ModifierBox.h"
#include "ModifierView.h"
#include "SettingsWin.h"

#include <stdlib.h>
#include <cstdio>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "SettingsWindow"

static const char* kStatusText = B_TRANSLATE_MARK("Current toggle state:");

SettingsWin::SettingsWin()
	:
	BWindow(BRect(100, 100, 500, 300), B_TRANSLATE("KeyCursor settings"), B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS | B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_AUTO_UPDATE_SIZE_LIMITS)
{
	// Enable checkbox
	// fEnabled = new BCheckBox("enable", B_TRANSLATE("Enable KeyCursor"),
		// new BMessage(ENABLED_CHANGED));
	// fEnabled->SetValue(fPrefs.GetEnabled());
	// fEnabled->MakeFocus(true);

	// Status bar
	BString status(kStatusText);
	status << " -";
	fStatusView = new BStringView("status", status);
	fStatusView->SetAlignment(B_ALIGN_CENTER);

	BView* statusView = new BView("statusView", B_SUPPORTS_LAYOUT);
	BLayoutBuilder::Group<>(statusView, B_VERTICAL, 0)
		.SetInsets(0, -1, 0, -2)
		.Add(new BSeparatorView(B_HORIZONTAL))
		.Add(fStatusView)
		.Add(new BSeparatorView(B_HORIZONTAL));

	BFont font = *be_plain_font;
	font_height fontHeight;
	font.GetHeight(&fontHeight);
	float height = ceilf(fontHeight.ascent + fontHeight.descent + fontHeight.leading);
	statusView->SetExplicitMinSize(BSize(B_SIZE_UNSET, height * 1.5));
	rgb_color color = tint_color(ui_color(B_PANEL_BACKGROUND_COLOR), B_DARKEN_1_TINT);
	statusView->SetViewColor(color);

	// Toogle keys
	fModBox = new ModifierBox(B_TRANSLATE("Toggle keys"), fPrefs.GetToggleModMask(),
		fPrefs.GetClickKeyMask());
	fModBox->SetDefaultModifierMask(fPrefs.GetDefaultClickKeyMask());
	fModBox->SetMessage(new BMessage(TOGGLE_CHANGED));
	fModBox->SetTarget(this);
	// fModBox->SetEnabled(fPrefs.GetEnabled());

	// Acceleration slider
	fAccelSlider = new BSlider("accel_slider", B_TRANSLATE("Acceleration"),
			new BMessage(ACCEL_CHANGED), 0, 40, B_HORIZONTAL);

	fAccelSlider->SetKeyIncrementValue(10);
	fAccelSlider->SetHashMarks(B_HASH_MARKS_BOTTOM);
	fAccelSlider->SetHashMarkCount(5);
	fAccelSlider->SetValue(fPrefs.GetAcceleration());
	fAccelSlider->SetLimitLabels(B_TRANSLATE("Min"), B_TRANSLATE("Max"));
	// fAccelSlider->SetEnabled(fPrefs.GetEnabled());
	fAccelSlider->SetExplicitMinSize(
		BSize(font.StringWidth("Quite a long string as window min width"),B_SIZE_UNSET));

	// Building layout
	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.AddGroup(B_VERTICAL)
			.SetInsets(B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS, 0)
			// .Add(fEnabled)
		.End()
		.Add(statusView)
		.AddGroup(B_VERTICAL)
			.SetInsets(B_USE_WINDOW_INSETS, 0, B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS)
			.Add(fModBox)
			.Add(fAccelSlider)
		.End();

	CenterOnScreen();

	// Get current toggle status
	_SendMessageToFilter(STATE);
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
			_SendMessageToFilter(PREFS_CHANGED);
		} break;

		// case ENABLED_CHANGED:
		// {
			// fPrefs.SetEnabled(fEnabled->Value());
			// fPrefs.Save();
			// fModBox->SetEnabled(!fModBox->IsEnabled());
			// fAccelSlider->SetEnabled(!fAccelSlider->IsEnabled());
			// _SendMessageToFilter(PREFS_CHANGED);
		// } break;

		case ACCEL_CHANGED:
		{
			float rate = fAccelSlider->Value();
			if ((rate < 0) || (rate > 40))
				rate = 10;

			fPrefs.SetAcceleration(rate);
			fPrefs.Save();
			_SendMessageToFilter(PREFS_CHANGED);
		} break;

		case STATE:
		{
			bool state;
			BString text(kStatusText);
			text << " ";
			if (message->FindBool("status", &state) == B_OK) {
				if (state == true)
					text << B_TRANSLATE_COMMENT("on", "Status as in on/off");
				else
					text << B_TRANSLATE_COMMENT("off", "Status as in on/off");
			} else
				text << "-";

			fStatusView->SetText(text);
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
SettingsWin::_SendMessageToFilter(int32 code)
{
	port_id port = find_port(KEY_CURSOR_PREFS_PORT_NAME);

	if (port < 0)
		return;

	write_port(port, code, NULL, 0);
}
