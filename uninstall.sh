#!/bin/sh

rm ~/config/non-packaged/add-ons/input_server/devices/KeyCursorDevice
rm ~/config/non-packaged/add-ons/input_server/filters/KeyCursorFilter

# /system/servers/input_server -q
kill -9 `pidof input_server`
sleep 1
