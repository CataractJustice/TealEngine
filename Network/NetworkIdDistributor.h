#include "GameNode/Component.h"
namespace TealEngine
{
	class NetworkIdDistributor : public Component
	{
		private:

		public:
		int id;
		
		virtual void onHostPeerConnect(PeerInfo& peer) override;
		virtual void onHostPeerPacket(PeerInfo& peer, const void* data, int size) override;
	};
} // namespace TealEngine
