// MIDItoSTR.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include <cstdio>

// Plays a specified MIDI file by using MCI_OPEN and MCI_PLAY. Returns 
// as soon as playback begins. The window procedure function for the 
// specified window will be notified when playback is complete. 
// Returns 0L on success; otherwise, it returns an MCI error code.

DWORD playMIDIFile(HWND hWndNotify, LPCWSTR lpszMIDIFileName)
{
	UINT wDeviceID;
	DWORD dwReturn;
	MCI_OPEN_PARMS mciOpenParms;
	MCI_PLAY_PARMS mciPlayParms;
	MCI_STATUS_PARMS mciStatusParms;
	MCI_SEQ_SET_PARMS mciSeqSetParms;

	// Open the device by specifying the device and filename.
	// MCI will attempt to choose the MIDI mapper as the output port.
	mciOpenParms.lpstrDeviceType = L"sequencer";
	mciOpenParms.lpstrElementName = lpszMIDIFileName;
	if (dwReturn = mciSendCommand(NULL, MCI_OPEN,
		MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
		(DWORD)(LPVOID)&mciOpenParms))
	{
		// Failed to open device. Don't close it; just return error.
		return (dwReturn);
	}

	// The device opened successfully; get the device ID.
	wDeviceID = mciOpenParms.wDeviceID;

	// Check if the output port is the MIDI mapper.
	mciStatusParms.dwItem = MCI_SEQ_STATUS_PORT;
	if (dwReturn = mciSendCommand(wDeviceID, MCI_STATUS,
		MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatusParms))
	{
		mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
		return (dwReturn);
	}

	// The output port is not the MIDI mapper. 
	// Ask if the user wants to continue.
	if (LOWORD(mciStatusParms.dwReturn) != MIDI_MAPPER)
	{
		// User does not want to continue. Not an error;
		// just close the device and return.
		mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
		return (0L);
	}

	// Begin playback. The window procedure function for the parent 
	// window will be notified with an MM_MCINOTIFY message when 
	// playback is complete. At this time, the window procedure closes 
	// the device.
	mciPlayParms.dwCallback = (DWORD)hWndNotify;
	if (dwReturn = mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY,
		(DWORD)(LPVOID)&mciPlayParms))
	{
		mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
		return (dwReturn);
	}

	return (0L);
}

int main() {

	FILE *f = fopen("MA_Theme.mid", "rb+");
	if (f == NULL) {
		return 0;
	}
	FILE *fOut = fopen("midi.cpp", "wb");

	int length = 1;
	unsigned char c = getc(f);
	fprintf(fOut, "#include \"midi.h\";\n\nconst unsigned char main_theme[] = { 0%x", c);

	while (!feof(f)) {
		unsigned char c = fgetc(f);
		length++;
		fprintf(fOut, ", 0x%x", c);
	}
	fprintf(fOut, " };\n");
	fprintf(fOut, "\nconst unsigned int main_theme_length = %d;\n", length);
	fclose(fOut);
	fclose(f);
	return 0;
}

