#pragma once
#include "BindingManager.h"

namespace Kirino
{
	class UBOBindings : public BindingManager 
	{
		private:
		void bindImpl(unsigned int index, unsigned int to) override;
		public:
		UBOBindings();
	};
} // namespace Kirino
