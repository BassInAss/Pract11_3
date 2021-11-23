#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <iostream>
#include<windows.h>
#include "atlstr.h"

using namespace std;
int main(int argc, char* argv[])
{
    SetConsoleOutputCP(1251);

    HANDLE hEvent = CreateEvent(0, TRUE, FALSE, TEXT("ConsoleApplication1.exe"));
    if (hEvent)
    {
        if (GetLastError() != ERROR_ALREADY_EXISTS) // первичный запуск
        {
            PROCESS_INFORMATION pi[3];
            memset(pi, 0, sizeof(pi));

            WCHAR fileName[MAX_PATH] = { 0 };
            GetModuleFileName(0, fileName, MAX_PATH);

            cout << "Запуск процессов..."<< endl;
            for (int i = 0; i < 3; i++)
            {
                STARTUPINFO si;
                memset(&si, 0, sizeof(si));
                if (!CreateProcess(fileName, 0, 0, 0, FALSE, CREATE_NEW_CONSOLE, 0, 0, &si, &pi[i]))
                    printf("Ошибка создания процесса!\n");
            }

            system("pause");

            printf("Сигнал синхронизации\n");
            SetEvent(hEvent);

            printf("Ожидание завершения работы процессов...\n");
            for (int i = 0; i < 3; i++)
            {
                if (pi[i].hProcess)
                    WaitForSingleObject(pi[i].hProcess, INFINITE);
            }

            system("pause");
        }
        else // вторичный запуск
        {
            printf("Ожидание синхронизации...\n");
            WaitForSingleObject(hEvent, INFINITE);

            printf("Работа...\n");
            for (int i = 0; i < 10; i++)
            {
                cout << i << endl;
            }
            Sleep(3000);
        }

        CloseHandle(hEvent);
    }
    else
    {
        printf("Ошибка! CreateEvent\n");
    }

    return 0;
}