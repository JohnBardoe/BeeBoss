#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>

#include <cstdio>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <wchar.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

const char* END_FLAG = "FUCKYOU=!=!!!=PENUSANAL";

#define NET_BUF_SIZE 1024
#define PORT_NO 15050 