#include "System/Input.h"
#include "TextInput.h"
#include "MonospaceBitmapText.h"
#include "GameNode/Properties/ComponentRefProp.h"

namespace TealEngine 
{
	void TextInput::onClick(unsigned short button) 
	{
		Input::Keyboard::focusCharInput(this);
	}
	void TextInput::onChar(unsigned int unicode) 
	{
		if(Input::Keyboard::getFocusedCharInput() == this) 
		{
			unsigned char c = unicode;
			this->text += c;
			this->textDisplay->setText(this->text);
		}
	}

	TextInput::TextInput() 
	{
		this->addProp(new ComponentRefProp(&textDisplay), "Text display");
		this->addProp(new StringProp(&text), "Text");
		this->addProp(new IntProp(&cursorBlinkDelay), "Cursor blink delay");
		this->cursorBlinkCounter = 0;
		this->cursorBlinkDelay = 25;
		this->cursorBlinkState = false;
		this->textDisplay = nullptr;
	}

	void TextInput::onKeyPress(unsigned int key) 
	{
		if(Input::Keyboard::getFocusedCharInput() != this) return;
		if(key == GLFW_KEY_BACKSPACE) 
		{
			this->text = this->text.substr(0, std::max(0, (int)text.length() - 1));
			this->textDisplay->setText(this->text);
		}
	}
	void TextInput::onKeyRepeat(unsigned int key) 
	{
		onKeyPress(key);
	}
	void TextInput::update() 
	{
		if(Input::Keyboard::getFocusedCharInput() != this) 
		{
			if(cursorBlinkState) this->textDisplay->setText(this->text);
			return;
		}
		cursorBlinkCounter++;
		if(cursorBlinkDelay < cursorBlinkCounter) 
		{
			cursorBlinkCounter = 0;
			cursorBlinkState = !cursorBlinkState;
			this->textDisplay->setText(this->text + (cursorBlinkState ? "|" : ""));
		}
	} 
}