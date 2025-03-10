#include "RadioButtonGroup.h"


using namespace NAS2D;


RadioButtonGroup::RadioButton::RadioButton(RadioButtonGroup* parentContainer, std::string newText, NAS2D::Delegate<void()> delegate) :
	mFont{fontCache.load(constants::FONT_PRIMARY, constants::FontPrimaryNormal)},
	mSkin{imageCache.load("ui/skin/radio.png")},
	mLabel{newText},
	mParentContainer{parentContainer}
{
	text(newText);
	mSignal.connect(delegate);
	Utility<EventHandler>::get().mouseButtonDown().connect(this, &RadioButtonGroup::RadioButton::onMouseDown);
	onTextChange();
}


RadioButtonGroup::RadioButton::~RadioButton()
{
	Utility<EventHandler>::get().mouseButtonDown().disconnect(this, &RadioButtonGroup::RadioButton::onMouseDown);
}


/**
 * Sets checked state.
 *
 * \note	Setting check state with this method will not fire
 *			a clicked event.
 */
void RadioButtonGroup::RadioButton::checked(bool toggle)
{
	mChecked = toggle;
}


bool RadioButtonGroup::RadioButton::checked() const
{
	return mChecked;
}


void RadioButtonGroup::RadioButton::text(const std::string& text)
{
	mLabel.text(text);
	onTextChange();
}


const std::string& RadioButtonGroup::RadioButton::text() const
{
	return mLabel.text();
}


void RadioButtonGroup::RadioButton::draw() const
{
	auto& renderer = Utility<Renderer>::get();

	const auto unselectedIconRect = NAS2D::Rectangle{0, 0, 13, 13};
	const auto selectedIconRect = NAS2D::Rectangle{13, 0, 13, 13};

	renderer.drawSubImage(mSkin, position(), (mChecked ? selectedIconRect : unselectedIconRect));
	renderer.drawText(mFont, text(), position() + NAS2D::Vector{20, 0}, NAS2D::Color::White);
}


/**
 * Enforces minimum and maximum sizes.
 */
void RadioButtonGroup::RadioButton::onResize()
{
	mRect.size({std::max(mRect.width, 13), 13});
}


void RadioButtonGroup::RadioButton::onTextChange()
{
	const auto textWidth = mFont.width(text());
	width((textWidth > 0) ? 20 + textWidth : 13);
}


void RadioButtonGroup::RadioButton::onMouseDown(EventHandler::MouseButton button, int x, int y)
{
	if (!enabled() || !visible()) { return; }

	if (button == EventHandler::MouseButton::Left && mRect.contains(Point{x, y}))
	{
		mParentContainer->select(*this);
		mSignal();
	}
}
