#!/bin/sh

rm -f ~/config/non-packaged/add-ons/input_server/devices/KeyCursorDevice
rm -f ~/config/non-packaged/add-ons/input_server/filters/KeyCursorFilter

cp src/KeyCursor ~/config/non-packaged/add-ons/input_server/devices/KeyCursorDevice
ln -s ~/config/non-packaged/add-ons/input_server/devices/KeyCursorDevice ~/config/non-packaged/add-ons/input_server/filters/KeyCursorFilter

# /system/servers/input_server -q
kill -9 `pidof input_server`
sleep 1
