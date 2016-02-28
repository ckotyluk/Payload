#include <stdint.h>
#include <string>

using namespace std;

class Network{
    public:
		Network(/*uint32_t*/ string ip, /*uint8_t*/ string port, bool server);
		void SendPacket(uint32_t packet);
    private:
		void ServerWorker();
		void ClientWorker();
		/*uint32_t*/ string myIP;
		/*uint8_t*/  string myPort;	
		uint32_t mySendPacket;
		bool isServer; // True if server, false if client
};
