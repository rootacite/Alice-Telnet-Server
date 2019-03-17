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
	wVersionRequested = MAKEWORD(1, 1);//第一个参数为低位字节；第二个参数为高位字节 	
	err = WSAStartup(wVersionRequested, &wsaData);//对winsock DLL（动态链接库文件）进行初始化，协商Winsock的版本支持，并分配必要的资源。	
	if (err != 0) { return 0; }
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)//LOBYTE（）取得16进制数最低位；HIBYTE（）取得16进制数最高（最左边）那个字节的内容		
	{
		WSACleanup();		return 0;
	}
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);//创建socket。AF_INET表示在Internet中通信；SOCK_STREAM表示socket是流套接字，对应tcp；0指定网络协议为TCP/IP
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);	//htonl用来将主机字节顺序转换为网络字节顺序(to network long)	//INADDR_ANY就是指定地址为0.0.0.0的地址,	//表示不确定地址,或“任意地址”。”
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(3689);//htons用来将主机字节顺序转换为网络字节顺序(to network short)

	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));//将本地地址绑定到所创建的socket上，以使在网络上标识该socket 	
	listen(sockSrv, 5);//socket监听，准备接受连接请求。 
	SOCKADDR_IN addrClient;

	int len = sizeof(SOCKADDR);
	//调用Recvfrom函数在绑定的socket上接收数据
//	printf("receiving datagrams...\n");
	while (1) {
		 sockConnadmin = (SOCKET*)malloc(sizeof(SOCKET));
		*sockConnadmin = accept(sockSrv, (SOCKADDR*)&addrClient, &len);
		SendAddrsetup = TRUE;
		while (1) {
			char buff[1024];
			int ret = recv(*sockConnadmin, (char*)&buff, 1024, 0);

			if (ret == SOCKET_ERROR ) {
				//teleprintf("连接中断\n");
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
	//关闭socket，结束接收数据
	//printf("finished receiving,closing socket..\n");
	//closesocket(RecvSocket);
	WSACleanup();
	MessageBoxA(0, "ERROR_ADMINLOGIN", "", 0);
	exit(0);
	//释放资源，退出
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