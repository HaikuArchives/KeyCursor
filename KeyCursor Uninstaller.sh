#!/bin/sh

ANSWER=$(alert --info --modal "Uninstall KeyCursor? Sure?" "Yes" "No")
if [ "$ANSWER" == "Yes" ]
then
	rm /boot/home/config/add-ons/input_server/filters/KeyCursor
	rm /boot/home/config/add-ons/input_server/devices/KeyCursor

	/system/servers/input_server -q

	rm /boot/home/config/settings/KeyCursor
	rm /boot/preferencess/KeyCursor
	rm /boot/home/config/be/Preferences/KeyCursor

	sleep 3s
	alert "KeyCursor was removed."
fi
