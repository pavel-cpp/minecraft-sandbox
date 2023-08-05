#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#pragma comment(lib, "ws2_32")

typedef struct _FakePlayer
{
	HANDLE t;
	SOCKET s;
	bool compression;
	bool bungeecord;
} FakePlayer;

bool CreateFP(const char* nick, FakePlayer* fp, const char* ip, u_short port);
bool SendCommandFP(const char* cmd, FakePlayer* fp);
bool DestroyFP(FakePlayer* fp);
