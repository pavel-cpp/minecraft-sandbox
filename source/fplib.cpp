#include "../headers/fplib.h"

using ul = unsigned long;
using ull = unsigned long long;

typedef struct _StartupInfoFP
{
	FakePlayer* fp;
	sockaddr_in srv;
	char nick[3 + 16];
} StartupInfoFP;

int getsizestr(const char* str)
{
	int iter = 0;
	while (str[iter] != '\0') ++iter;
	return iter;
}

const char ru_ru[] = { 12, 0x04, 0x05, 'r','u','_','r','u', 0x08, 0x00, 0x01, 0x7F, 0x01 };
const char brand[] = { 18, 0x09, 0x08, 'M','C', '|', 'B', 'r','a','n','d', 7, 'v', 'a', 'n', 'i', 'l', 'l', 'a' };
const char ping[] = { 2, 0x00, 0x01 };

const char ru_ru2[] = { 13, 0x00, 0x04, 0x05, 'r','u','_','r','u', 0x08, 0x00, 0x01, 0x7F, 0x01 };
const char brand2[] = { 19, 0x00, 0x09, 0x08, 'M','C', '|', 'B', 'r','a','n','d', 7, 'v', 'a', 'n', 'i', 'l', 'l', 'a' };
const char ping2[] = { 3, 0x00, 0x00, 0x01 };

DWORD bot(StartupInfoFP *_si)
{
    StartupInfoFP si;
    si.fp = _si->fp;
    si.srv = _si->srv;
    memcpy(si.nick, _si->nick, 19);
	while (true)
	{
		SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (s == INVALID_SOCKET)
		{
			si.fp->s = INVALID_SOCKET;
			return -1;
		}
		si.fp->s = s;

        const DWORD send_timeout = 60000;
		setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (const char*)&send_timeout, sizeof(send_timeout));

        const DWORD recv_timeout = 60000;
		setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&recv_timeout, sizeof(recv_timeout));

		do
		{
			while (connect(s, (sockaddr*)&si.srv, sizeof(sockaddr)) == SOCKET_ERROR);

			if (si.fp->bungeecord)
			{
				byte join[] = { 53, 0, 0xD4, 0x2, 46, 0x0, '1', '2', '7', '.', '0', '.', '0', '.', '1', 0x0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (byte)(si.srv.sin_port % 256), (byte)(si.srv.sin_port / 256), 0x2 };

				for (int i = 16; i < 48; ++i)
                    join[i] = rand() % 10 + '0';

				send(s, (const char*)join, sizeof(join), 0);
			}
			else
			{
				byte join[] = { 16, 0, 0xD4, 0x2, 9, '1', '2', '7', '.', '0', '.', '0', '.', '1', (byte)(si.srv.sin_port % 256), (byte)(si.srv.sin_port / 256), 0x2 };
				send(s, (const char*)join, sizeof(join), 0);
			}

			send(s, si.nick, 1 + si.nick[0], 0);

			char buf[65536];
			int len;
			recv(s, buf, sizeof(buf), 0);
			recv(s, buf, sizeof(buf), 0);

			if (si.fp->compression)
			{
				recv(s, buf, sizeof(buf), 0);

				send(s, ru_ru2, sizeof(ru_ru2), 0);
				send(s, brand2, sizeof(brand2), 0);
				send(s, ping2, sizeof(ping2), 0);

				while (true)
				{
					len = recv(s, buf, sizeof(buf), 0);
					if (len == 7)
					{
						if (buf[0] == 6)
							if (buf[2] == 0x11)
							{
								buf[2] = 0x05;
								if (send(s, buf, 7, 0) != 7)
									break;
							}
					}
					else if (len == 11)
					{
						if (buf[0] == 10)
							if (buf[2] == 0x1F)
							{
								buf[2] = 0x0B;
								if (send(s, buf, 11, 0) != 11)
									break;
							}
					}
					else if (len <= 0)
						break;
				}
			}
			else
			{
				send(s, ru_ru, sizeof(ru_ru), 0);
				send(s, brand, sizeof(brand), 0);
				send(s, ping, sizeof(ping), 0);

				while (true)
				{
					len = recv(s, buf, sizeof(buf), 0);
					if (len == 6)
					{
						if (buf[0] == 5)
							if (buf[1] == 0x11)
							{
								buf[1] = 0x05;
								if (send(s, buf, 6, 0) != 6)
									break;
							}
					}
					else if (len == 10)
					{
						if (buf[0] == 9)
							if (buf[1] == 0x1F)
							{
								buf[1] = 0x0B;
								if (send(s, buf, 10, 0) != 10)
									break;
							}
					}
					else if (len <= 0)
						break;
				}
			}
		} while (false);
		closesocket(s);
	}
}

bool CreateFP(const char* nick, FakePlayer* fp, const char* ip, u_short port)
{
	StartupInfoFP si;
	si.fp = fp;

	si.srv.sin_family = AF_INET;
	si.srv.sin_port = htons(port);

    si.srv.sin_addr.S_un.S_addr = inet_addr(ip);

	if ((si.nick[2] = getsizestr(nick)) > 16)
		return true;

	si.nick[0] = si.nick[2] + 2;
	si.nick[1] = 0x00;
	memcpy(si.nick + 3, nick, si.nick[2]);

	if ((fp->t = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)bot, &si, 0, 0)) == NULL)
		return true;

	Sleep(1);
	return false;
}

bool SendCommandFP(const char* cmd, FakePlayer* fp)
{
	char pack[MAX_PATH];

	if (fp->compression)
	{
		if ((pack[3] = getsizestr(cmd)) > 256)
			return true;

		pack[0] = 3 + pack[3];
		pack[1] = 0x00;
		pack[2] = 0x02;
		memcpy(pack + 4, cmd, pack[3]);
	}
	else
	{
		if ((pack[2] = getsizestr(cmd)) > 256)
			return true;

		pack[0] = 2 + pack[2];
		pack[1] = 0x02;
		memcpy(pack + 3, cmd, pack[2]);
	}

	return send(fp->s, pack, 1 + pack[0], 0) != 1 + pack[0];
}

bool DestroyFP(FakePlayer* fp)
{
	if (TerminateThread(fp->t, 0) == FALSE)
		return true;

	closesocket(fp->s);
	return false;
}
