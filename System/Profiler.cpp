#include "libs/imgui/imgui.h"
#include "Profiler.h"

namespace TealEngine
{
	Profiler::ProfilerRecord::ProfilerRecord() 
	{
		average = std::chrono::nanoseconds(0);
	}

	void Profiler::ProfilerRecord::display(const std::string& label) const
	{
		
		ImGuiTreeNodeFlags treeFlags = 
		ImGuiTreeNodeFlags_AllowItemOverlap | 
		ImGuiTreeNodeFlags_OpenOnArrow | 
		ImGuiTreeNodeFlags_OpenOnDoubleClick;

		float fraction = (this->back && this->back->average.count() != 0) ? (float)this->average.count() / (float)this->back->average.count() : 0.0f;
		bool treeOpen = ImGui::TreeNodeEx(("##" + label).c_str(), treeFlags);
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f - fraction, 1.0f - fraction, 1.0f), "%s took %dmc, %d.%d%s", 
		label.c_str(), 
		(int)std::chrono::duration_cast<std::chrono::microseconds>(this->average).count()
		, (int)(fraction * 100.0f), ((int)(fraction * 10000.0f) % 100), "%");
		if(records.size() && treeOpen) 
		{
			for(const auto&[recordLabel, record] : records) 
			{
				record.display(recordLabel);
			}
		}
		if(treeOpen)
			ImGui::TreePop();
	}

	Profiler::ProfilerPoint::ProfilerPoint(Profiler* profiler, const std::string& label) 
	{
		auto it = profiler->current->records.find(label);
		ProfilerRecord* record = nullptr;
		if(it == profiler->current->records.cend()) 
		{
			record = &(profiler->current->records[label] = ProfilerRecord());
			record->back = profiler->current;
		}
		else 
		{
			record = &it->second;
		}
		profiler->current = record;
		this->profiler = profiler;
		this->enter = std::chrono::high_resolution_clock::now();
		ended = false;
	}
	Profiler::ProfilerPoint::~ProfilerPoint() 
	{
		if(!ended)
			end();
	}
	
	void Profiler::ProfilerPoint::end() 
	{
		profiler->current->enter = this->enter;
		profiler->current->exit = std::chrono::high_resolution_clock::now();
		profiler->current->average = profiler->current->average / 90 * 89 + (profiler->current->exit - profiler->current->enter) / 90;
		profiler->current = profiler->current->back;
		ended = true;
	}

	Profiler::Profiler()
	{
		initial.back = nullptr;
		current = &initial;
	}

	void Profiler::display() 
	{
		initial.display("Initial");
	}
} // namespace TealEngine
