#include <Window.h>

#include "Preferences.h"

class BView;
class BSlider;
class BCheckBox;
class ModifierView;
class ModifierBox;

class SettingsWin : public BWindow
{
public:
				SettingsWin();
				~SettingsWin();
	virtual void MessageReceived(BMessage* message);
	virtual bool QuitRequested();

private:
	void SendUpdateMessage();

	BView* bgView;
	ModifierBox* fModBox;
	BCheckBox* fEnabled;
	BSlider* fAccelSlider;

	Preferences fPrefs;
};
