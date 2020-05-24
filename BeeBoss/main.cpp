#include "utils.h"
#include "comm.h"

#define ENCRYPTION_KEY 4
#define IS_RECEIVER_DEBUG false
Comm c;

void runserver()
{
	// Start Server Daemon
	c.startServer(27015);
	printf("Server Started........\n");
	while (TRUE)
	{
		char rec[32];
		c.recvData(rec, 32, true);

		if (strcmp(rec, "FileSend") == 0)
		{
			c.fileReceive();
			printf("File Received.........\n");
		}
		if (strcmp(rec, "EndConnection") == 0) {
			break;
			printf("Connection Ended......\n");
		}
	}
	// Disconnect client
	c.closeConnection();
}

void runclient(char* ip, std::vector <std::wstring > fpath)
{
	char rec[32] = "";
	// Connect To Server
	c.connect2Server(ip, 27015);
	printf("Connected to server...\n");
	// Sending File
	for (auto i : fpath) {
		c.sendData((char*)"FileSend", 9, true);
		c.fileSend((wchar_t*)i.c_str());
		printf("File Sent.............\n");
	}
	// Send Close Connection Signal
	c.sendData((char*)"EndConnection", true);
	printf("Connection ended......\n");
}

int main() {
	setlocale(LC_ALL, "rus");
	c = Comm();

	if (!IS_RECEIVER_DEBUG) {
		std::vector<std::wstring> droppedFiles = getDroppedFiles();

		for (auto i : droppedFiles)
			std::wcout << i << std::endl;

		runclient((char*)"127.0.0.1", droppedFiles);
	}
	else
		runserver();

	return 0;
}