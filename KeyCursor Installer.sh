#!/bin/sh
# Installer for the KeyCursor input_server add-on & preflet.

BASE_DIR=$(dirname "$0")
SETTINGS_FILE="KeyCursor"
SETTINGS_DIR="/boot/home/config/settings"
PREFS_DIR="/boot/preferences"
FILTER_ADDON_DIR="/boot/home/config/add-ons/input_server/filters"
DEVICE_ADDON_DIR="/boot/home/config/add-ons/input_server/devices"

cd "$BASE_DIR"

rm "$SETTINGS_DIR/$SETTINGS_FILE"
copyattr --data ./preflet/KeyCursor "$PREFS_DIR"
copyattr --data ./src/KeyCursor "$FILTER_ADDON_DIR"
ln -s "$FILTER_ADDON_DIR"/KeyCursor "$DEVICE_ADDON_DIR"

/system/servers/input_server -q
snooze 3s

/boot/home/config/be/Preferences
copyattr --data "$PREFS_DIR"/KeyCursor
ln -s "$PREFS_DIR"/KeyCursor /boot/home/config/be/Preferences

sync

ANWSER=$(alert --info "KeyCursor has been installed.
Do you want to configure KeyCursor now?." "Yes" "No")

if [ "$ANWSER" = "Yes" ]
then
	"$PREFS_DIR"/KeyCursor &
fi
