#include "sock_header.h"


class Comm
{

public:
	
	Comm();
	~Comm();

	void connect2Server(char*,int);
	 int sendData(char*, bool);
	 int recvData(char*,int, bool);
	void fileSend(wchar_t*);
	void fileReceive(wchar_t*);
	void startServer(int);
	void waitForClient();
	void closeConnection();
};