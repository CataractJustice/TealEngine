#include "PacketTemplates.h"

namespace TealEngine
{
	namespace PacketTemplates 
	{
		PacketTemplate
			nodePacketTemplate({ "node_id", "packet" }),
			componentPacketTemplate({ "component_id", "packet" }),
			newNodePacketTemplate({ "node_id" }),
			newComponentPacketTemplate({ "component_type", "component_id", "node_id" }),
			removeNodePacketTemplate({ "node_id" }),
			removeComponentPacketTemplate({ "component_id" })
			;
	}
}