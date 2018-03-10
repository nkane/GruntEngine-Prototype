/*
 *  Create By: Nick Kane
 *
 */

#include <windows.h>

struct win32_platform_file_group
{
    HANDLE FindHandle;
    WIN32_FIND_DATAW FindData;
};

internal
PLATFORM_GET_ALL_FILE_OF_TYPE_BEGIN(Win32GetAllFilesOfTypeBegin)
{
    platform_file_group Result = {};
    win32_platform_file_group *Win32FileGroup = (win32_platform_file_group *) VirtualAlloc(0, sizeof(win32_platform_file_group), (MEM_RESERVE | MEM_COMMIT), PAGE_READWRITE);
    Result.Platform = Win32FileGroup;
    wchar_t *WildCard = L"*.*";
    switch (Type)
    {
        case PlatformFileType_AssetFile:
        {
            WildCard = L"*.png";
        } break;

        case PlatformFileType_SavedGameFile:
        {
            WildCard = L"*.gs";
        } break;

        InvalidDefaultCase;
    }
    Result.FileCount = 0;
    WIN32_FIND_DATAW FindData;
    HANDLE FindHandle = FindFirstFileW(WildCard, &FindData);
    while (FindHandle != INVALID_HANDLE_VALUE)
    {
        ++Result.FileCount;
        if (!FindNextFileW(FindHandle, &FindData))
        {
            break;
        }
    }
    FindClose(FindHandle);
    Win32FileGroup->FindHandle = FindFirstFileW(WildCard, &Win32FileGroup->FindData);
    return Result;
}

internal
PLATFORM_GET_ALL_FILE_OF_TYPE_END(Win32GetAllFilesOfTypeEnd)
{
    win32_platform_file_group *Win32FileGroup = (win32_platform_file_group *)FileGroup->Platform;
    if (Win32FileGroup)
    {
        FindClose(Win32FileGroup->FindHandle);
        VirtualFree(Win32FileGroup, 0, MEM_RELEASE);
    }
}
