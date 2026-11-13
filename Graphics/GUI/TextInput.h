#include "GameNode/Component.h"
namespace TealEngine
{
	class MonospaceBitmapText;
	class TextInput : public Component 
	{
		private:
		std::string text;
		int cursorBlinkCounter;
		bool cursorBlinkState;
		public:
		int cursorBlinkDelay;
		MonospaceBitmapText* textDisplay;
		void onClick(unsigned short button) override;
		void onChar(unsigned int unicode) override;
		void onKeyPress(unsigned int key) override;
		void onKeyRepeat(unsigned int key) override;
		void update() override;
		inline const std::string& getInputText() const { return text; }
		TextInput();
	};
} // namespace TealEngine
