#include "utils.h"
#include "comm.h"

#define ENCRYPTION_KEY 4

Comm c;

void runserver()
{
	// Start Server Daemon
	c.startServer(27015);
	printf("Server Started........\n");
	while (TRUE) {
		// Wait until a client connects
		printf("Client Connected......\n");

		// Work with client
		while (TRUE)
		{
			char rec[50];
			c.recvData(rec, 32, true);

			if (strcmp(rec, "FileSend") == 0)
			{
				c.fileReceive();
				printf("File Received.........\n");
			}
			if (strcmp(rec, "EndConnection") == 0)break;
			printf("Connection Ended......\n");
		}
		// Disconnect client
		c.closeConnection();
	}
}

void runclient(char* ip, wchar_t* fpath)
{
	char rec[32] = "";
	// Connect To Server
	c.connect2Server(ip, 27015);
	printf("Connected to server...\n");
	// Sending File
	c.sendData((char*)"FileSend", true);
	c.fileSend(fpath);
	printf("File Sent.............\n");
	// Send Close Connection Signal
	c.sendData((char*)"EndConnection", true);
	printf("Connection ended......\n");
}

int main() {
	setlocale(LC_ALL, "rus");
	c = Comm();
	
	std::vector<std::wstring> droppedFiles = getDroppedFiles();

	for (auto i : droppedFiles)
		std::wcout << i << std::endl;

	runclient((char*)"127.0.0.1", (wchar_t*)droppedFiles[0].c_str());
	

	//runserver();

	return 0;
}