/*
 * KeyCursor.h
 *
 *	KeyCursor is an input_server filter add-on and an input_server
 *	device add-on that allows the cursor to be controlled with the
 *	keyboard. The KeyCursorFilter watches the keyboard event stream
 *	for key up and key down events that are control keys for the
 *	keyboard cursor, and when it processes a control event, it sends
 *	a message over a port to the KeyCursorDevice. The KeyCursorDevice
 *	keeps track of which control keys are down, and if necessary,
 *	generates pointer events to simulate mouse movement or mouse
 *	button clicks.
 *
 *	Copyright 2000, Be Incorporated. All Rights Reserved.
 *	This file may be used under the terms of the Be Sample Code License.
 *
 *  Copyright 2004-2023, HaikuArchives Team
 *  Distributed under the terms of the MIT License.
 *
 *  Authors:
 * 		Nathan Schrenk
 * 		Thomas Thiriez (code from his EasyMove app)
 *		Oscar Lesta
 *		Humdinger
 */
#ifndef KEY_CURSOR_H
#define KEY_CURSOR_H

#include <add-ons/input_server/InputServerFilter.h>
#include <add-ons/input_server/InputServerDevice.h>
#include <OS.h>

#include "Preferences.h"

extern "C" _EXPORT BInputServerDevice* instantiate_input_device();
extern "C" _EXPORT BInputServerFilter* instantiate_input_filter();
extern "C" _EXPORT BView* instantiate_deskbar_item(float maxWidth, float maxHeight);

void ShowAbout();

#define APP_SIGNATURE "application/x-vnd.OscarL-KeyCursorSettings"
#define REPLICANT_SIGNATURE "KeyCursorReplicant"
#define REPLICANT_VIEW_NAME "KeyCursorView"

#define KEY_CURSOR_DEVICE_NAME			"Keyboard Cursor"
#define KEY_CURSOR_DEVICE_THREAD_NAME	"KeyCursorDevice"
#define KEY_CURSOR_DEVICE_PORT_NAME		"KeyCursorDevice port"

// message codes for cursor movement
#define LEFT_KEY_DOWN		'laKD'
#define RIGHT_KEY_DOWN		'raKD'
#define UP_KEY_DOWN			'uaKD'
#define DOWN_KEY_DOWN		'daKD'
#define LEFT_KEY_UP			'laKU'
#define RIGHT_KEY_UP		'raKU'
#define UP_KEY_UP			'uaKU'
#define DOWN_KEY_UP			'daKU'

// message codes for 'wheel' events.
#define PAGE_UP_KEY_DOWN	'puKD'
#define PAGE_DOWN_KEY_DOWN	'pdKD'
#define PAGE_UP_KEY_UP		'puKU'
#define PAGE_DOWN_KEY_UP	'pdKU'

// message codes for button clicks, button number sent as data
#define BUTTON_DOWN			'bUTD'
#define BUTTON_UP			'bUTU'

// message codes to control the filter/device
#define QUIT_COMMAND		'quIT'
#define STATE				'stat'
#define TOGGLE				'togl'

// messages of the replicant
#define ABOUT				'abut'
#define OPEN_PREFS			'pref'


class KeyCursorFilter : public BInputServerFilter
{
public:
							KeyCursorFilter();
	virtual 				~KeyCursorFilter();

	virtual	status_t		InitCheck();
	virtual	filter_result	Filter(BMessage* message, BList* outList);

private:
			void			_SendMessageToDevice(int32 what, int32 data = 0);
			void			_SendStatus();
			void			_Toggle();

			void			_AddToDeskbar();
			void			_RemoveFromDeskbar();
			bool			_IsLivingInDeskbar();

			BMessenger*		_ReplicantMessenger();
			int32			_GetReplicantAt(BMessenger target, int32 index) const;
			status_t		_GetReplicantName(BMessenger target, int32 uid, BMessage* reply) const;
			status_t		_GetReplicantView(BMessenger target, int32 uid, BMessage* reply) const;

			// This thread will wait for preference's changes.
			static int32	PrefsThreadFunc(void* cookie);

			thread_id 		fPrefsThread;

			port_id			fPortID;

			uint32			fNecessaryMods;
			uchar			fButtonPressed;
			bool			fLeftPressed;
			bool			fRightPressed;
			bool			fUpPressed;
			bool			fDownPressed;
			bool			fWheelUpPressed;
			bool			fWheelDownPressed;
			bool			fToggleMode;
			bool			fToggleOn;

			Preferences 	fPrefs;
};


class KeyCursorDevice : public BInputServerDevice
{
public:
							KeyCursorDevice();
	virtual					~KeyCursorDevice();

	virtual status_t		InitCheck();
	virtual status_t		SystemShuttingDown();

	virtual status_t		Start(const char* device, void* cookie);
	virtual	status_t		Stop(const char* device, void* cookie);
	virtual status_t		Control(const char* device,
									void*       cookie,
									uint32      code,
									BMessage*   message);

private:
		static	int32		ThreadFunc(void* castToKeyCursorDevice);
				void		MessageLoop();
				void		ProcessMessage(int32 what, int32 data);
				void		GenerateMotionEvent();

				bigtime_t 	fLeftTime;
				bigtime_t 	fRightTime;
				bigtime_t 	fUpTime;
				bigtime_t 	fDownTime;

				bigtime_t	fWheelUpTime;
				bigtime_t	fWheelDownTime;

				bigtime_t	fTickInterval;
				bigtime_t	fLastClick;
				bigtime_t	fClickSpeed;
				thread_id	fThreadID;
				port_id		fPortID;
				float		fAcceleration;
				int32		fClickCount;
				char		fClickedButton;

				Preferences fPrefs;
};

#endif // KEY_CURSOR_H
