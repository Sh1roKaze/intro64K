#pragma once
#include "windows.h"
#include <cstdio>


char *initMIDIfile();
DWORD playMIDIFile(HWND hWndNotify, LPSTR lpszMIDIFileName);