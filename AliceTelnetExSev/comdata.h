#include <Windows.h>
#include "reloadpag.hPP"


#define DATA_COMMAND 1
#define FILE_STARTUP 2
#define FILE_DATA 3
#define FILE_GET 4
#define FILE_FIN 5
#define FILE_ERROR 6
#define DATA_REVIEW 7
#define DATA_COMMAND_HIDE 8

struct MESSAGEDATA {
	UINT uId;
	char data[4096];
	UINT size;
};