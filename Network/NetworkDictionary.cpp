#include "NetworkDictionary.h"
#include <fstream>
#include "imgui.h"
#include "libs/imgui/misc/cpp/imgui_stdlib.h"
namespace TealEngine
{
	const char netLabelsSaveFilePath[] = "./netlabels";
	void NetworkDictionary::save() const 
	{
		std::ofstream save(netLabelsSaveFilePath,  std::ios::binary);
		for(const std::string& label : labels)
		save.write(label.c_str(), label.length() + 1);
	};
	void NetworkDictionary::load() 
	{
		labels.clear();
		std::ifstream load(netLabelsSaveFilePath);
		for(std::string label; std::getline(load, label, '\0');)
			labels.push_back(label);
	};
	int NetworkDictionary::getNetIdOf(const std::string& label) const 
	{
		for(int i = 0; i < labels.size(); i++) 
		{
			if(labels[i] == label) return i;
		}
		return -1;
	};
	void NetworkDictionary::addLabel(const std::string& label) 
	{
		labels.push_back(label);
		save();
	};
	void NetworkDictionary::display() 
	{
		for(int i = 0; i < labels.size(); i++) 
		{
			std::string inputLabel = ": " + std::to_string(i);
			if(ImGui::InputText(inputLabel.c_str(), &labels[i])) 
			{
				save();
			}
		}
		if(ImGui::Button("New label")) 
		{
			this->addLabel("Unnamed");
		}
	};
} // namespace TealEngine
