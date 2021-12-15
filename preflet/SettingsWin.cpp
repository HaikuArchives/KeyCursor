#include <stdlib.h>

#include <Application.h>
#include <Box.h>
#include <Slider.h>
#include <Screen.h>
#include <String.h>
#include <TextView.h>
#include <View.h>

#include "ModifierBox.h"
#include "ModifierView.h"
#include "SettingsWin.h"

SettingsWin::SettingsWin()
	: BWindow(BRect(100, 100, 400, 300), "KeyCursor Settings", B_TITLED_WINDOW,
				B_ASYNCHRONOUS_CONTROLS | B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	bgView = new BView(BRect(Bounds()), "", B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	bgView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(bgView);

	fEnabled = new BCheckBox(BRect(10, 5, 10, 10), "fEnabled", "Enable KeyCursor",
							new BMessage(ENABLED_CHANGED));
	fEnabled->SetValue(fPrefs.GetEnabled());
	fEnabled->ResizeToPreferred();
	bgView->AddChild(fEnabled);

	fEnabled->MakeFocus(true);

	float right;
	float bottom;
	BRect frame;

	frame = BRect(10, fEnabled->Frame().bottom + 10, 10,
					fEnabled->Frame().bottom + 10);

	fModBox = new ModifierBox(frame, "Toggle Keys", fPrefs.GetToggleModMask(),
							 fPrefs.GetClickKeyMask());
	fModBox->SetDefaultModifierMask(fPrefs.GetDefaultClickKeyMask());

	fModBox->SetMessage(new BMessage(TOGGLE_CHANGED));
	fModBox->SetTarget(this);
	bgView->AddChild(fModBox);

	fModBox->SetEnabled(fPrefs.GetEnabled());

	bottom = fModBox->Frame().bottom;
	right = fModBox->Frame().right;

	fAccelSlider = new BSlider( BRect(10, fModBox->Frame().bottom + 10, 10,
								fModBox->Frame().bottom + 10), "accel_slider",
								"Acceleration Factor",
								new BMessage(ACCEL_CHANGED), 0, 40);

	fAccelSlider->SetKeyIncrementValue(10);
	fAccelSlider->SetHashMarks(B_HASH_MARKS_BOTTOM);
	fAccelSlider->SetHashMarkCount(5);
	fAccelSlider->SetValue(fPrefs.GetAcceleration());
	fAccelSlider->SetLimitLabels("Mix", "Max");

	fAccelSlider->ResizeToPreferred();
	fAccelSlider->ResizeTo( fModBox->Frame().Width() - 2,
							fAccelSlider->Frame().Height());
	
	bgView->AddChild(fAccelSlider);

	fAccelSlider->SetEnabled(fPrefs.GetEnabled());

	bottom = fAccelSlider->Frame().bottom;
	ResizeTo(right + 10, bottom);

	MoveTo(fPrefs.WindowCorner());

	// Code to make sure that the window doesn't get drawn off screen...
	{
		BScreen screen;

		if (!(	screen.Frame().right >= Frame().right &&
				screen.Frame().bottom >= Frame().bottom))
			MoveTo( (screen.Frame().right  - Bounds().right)  * 0.5,
					(screen.Frame().bottom - Bounds().bottom) * 0.5);
	}
}

SettingsWin::~SettingsWin()
{
}

void SettingsWin::MessageReceived(BMessage* message)
{
	switch(message->what)
	{
		case TOGGLE_CHANGED:
			fPrefs.SetToggleModMask(fModBox->GetModifierMask());

			fPrefs.Save();
			SendUpdateMessage();
		break;

		case ENABLED_CHANGED:
			fPrefs.SetEnabled(fEnabled->Value());
			fPrefs.Save();
			fModBox->SetEnabled(!fModBox->IsEnabled());
			fAccelSlider->SetEnabled(!fAccelSlider->IsEnabled());
			SendUpdateMessage();
		break;

		case ACCEL_CHANGED:
		{
			float rate = fAccelSlider->Value();
			if ((rate < 0) || (rate > 40))
				rate = 10;

			fPrefs.SetAcceleration(rate);
			fPrefs.Save();
			SendUpdateMessage();
		}
		break;

		default:
			BWindow::MessageReceived(message);
	}
}

bool SettingsWin::QuitRequested()
{
	fPrefs.SetWindowCorner(BPoint(Frame().left,Frame().top));

	be_app_messenger.SendMessage(B_QUIT_REQUESTED);

	return true;
}

void SettingsWin::SendUpdateMessage()
{
	port_id port = find_port(KEY_CURSOR_PREFS_PORT_NAME);

	if (port < 0) return;

	write_port(port, PREFS_CHANGED, NULL, 0);
}
