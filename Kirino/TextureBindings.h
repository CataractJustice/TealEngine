#pragma once
#include "BindingManager.h"

namespace Kirino
{
	class TextureBindings : public BindingManager 
	{
		private:
		void bindImpl(unsigned int index, unsigned int to) override; 
		public: 
		TextureBindings();
	};
} // namespace Kirino
