#pragma once
#include <string>
#include <vector>

namespace TealEngine
{
	class NetworkDictionary 
	{
		std::vector<std::string> labels;
		public:
		void save() const;
		void load();
		int getNetIdOf(const std::string& label) const;
		void addLabel(const std::string& label);
		void display();
		inline const std::vector<std::string>& getLabels() const { return labels; };
	};
} // namespace TealEngine
