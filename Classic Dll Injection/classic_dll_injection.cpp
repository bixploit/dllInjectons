#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tlhelp32.h>

// finding process ID
int getProcName(const wchar_t* procname) {

    HANDLE hSnapshot;
    PROCESSENTRY32 pe32;
    int pID{};
    BOOL hResult;

    // snapshot of all processes
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hSnapshot)
        return 0;

    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Get firstt process info in snapshot
    hResult = Process32First(hSnapshot, &pe32);

    // if successful go on
    while (hResult) {
        // Comparing getting processes name and specified process name
        if (wcscmp(procname, pe32.szExeFile) == 0) {
            pID = pe32.th32ProcessID;
            break;
        }
        hResult = Process32Next(hSnapshot, &pe32);
    }

    // closes an open handle
    CloseHandle(hSnapshot);
    return pID;
}

int main(int argc, char* argv[]) {
    HANDLE processHandle;
    PVOID remoteBuffer;
    char dllPath[] = "inject.dll"; //dll name to inject
    wchar_t targetProcessName[] = L"Assembly_Helper.exe"; //target process name

    // Gets process handle with process name
    processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, getProcName(targetProcessName));
    // Allocates memory in target process
    remoteBuffer = VirtualAllocEx(processHandle, NULL, sizeof(dllPath), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    // Writes dllpath to allocated memory 
    WriteProcessMemory(processHandle, remoteBuffer, (LPVOID)dllPath, sizeof(dllPath), NULL);

    // Gets LoadLibrary Function address
    PTHREAD_START_ROUTINE threatStartRoutineAddress = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryA");

    // Create a thread on target process that execute loadlibrary function with parameter. The parameter is allocated memory
    CreateRemoteThread(processHandle, NULL, 0, threatStartRoutineAddress, remoteBuffer, 0, NULL);
    CloseHandle(processHandle);

    return 0;
}