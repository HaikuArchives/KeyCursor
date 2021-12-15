#include <CheckBox.h>

class ModifierView : public BCheckBox
{
public:
		ModifierView(BRect frame, uint32 modifierMask, const char* label,
					bool initialValue, BMessage* msg);
		~ModifierView();
	uint32 GetMask();

private:
	uint32 fModifierMask;
};
