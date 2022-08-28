/*
 * KeyCursorDevice.cpp
 *
 *	Copyright 2000, Be Incorporated.   All Rights Reserved.
 *	This file may be used under the terms of the Be Sample Code License.
 */

#include "KeyCursor.h"
#include <InterfaceDefs.h>
#include <Message.h>
#include <View.h>	// for B_PRIMARY_MOUSE_BUTTON, etc.
#include <stdio.h>

//------------------------------------------------------------------------------

extern "C" {

BInputServerDevice* instantiate_input_device()
{
	return new KeyCursorDevice();
}

} // extern "C"

//-------------------------------------------------------------------------------

KeyCursorDevice::KeyCursorDevice()
	: BInputServerDevice()
{
	fLeftTime = 0;
	fRightTime = 0;
	fUpTime = 0;
	fDownTime = 0;
	fWheelUpTime = 0;
	fWheelDownTime = 0;

	fTickInterval = 20000; // 50 ticks/second

	fLastClick = 0;
	fThreadID = -1;
	fPortID = -1;
	fClickCount = 0;
	fClickedButton = 0;
	fAcceleration = fPrefs.GetAcceleration() / 1000000.0f;
}

KeyCursorDevice::~KeyCursorDevice()
{
	// XXX: need to quit thread, etc.
}

status_t KeyCursorDevice::InitCheck()
{
	input_device_ref* devices[2];
	input_device_ref devRef = {	(char*) KEY_CURSOR_DEVICE_NAME,
								B_POINTING_DEVICE,
								(void*) this };
	devices[0] = &devRef;
	devices[1] = NULL;
	return RegisterDevices(devices);
}

status_t KeyCursorDevice::SystemShuttingDown()
{
	return B_OK;
}

status_t KeyCursorDevice::Start(const char* /*device*/, void* /*cookie*/)
{
	if (fThreadID < B_OK)
	{
		fPortID = create_port(50, KEY_CURSOR_DEVICE_PORT_NAME);
		fThreadID = spawn_thread(KeyCursorDevice::ThreadFunc,
								KEY_CURSOR_DEVICE_THREAD_NAME,
								B_URGENT_DISPLAY_PRIORITY,
								(void*) this);
		resume_thread(fThreadID);
	}
	return B_OK;
}

status_t KeyCursorDevice::Stop(const char* /*device*/, void* /*cookie*/)
{
	if (fPortID > 0)
	{
		sem_id quitSem = create_sem(0, "KeyCursorDevice quit sem");
		write_port(fPortID, QUIT_COMMAND, (void*) &quitSem, sizeof(quitSem));
		acquire_sem(quitSem);
		delete_sem(quitSem);
		delete_port(fPortID);
		fPortID = -1;
	}
	return B_OK;
}

status_t
KeyCursorDevice::Control(const char* device, void* cookie, uint32 code, BMessage* message)
{
	BInputServerDevice::Control(device, cookie, code, message);
	return B_OK;
}

int32 KeyCursorDevice::ThreadFunc(void* castToKeyCursorDevice)
{
	KeyCursorDevice* device = (KeyCursorDevice*) castToKeyCursorDevice;
	device->MessageLoop();
	return 0;
}

void KeyCursorDevice::MessageLoop()
{
	for (;;)
	{
		int32 what = 0;
		int32 data = 0;
		while (read_port_etc(fPortID, &what, &data, sizeof(data), B_TIMEOUT, 0) >= 0)
		{
			if (what == QUIT_COMMAND)
			{
				release_sem((sem_id) data);
				return;
			}

			ProcessMessage(what, data);
		}
		GenerateMotionEvent();
		snooze(fTickInterval);
	}
}

