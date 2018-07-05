#pragma once

#define MINPACKETLEN 5

extern SOCKET mySock;
extern bool bLogRecv;
extern bool bLogSend;

// HOOKS

// winsock recv
extern int (WINAPI *precv)(SOCKET socket, char* buffer, int length, int flags);
int WINAPI MyRecv(SOCKET socket, char* buffer, int length, int flags);

// winsock send
extern int (WINAPI *psend)(SOCKET socket, const char* buffer, int length, int flags);
int WINAPI MySend(SOCKET socket, const char* buffer, int length, int flags);