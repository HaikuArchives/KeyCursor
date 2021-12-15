KeyCursor version 1.1
(original version) by Nathan Schrenk,
originally posted on BeBits by Eugenia Loli-Queru.
(this version) by Oscar Lesta <bipolar@softhome.net>.

What is this?
It's just an update to Nathan Schrenk's mighty KeyCursor input_server add-on.
Read his BeNewsLetter article to find more (included as article.txt).

This one does all what the original version did (let you simulate a mouse using the keyboard) plus:

- Now you have a little preferences app that allows you to:
	- choose which keys will toggle KeyCursor On/Off
	- configure the acceleration factor
	- disable KeyCursor usage, so you don't turn it on by accident.

- PageUp and PageDown act as a vertical mouse wheel when KeyCursor its active.
Just try it on Net+, it gives you a smooth scroll effect, kinda. I find it very useful to read the BeBook, mouseless :-)

Basic usage:
Hit and release the toggle key(s) to activate KeyCursor, then use the arrow keys to move the cursor.

The space bar key acts as the primary button, and if you press SHIFT while "clicking" with the space key, you get a secondary button event.

Hit again the toggle key(s) to deactivate it.

How to (un) install it?
Use the provided installer/uninstaller.

What else do I need to know?
The Preflet only allows you to configure the "toggle-keys" and not the "click" or "move" keys. That's because the laters are just fine for me. The source code it's included, just in case you want to make it more complete.

Interaction with other input filters: works OK with InstantClick and TwoWheels (the ones I use). Your milleage will vary.

To build from sources, just type make at the shell, then use the install scripts.

As requested by previous mantainer, I'll let you know where to get the older versions:
Eugenia's version:
http://bezip.de/app/777/

Nathan's (original) version:
http://chaosdna.com/mirror/ftp.be.com/samples/input_server/KeyCursor.zip

Thanks to:
Nathan Schrenk - for the original version, quite handy when things go wrong with your  input_server device hacking efforts.
Thomas Thiriez - I used part of his EasyMove input_server filter addon and prefs app.
OBOS devs - I used some bytes from their cvs too.
Eugenia - for transfering the app to me.

Newer versions, if any, will be available here: http://www.bebits.com/apps/1941/

---
Oscar, 16 April 2004.