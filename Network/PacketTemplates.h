#pragma once
#ifndef  PACKET_TEMPLATES
#define PACKET_TEMPLATES
#include "System/PreparedTStructSchema.h"
namespace TealEngine {
	typedef PreparedTStructSchema PacketTemplate;
	namespace PacketTemplates {
		extern PacketTemplate
			nodePacketTemplate,
			componentPacketTemplate,
			newNodePacketTemplate,
			newComponentPacketTemplate,
			removeNodePacketTemplate,
			removeComponentPacketTemplate
			;
	}
}
#endif // ! PacketTemplates