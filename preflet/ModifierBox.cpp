#include "ModifierBox.h"
#include "ModifierView.h"

////////////////////////////////////////////////////////////////////////////////

const ModifierBox::MaskListType ModifierBox::ModifierList[]=
{
	{B_SHIFT_KEY,	"Shift"},
	{B_CONTROL_KEY,	"Control"},
	{B_COMMAND_KEY,	"Command"},
	{B_OPTION_KEY,	"Option"},
	{B_MENU_KEY,	"Menu"}
};

////////////////////////////////////////////////////////////////////////////////

ModifierBox::ModifierBox(BRect frame, const char* label, uint32 modifier,
						uint32 button)
	: BBox(frame),
	fDefaultModifier(0)
{
	if (label) SetLabel(label);

	float right = 0;
	float bottom;
	BRect frame(10, 15, 10, 15);

	fModifierViewList = new ModifierView*[5];

	BFont f = be_plain_font;
	float width = f.StringWidth("Command");
	width += 20;

	fModifierViewList[0] = new ModifierView(BRect(10, 15, 10 + width, 15), ModifierList[0].mask, ModifierList[0].label, ModifierList[0].mask & modifier, new BMessage(MODIFIER_CHANGED));
	fModifierViewList[1] = new ModifierView(BRect(10, 35, 10 + width, 45), ModifierList[1].mask, ModifierList[1].label, ModifierList[1].mask & modifier, new BMessage(MODIFIER_CHANGED));
	fModifierViewList[2] = new ModifierView(BRect(10, 55, 10 + width, 65), ModifierList[2].mask, ModifierList[2].label, ModifierList[2].mask & modifier, new BMessage(MODIFIER_CHANGED));
	fModifierViewList[3] = new ModifierView(BRect(15 + width, 15, 75 + width, 15), ModifierList[3].mask, ModifierList[3].label, ModifierList[3].mask & modifier, new BMessage(MODIFIER_CHANGED));
	fModifierViewList[4] = new ModifierView(BRect(15 + width, 35, 75 + width, 45), ModifierList[4].mask, ModifierList[4].label, ModifierList[4].mask & modifier, new BMessage(MODIFIER_CHANGED));

	for (int32 i = 0; i < 5; i++)
	{
		fModifierViewList[i]->ResizeToPreferred();
		AddChild(fModifierViewList[i]);

		right = max_c(right, fModifierViewList[i]->Frame().right);
		frame.OffsetTo(frame.left, fModifierViewList[i]->Frame().bottom + 5);
	}

	bottom = fModifierViewList[5 - 1]->Frame().bottom;
	ResizeTo(right + 10, bottom + 30);
}

ModifierBox::~ModifierBox()
{
	delete fModifierViewList;
}

void ModifierBox::MessageReceived(BMessage* message)
{
	uint32 mask = 0;

	switch(message->what)
	{
		case MODIFIER_CHANGED:
			for (int32 i = 0; i < 5; i++)
				mask |= fModifierViewList[i]->GetMask();
			if (!mask)
			{
				mask = fDefaultModifier;
				for (int32 i = 0; i < 5; i++)
					fModifierViewList[i]->SetValue(ModifierList[i].mask & mask);
			}
			InvokeNotify(Message());
		break;

		default:
			BBox::MessageReceived(message);
	}
}

void ModifierBox::AttachedToWindow()
{
	for (int32 i = 0; i < 5; i++)
		fModifierViewList[i]->SetTarget(this);
}

////////////////////////////////////////////////////////////////////////////////

uint32 ModifierBox::GetModifierMask()
{
	uint32 mask = 0;

	for (int32 i = 0; i < 5; i++)
		mask |= fModifierViewList[i]->GetMask();

	return mask;
}

void ModifierBox::SetModifierMask(uint32 mask)
{
	for (int32 i = 0; i < 5; i++)
		fModifierViewList[i]->SetValue(ModifierList[i].mask & mask);
}

void ModifierBox::SetDefaultModifierMask(uint32 mask)
{
	fDefaultModifier = mask;
}

void ModifierBox::SetEnabled(bool enabled)
{
	for (int i = 0; i < 5; i++)
		fModifierViewList[i]->SetEnabled(enabled);

	fControlsEnabled = enabled;
}

////////////////////////////////////////////////////////////////////////////////