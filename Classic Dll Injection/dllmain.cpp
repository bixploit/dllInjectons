// dllmain.cpp : DLL uygulamasının giriş noktasını tanımlar.
#include "pch.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    int msgboxID{};

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:

        msgboxID = MessageBox(
            NULL,
            (LPCWSTR)L"Dll Injected",
            (LPCWSTR)L"Hacked",
            MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
        );
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
    }
    return TRUE;
}

