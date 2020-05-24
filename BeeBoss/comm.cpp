#include "comm.h"
#include "utils.h"

#include <fstream>

WSADATA wsaData;
SOCKET m_socket;
sockaddr_in con;
SOCKET AcceptSocket;

//TODO: handle errors when sending/receiving
//handle OK reply

Comm::Comm()
{

    int iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
    if ( iResult != NO_ERROR )
        printf("Error at WSAStartup()\n");

    m_socket = socket( AF_INET, SOCK_DGRAM, 0 );

    if ( m_socket == INVALID_SOCKET ) {
        printf( "Error at socket(): %ld\n", WSAGetLastError() );
        WSACleanup();
        return;
    }

}


Comm::~Comm(){WSACleanup();}

void Comm::connect2Server(char *ip,int port)
{
    con.sin_family = AF_INET;
    con.sin_addr.s_addr = inet_addr( ip );
    con.sin_port = htons( port );

    if ( connect( m_socket, (SOCKADDR*) &con, sizeof(con) ) == SOCKET_ERROR) {
        printf( "Failed to connect.\n" );
        WSACleanup();
        return;
    }
}


void Comm::startServer(int port)
{
    con.sin_family = AF_INET;
	con.sin_addr.s_addr = INADDR_ANY;
    con.sin_port = htons( port );

    if ( bind( m_socket, (SOCKADDR*) &con, sizeof(con) ) == SOCKET_ERROR) {
        printf( "Failed to connect.\n" );
        WSACleanup();
        return;
    }

}

void Comm::waitForClient()
{
		AcceptSocket = SOCKET_ERROR;
        while ( AcceptSocket == SOCKET_ERROR ) {
            AcceptSocket = accept( m_socket, NULL, NULL );
        }
}

int Comm::sendData(char *sendbuf, bool confirmation = false)
{
	int size = sendto( m_socket, sendbuf, strlen(sendbuf), 0, reinterpret_cast<SOCKADDR*>(&con), sizeof(con));
	if (!confirmation)
		return size;
	else {
		char recBuf[32];
		recvData(recBuf, 32);
		if (strcmp(recBuf, "OK"))
			return size;
		else
			return -1;
	}
}


int Comm::recvData(char *recvbuf,int size, bool confirmation = false)
{
	sockaddr_in from;
	int size_sock = sizeof(from);
	int sz = recvfrom( m_socket, recvbuf, size, 0, reinterpret_cast<SOCKADDR*>(&from), &size_sock);
	recvbuf[sz] = '\0';
	if (confirmation)
		sendData((char*)"OK");
	return sz;
}


void Comm::closeConnection()
{
	closesocket(m_socket);
}


void Comm::fileReceive(wchar_t* filename)
{

	char rec[32];
	
	recvData(rec, 32, true);

	filename = (wchar_t*)widen(rec).c_str();

	FILE *fw = _wfopen(filename, (const wchar_t*)"wb");

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

	char filesize[10];itoa(size,filesize,10);
	char recvBuf[32];

	sendData((char*)narrow(filename).c_str(), true);
	sendData(filesize, true);

	
	FILE *fr = _wfopen(fpath, (const wchar_t*)"rb");

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



