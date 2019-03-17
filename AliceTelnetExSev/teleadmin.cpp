#include <stdio.h>
#include <Winsock2.h> //windows socket的头文件

#include <iostream>

#include "compack.h"
#pragma comment( lib, "ws2_32.lib" )// 链接Winsock2.h的静态库文件


DWORD WINAPI AdminLogin(LPVOID lp) {
	//WSADATA wsaData;//初始化
	SOCKET RecvSocket;
	sockaddr_in RecvAddr;//服务器地址
	int Port = 3689;//服务器监听地址
	char RecvBuf[1024];//发送数据的缓冲区
	int BufLen = 1024;//缓冲区大小

	int SenderAddrSize = sizeof(SenderAddr);
	//初始化Socket
//	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//创建接收数据报的socket
	RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//将socket与制定端口和0.0.0.0绑定
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(Port);
	RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(RecvSocket, (SOCKADDR *)&RecvAddr, sizeof(RecvAddr));
	//调用Recvfrom函数在绑定的socket上接收数据
//	printf("receiving datagrams...\n");
	while (1) {
		int nbSize = recvfrom(RecvSocket, RecvBuf, BufLen, 0, (SOCKADDR *)&SenderAddr, &SenderAddrSize);
		if (nbSize == -1) {
			break;
		}
		teleBuffUDP = RecvBuf;
		Lastdata = TRUE;
		SendAddrsetup = TRUE;
	}
	//关闭socket，结束接收数据
	//printf("finished receiving,closing socket..\n");
	closesocket(RecvSocket);
	WSACleanup();
	MessageBoxA(0, "ERROR_ADMINLOGIN", "", 0);
	exit(0);
	//释放资源，退出
	//printf("Exiting.\n");

	return 0;
}

int teleprintf(const char *pchar) {
	SOCKET SendSocket;
//	sockaddr_in RecvAddr;//服务器地址
	int Port = 3689;//服务器监听地址
	//char SendBuf[1024];//发送数据的缓冲区
//	int BufLen = 1024;//缓冲区大小
	//初始化Socket
	//WSAStartup(MAKEWORD(2, 2), &wsaData);
	//创建Socket对象
	SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//设置服务器地址
	//RecvAddr.sin_family = AF_INET;
	//RecvAddr.sin_port = htons(Port);
	//RecvAddr.sin_addr.s_addr = inet_addr("192.168.137.1");
	//向服务器发送数据报
//	printf("Sending a datagram to the receiver...\n");
	sendto(SendSocket, pchar, strlen(pchar), 0, (SOCKADDR *)&SenderAddr, sizeof(SenderAddr));
	//发送完成，关闭Socket
//	printf("finished sending,close socket.\n");
	closesocket(SendSocket);
//	printf("Exting.\n");
	WSACleanup();
	return 0;
}

int telegetline(string &str) {
	while (TRUE) {
		Sleep(10);

		if (Lastdata)
			break;
	}

	Lastdata = FALSE;
	str = teleBuffUDP;

	return 0;
}