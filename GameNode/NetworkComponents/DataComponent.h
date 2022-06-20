#pragma once
#include <map>
#include <string>
#include "GameNode/Component.h"
#include "Network/PeerData.h"
#include "System/TStruct.h"
using namespace std;
namespace TealEngine 
{
	class ClientComponent;
	class Server;
	class DataComponent : public Component
	{
	private:
		unsigned int id;
		static unsigned int idCounter;
		TStruct* data;

	public:
		unsigned int getID();
		DataComponent(unsigned int id = 0);
		void setSchema(PreparedTStructSchema schema) 
		{
			TStruct* data;
		}
	};
}