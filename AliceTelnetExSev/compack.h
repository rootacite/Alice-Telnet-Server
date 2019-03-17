#pragma once

#include "comdata.h"

using namespace std;

int ReleData(string &input, SOCKET*remote) {
	MESSAGEDATA remdata;

	if (input[0] != '#') {
		if (input[0] == '@') {
			remdata.uId = DATA_COMMAND_HIDE;
			input.erase(0,1);
			strcpy(remdata.data, input.c_str());
			remdata.size = input.length();

			send(*remote, (char*)& remdata, sizeof(MESSAGEDATA), 0);
		}
		else {
			remdata.uId = DATA_COMMAND;
			strcpy(remdata.data, input.c_str());
			remdata.size = input.length();
			send(*remote, (char*)& remdata, sizeof(MESSAGEDATA), 0);
		}
	}

	if (input[0] == '#') {
		if (input[1] == 'g') {
			teleprintf("请输入文件名：\n");
			string tfs = "";
			telegetline( tfs);

			remdata.uId = FILE_GET;
			strcpy(remdata.data, tfs.c_str());
			remdata.size = 0;

			send(*remote, (char*)& remdata, sizeof(MESSAGEDATA), 0);

			MESSAGEDATA fdatarem;

			int ret = recv(*remote, (char*)&fdatarem, sizeof(MESSAGEDATA), 0);
			if (fdatarem.uId == FILE_ERROR) {
				teleprintf("远程文件不存在！\n");
				return -1;
			}

			HANDLE hFile = CreateFileA(tfs.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);


			while (TRUE) {
				MESSAGEDATA localdata, remotedata;

				localdata.uId = FILE_DATA;
				send(*remote, (char*)&localdata, sizeof(MESSAGEDATA), 0);

				recv(*remote, (char*)&remotedata, sizeof(MESSAGEDATA), 0);

				if (remotedata.uId == FILE_DATA) {
					DWORD nSize;
					WriteFile(hFile, remotedata.data, remotedata.size, &nSize, NULL);
					teleprintf("ondata");
					continue;
				}
				if (remotedata.uId == FILE_FIN) {
					DWORD nSize;
					WriteFile(hFile, remotedata.data, remotedata.size, &nSize, NULL);
					CloseHandle(hFile);

					teleprintf("finish...\n");
					return -1;
				}
			}
		}
		if (input[1] == 's') {
			teleprintf("请输入文件名：\n");
			string tfs = "";
			telegetline( tfs);

			HANDLE hFile = CreateFileA(tfs.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE) {
				teleprintf("文件不存在！\n");
				return -1;
			}
			remdata.uId = FILE_STARTUP;
			strcpy(remdata.data, tfs.c_str());
			remdata.size = 0;

			send(*remote, (char*)& remdata, sizeof(MESSAGEDATA), 0);

			MESSAGEDATA fdatarem;

			int ret = recv(*remote, (char*)&fdatarem, sizeof(MESSAGEDATA), 0);
			if (fdatarem.uId == FILE_ERROR) {
				teleprintf("远程文件建立失败！\n");
				return -1;
			}

			while (TRUE) {
				DWORD dwRead;
				char RemBuff[4096];
				ReadFile(hFile, RemBuff, 4096, &dwRead, NULL);

				if (dwRead < 4096) {
					MESSAGEDATA filedatarew;
					filedatarew.uId = FILE_FIN;
					memcpy(filedatarew.data, RemBuff, dwRead);
					filedatarew.size = dwRead;

					send(*remote, (char*)& filedatarew, sizeof(MESSAGEDATA), 0);
					teleprintf("dfinata\n");

					CloseHandle(hFile);
					break;
				}
				else {
					MESSAGEDATA filedatarew;
					filedatarew.uId = FILE_DATA;
					memcpy(filedatarew.data, RemBuff, dwRead);
					filedatarew.size = dwRead;

					send(*remote, (char*)& filedatarew, sizeof(MESSAGEDATA), 0);

					MESSAGEDATA locdata;

					int ret = recv(*remote, (char*)&locdata, sizeof(MESSAGEDATA), 0);
					teleprintf("data\n");
					//	CloseHandle(hFile);
				}
			}

		}
	}
	return 0;
}