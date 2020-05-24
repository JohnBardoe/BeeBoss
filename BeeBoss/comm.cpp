#include "comm.h"
#include "utils.h"

#include <fstream>

WSADATA wsaData;
SOCKET m_socket;
sockaddr_in this_con, other_con;
SOCKET AcceptSocket;

//TODO: handle errors when sending/receiving
//handle OK reply

Comm::Comm()
{

    int iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
    if ( iResult != NO_ERROR )
        printf("Error at WSAStartup()\n");

    m_socket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

    if ( m_socket == INVALID_SOCKET ) {
        printf( "Error at socket(): %ld\n", WSAGetLastError() );
        WSACleanup();
        return;
    }

}


Comm::~Comm(){WSACleanup();}

void Comm::connect2Server(char *ip,int port)
{
    other_con.sin_family = AF_INET;
	other_con.sin_addr.s_addr = inet_addr( ip );
	other_con.sin_port = htons( port );
	/*
    if ( connect( m_socket, (SOCKADDR*) &con, sizeof(con) ) == SOCKET_ERROR) {
        printf( "Failed to connect.\n" );
        WSACleanup();
        return;
    }
	*/
	//ADD CONNECTION CHECK
}


void Comm::startServer(int port)
{
    this_con.sin_family = AF_INET;
	this_con.sin_addr.s_addr = INADDR_ANY;
    this_con.sin_port = htons( port );

    if ( bind( m_socket, (SOCKADDR*) &this_con, sizeof(this_con) ) == SOCKET_ERROR) {
        printf( "Failed to connect.\n" );
        WSACleanup();
        return;
    }

}


int Comm::sendData(char *sendbuf, bool confirmation)
{
	int size;
	if ((size = sendto(m_socket, sendbuf, strlen(sendbuf), 0, reinterpret_cast<SOCKADDR*>(&other_con), sizeof(other_con))) == SOCKET_ERROR) {
		std::cout <<"Error in sendData: "<< WSAGetLastError() << std::endl;
		return SOCKET_ERROR;
	}
	if (!confirmation)
		return size;
	else {
		char recBuf[32];
		recvData(recBuf, 32);
		if (strcmp(recBuf, "OK") == 0)
			return size;
		else
			return -1;
	}
}


int Comm::recvData(char *recvbuf,int size, bool confirmation)
{

	int size_sock = sizeof(other_con);
	int sz;
	if ((sz = recvfrom(m_socket, recvbuf, size, 0, reinterpret_cast<SOCKADDR*>(&other_con), &size_sock)) == SOCKET_ERROR) {
		std::cout << "Error in recvData: " << WSAGetLastError() << std::endl;
		return SOCKET_ERROR;
	}
	recvbuf[sz] = '\0';
	if (confirmation)
		sendData((char*)"OK");
	return sz;
}


void Comm::closeConnection()
{
	closesocket(m_socket);
}


void Comm::fileReceive()
{

	char rec[32];
	
	recvData(rec, 32, true);

	std::wcout << (wchar_t*)widen(rec).c_str() << std::endl;

	FILE *fw = _wfopen((wchar_t*)widen(rec).c_str(), L"a");

	int recs = recvData(rec, 32, true);

	rec[recs]='\0';
	int size = atoi(rec);
				

	while(size > 0)
	{
		char buffer[NET_BUF_SIZE + 6];

		if(size>= NET_BUF_SIZE)
		{
			recvData(buffer, NET_BUF_SIZE, true);
			fwrite(buffer, NET_BUF_SIZE, 1, fw);

		}
		else
		{
			recvData(buffer, size, true);
			buffer[size]='\0';
			fwrite(buffer, size, 1, fw);
		}


		size -= NET_BUF_SIZE;

	}

	fclose(fw);

}

void Comm::fileSend(wchar_t* fpath)
{

	// Extract only filename from given path.
	wchar_t filename[50];
	int i=wcslen(fpath);
	for(;i>0;i--)if(fpath[i-1]=='\\')break;
	for(int j=0;i<=(int)wcslen(fpath);i++)filename[j++]=fpath[i];
	////////////////////////////////////////

	std::ifstream myFile (fpath, std::ios::in| std::ios::binary| std::ios::ate);
	int size = (int)myFile.tellg();  //rewrite this pls
	myFile.close();

	char filesize[10];_itoa(size,filesize,10);
	char recvBuf[32];

	sendData((char*)narrow(filename).c_str(), true);
	sendData(filesize, true);

	
	FILE *fr = _wfopen(fpath, (const wchar_t*)"r");

	while(size > 0)
	{
		char buffer[NET_BUF_SIZE + 6];

		if(size>= NET_BUF_SIZE)
			fread(buffer, NET_BUF_SIZE, 1, fr);
		else
		{
			fread(buffer, size, 1, fr);
			buffer[size]='\0';
		}

		sendData(buffer, true);

		size -= NET_BUF_SIZE;

	}

	fclose(fr);

}



