#pragma once

#include "Control.h"

#include "NAS2D/Renderer/Color.h"
#include "NAS2D/Resources/Font.h"

#include <string>


class TextArea : public Control
{
public:
	TextArea() = default;
	virtual ~TextArea() = default;

	void textColor(int r, int g, int b, int a = 255) { textColor(NAS2D::Color{static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b), static_cast<uint8_t>(a)}); };
	void textColor(const NAS2D::Color& color) { mTextColor = color; }

	void font(const std::string&, size_t);

	void update();

private:
	typedef std::vector<std::string> StringList;

private:
	virtual void onSizeChanged();
	virtual void onTextChanged();
	virtual void onFontChanged();

	virtual void draw();
	void processString();

private:
	size_t		mNumLines = 0;

	StringList	mFormattedList;

	NAS2D::Color	mTextColor = NAS2D::Color::White;

	NAS2D::Font*	mFont = nullptr;
};
