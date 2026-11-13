#pragma once
namespace Kirino
{
	class BindingManager 
	{
		private:
		unsigned int* bindings;
		unsigned int max;
		virtual void bindImpl(unsigned int index, unsigned int to) = 0;
		public:
		BindingManager(unsigned int maxBindings);
		~BindingManager();
		void bind(unsigned int index, unsigned int to);
		void reset();
	};
} // namespace Kirino
