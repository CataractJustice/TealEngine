
namespace TealEngine
{
	class Server
	{
	private:
		/* data */
	public:
		struct ServerProperties
		{
			int port;
		};

		struct ComponentPacketHeader
		{
			int networkId;
		};
		
		Server(ServerProperties props);
		~Server();
		void onComponentPacket(void* packet, int size);
		void sendToComponent(int componentNetworkId, void* message, int size);
	};
	
} // namespace TealEngine
