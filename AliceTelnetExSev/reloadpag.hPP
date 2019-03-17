#pragma once

#include <string>

using namespace std;

SOCKET* sockConnadmin = NULL;
string teleBuffUDP = "";
sockaddr_in SenderAddr;

BOOL SendAddrsetup = FALSE;
BOOL Lastdata = FALSE;

DWORD WINAPI AdminLogin(LPVOID lp);

int teleprintf(const char *pchar);

int telegetline(std::string &str);



DWORD WINAPI AdminLogin(LPVOID lp) {
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(1, 1);//��һ������Ϊ��λ�ֽڣ��ڶ�������Ϊ��λ�ֽ� 	
	err = WSAStartup(wVersionRequested, &wsaData);//��winsock DLL����̬���ӿ��ļ������г�ʼ����Э��Winsock�İ汾֧�֣��������Ҫ����Դ��	
	if (err != 0) { return 0; }
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)//LOBYTE����ȡ��16���������λ��HIBYTE����ȡ��16��������ߣ�����ߣ��Ǹ��ֽڵ�����		
	{
		WSACleanup();		return 0;
	}
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);//����socket��AF_INET��ʾ��Internet��ͨ�ţ�SOCK_STREAM��ʾsocket�����׽��֣���Ӧtcp��0ָ������Э��ΪTCP/IP
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);	//htonl�����������ֽ�˳��ת��Ϊ�����ֽ�˳��(to network long)	//INADDR_ANY����ָ����ַΪ0.0.0.0�ĵ�ַ,	//��ʾ��ȷ����ַ,�������ַ������
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(3689);//htons�����������ֽ�˳��ת��Ϊ�����ֽ�˳��(to network short)

	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));//�����ص�ַ�󶨵���������socket�ϣ���ʹ�������ϱ�ʶ��socket 	
	listen(sockSrv, 5);//socket������׼�������������� 
	SOCKADDR_IN addrClient;

	int len = sizeof(SOCKADDR);
	//����Recvfrom�����ڰ󶨵�socket�Ͻ�������
//	printf("receiving datagrams...\n");
	while (1) {
		 sockConnadmin = (SOCKET*)malloc(sizeof(SOCKET));
		*sockConnadmin = accept(sockSrv, (SOCKADDR*)&addrClient, &len);
		SendAddrsetup = TRUE;
		while (1) {
			char buff[1024];
			int ret = recv(*sockConnadmin, (char*)&buff, 1024, 0);

			if (ret == SOCKET_ERROR ) {
				//teleprintf("�����ж�\n");
				break;
			}

			buff[ret] = '\0';
			teleBuffUDP = buff;
			Lastdata = TRUE;
		}
		closesocket(*sockConnadmin);
		delete sockConnadmin;

		SendAddrsetup = FALSE;
	}
	//�ر�socket��������������
	//printf("finished receiving,closing socket..\n");
	//closesocket(RecvSocket);
	WSACleanup();
	MessageBoxA(0, "ERROR_ADMINLOGIN", "", 0);
	exit(0);
	//�ͷ���Դ���˳�
	//printf("Exiting.\n");

	return 0;
}

int teleprintf(const char *pchar) {

	while (!SendAddrsetup) {
		
		//MessageBoxA(0,pchar,"",0);
		Sleep(10);
	}

	send(*sockConnadmin, pchar, strlen(pchar), 0);


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