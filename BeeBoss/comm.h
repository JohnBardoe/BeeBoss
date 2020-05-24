#include "sock_header.h"

class Comm
{

public:
	
	Comm();
	~Comm();

	void connect2Server(char* ip, int port);
	int sendData(char* sendbuf, bool confirmation = false);
	int recvData(char* recvbuf, int size, bool confirmation = false);
	void fileSend(wchar_t* fpath);
	void fileReceive();
	void startServer(int port);
	void closeConnection();
};