void KeyCursorDevice::ProcessMessage(int32 what, int32 data)
{
	bigtime_t now = system_time();

	if (what == PREFS_CHANGED)
	{
		fPrefs.Update();
		fAcceleration = fPrefs.GetAcceleration() / 1000000.0f;
		return;
	}

	switch (what)
	{
		case LEFT_KEY_DOWN:
			fLeftTime = now;
		break;
		case RIGHT_KEY_DOWN:
			fRightTime = now;
		break;
		case UP_KEY_DOWN:
			fUpTime = now;
		break;
		case DOWN_KEY_DOWN:
			fDownTime = now;
		break;
		case PAGE_UP_KEY_DOWN:
			fWheelUpTime = now;
		break;
		case PAGE_DOWN_KEY_DOWN:
			fWheelDownTime = now;
		break;

		case LEFT_KEY_UP:
			fLeftTime = 0;
		break;
		case RIGHT_KEY_UP:
			fRightTime = 0;
		break;
		case UP_KEY_UP:
			fUpTime = 0;
		break;
		case DOWN_KEY_UP:
			fDownTime = 0;
		break;
		case PAGE_UP_KEY_UP:
			fWheelUpTime = 0;
		break;
		case PAGE_DOWN_KEY_UP:
			fWheelDownTime = 0;
		break;

		case BUTTON_DOWN:
		{
			bigtime_t click_speed;
			get_click_speed(&click_speed);
			char clicked = (char) data;

			if (clicked != fClickedButton)
			{
				fClickCount = 1;
				fClickedButton = clicked;
			}
			else
			{
				if ((now - fLastClick) < click_speed)
					fClickCount++;
				else
					fClickCount = 1;
			}

			fLastClick = now;

			int32 buttonMask;
			switch (fClickedButton)
			{
				case 1:	buttonMask = B_PRIMARY_MOUSE_BUTTON; break;
				case 2:	buttonMask = B_SECONDARY_MOUSE_BUTTON; break;
				case 3:	buttonMask = B_TERTIARY_MOUSE_BUTTON; break;
				default: buttonMask = 0; break;
			}

			BMessage* event = new BMessage(B_MOUSE_DOWN);
			event->AddInt64("when", now);
			event->AddInt32("buttons", buttonMask);
			event->AddInt32("clicks", fClickCount);
			event->AddInt32("x", 0);
			event->AddInt32("y", 0);
			EnqueueMessage(event);
		}
		break;

		case BUTTON_UP:
		{
			fClickedButton = 0;
			fLastClick = 0;
			fClickCount = 0;

			BMessage* event = new BMessage(B_MOUSE_UP);
			event->AddInt64("when", now);
			event->AddInt32("x", 0);
			event->AddInt32("y", 0);
			EnqueueMessage(event);
		}
		break;
	}
}

void KeyCursorDevice::GenerateMotionEvent()
{
	if (fLeftTime || fRightTime || fUpTime || fDownTime || fWheelUpTime || fWheelDownTime)
	{
		bigtime_t now = system_time();
		int32 x(0), y(0), w(0);
		int32 delta;

		if (fLeftTime)
		{
			delta = (int32) (fAcceleration * (now - fLeftTime));
			x -= (delta > 0) ? delta : 1;
		}

		if (fRightTime)
		{
			delta = (int32) (fAcceleration * (now - fRightTime));
			x += (delta > 0) ? delta : 1;
		}

		if (fUpTime)
		{
			delta = (int32) (fAcceleration * (now - fUpTime));
			y += (delta > 0) ? delta : 1;
		}

		if (fDownTime)
		{
			delta = (int32) (fAcceleration * (now - fDownTime));
			y -= (delta > 0) ? delta : 1;
		}

		if (fWheelUpTime)
		{
			delta = (int32) (now - fWheelUpTime);
			w -= (delta > 0) ? 1 : 0;
		}

		if (fWheelDownTime)
		{
			delta = (int32) (now - fWheelDownTime);
			w -= (delta > 0) ? -1 : 0;
		}

		int32 buttonMask;
		switch (fClickedButton)
		{
			case 1 : buttonMask = B_PRIMARY_MOUSE_BUTTON;   break;
			case 2 : buttonMask = B_SECONDARY_MOUSE_BUTTON; break;
			case 3 : buttonMask = B_TERTIARY_MOUSE_BUTTON;  break;
			default: buttonMask = 0;                        break;
		}

		BMessage* event = new BMessage(B_MOUSE_MOVED);
		event->AddInt64("when", now);
		event->AddInt32("buttons", buttonMask);
		event->AddInt32("x", x);
		event->AddInt32("y", y);
		EnqueueMessage(event);

		if (w)
		{
			event = new BMessage(B_MOUSE_WHEEL_CHANGED);
			event->AddInt64("when", now);
//			event->AddFloat("be:wheel_delta_x", w);
			event->AddFloat("be:wheel_delta_y", w);
			EnqueueMessage(event);
		}
	}
}
