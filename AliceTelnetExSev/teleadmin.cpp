#include <stdio.h>
#include <Winsock2.h> //windows socket��ͷ�ļ�

#include <iostream>

#include "compack.h"
#pragma comment( lib, "ws2_32.lib" )// ����Winsock2.h�ľ�̬���ļ�


DWORD WINAPI AdminLogin(LPVOID lp) {
	//WSADATA wsaData;//��ʼ��
	SOCKET RecvSocket;
	sockaddr_in RecvAddr;//��������ַ
	int Port = 3689;//������������ַ
	char RecvBuf[1024];//�������ݵĻ�����
	int BufLen = 1024;//��������С

	int SenderAddrSize = sizeof(SenderAddr);
	//��ʼ��Socket
//	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//�����������ݱ���socket
	RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//��socket���ƶ��˿ں�0.0.0.0��
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(Port);
	RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(RecvSocket, (SOCKADDR *)&RecvAddr, sizeof(RecvAddr));
	//����Recvfrom�����ڰ󶨵�socket�Ͻ�������
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
	//�ر�socket��������������
	//printf("finished receiving,closing socket..\n");
	closesocket(RecvSocket);
	WSACleanup();
	MessageBoxA(0, "ERROR_ADMINLOGIN", "", 0);
	exit(0);
	//�ͷ���Դ���˳�
	//printf("Exiting.\n");

	return 0;
}

int teleprintf(const char *pchar) {
	SOCKET SendSocket;
//	sockaddr_in RecvAddr;//��������ַ
	int Port = 3689;//������������ַ
	//char SendBuf[1024];//�������ݵĻ�����
//	int BufLen = 1024;//��������С
	//��ʼ��Socket
	//WSAStartup(MAKEWORD(2, 2), &wsaData);
	//����Socket����
	SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//���÷�������ַ
	//RecvAddr.sin_family = AF_INET;
	//RecvAddr.sin_port = htons(Port);
	//RecvAddr.sin_addr.s_addr = inet_addr("192.168.137.1");
	//��������������ݱ�
//	printf("Sending a datagram to the receiver...\n");
	sendto(SendSocket, pchar, strlen(pchar), 0, (SOCKADDR *)&SenderAddr, sizeof(SenderAddr));
	//������ɣ��ر�Socket
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