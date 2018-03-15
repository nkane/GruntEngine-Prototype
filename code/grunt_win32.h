/*
 *  Create By: Nick Kane
 *
 */

#include <windows.h>

#define FOLDER_LIMIT        32
#define FOLDER_NAME_LIMIT   64

struct win32_platform_file_group
{
    HANDLE FindHandle;
    WIN32_FIND_DATAA FindData;
};

// TODO(nick):
// 1) hacky way of handling this - probably a better solution ...
// 2) possible make this an seperate exe that makes a configuration file for the game to consume?
internal
PLATFORM_GET_ALL_FOLDERS_IN_DIRECTORY_BEGIN(Win32GetAllFoldersInDirectoryBegin)
{
    char path[128];
    StringCopyOverwrite(path, directoryPath, 128);
    StringConcatenate(path, "/*.*\0");
    WIN32_FIND_DATAA FindData;
    HANDLE FindHandle = FindFirstFileA(path, &FindData);
    int i = 0;
    int j = 0;
    while (FindHandle != INVALID_HANDLE_VALUE)
    {
        if ((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
            (StringCompare(FindData.cFileName, ".") != 0)          &&
            (StringCompare(FindData.cFileName, "..") != 0))
        {
            // get the path for the next directory
            char nextDirectoryPath[128] = { 0 };
            StringCopyOverwrite(nextDirectoryPath, directoryPath, 128);
            StringConcatenate(nextDirectoryPath, "/");
            StringConcatenate(nextDirectoryPath, FindData.cFileName);

            char findPath[256] = { 0 };
            StringConcatenate(findPath, nextDirectoryPath);
            StringConcatenate(findPath, "/");
            StringConcatenate(findPath, "*.png");

            // create texture base name
            char textureBaseName[32] = { 0 };
            StringCopyOverwrite(textureBaseName, FindData.cFileName, 32);

            // need the next find name to determine what to store as
            WIN32_FIND_DATAA NestedFindData;
            HANDLE NestedFileHandle = FindFirstFileA(findPath, &NestedFindData);
            while (NestedFileHandle != INVALID_HANDLE_VALUE)
            {
                // set the texture name
                char *textureName = (char *) VirtualAlloc(0, sizeof(char) * 32, (MEM_RESERVE | MEM_COMMIT), PAGE_READWRITE);
                StringCopyOverwrite(textureName, textureBaseName, 32);
                StringConcatenate(textureName, "-");
                StringConcatenate(textureName, NestedFindData.cFileName);
                StringTrim(textureName, '.');
                textureList[i][j] = textureName;

                // set the texture path
                char *texturePath = (char *) VirtualAlloc(0, sizeof(char) * 256, (MEM_RESERVE | MEM_COMMIT), PAGE_READWRITE);
                StringCopyOverwrite(texturePath, nextDirectoryPath, 256);
                StringConcatenate(texturePath, "/");
                StringConcatenate(texturePath, NestedFindData.cFileName);
                textureList[i][j + 1] = texturePath;
                ++i;

                if (!FindNextFileA(NestedFileHandle, &NestedFindData))
                {
                    break;
                }
            }
            ++j;
        }

        if (!FindNextFileA(FindHandle, &FindData))
        {
            break;
        }
    }
    FindClose(FindHandle);
}

internal
PLATFORM_GET_ALL_FOLDERS_IN_DIRECTORY_END(Win32GetAllFoldersInDirectoryEnd)
{
    // TODO(nick): have to free each pointer!
    if (directoryContent)
    {
        VirtualFree(directoryContent, 0, MEM_RELEASE);
    }
}

internal
PLATFORM_GET_ALL_FILE_OF_TYPE_BEGIN(Win32GetAllFilesOfTypeBegin)
{
    platform_file_group Result = {};
    win32_platform_file_group *Win32FileGroup = (win32_platform_file_group *) VirtualAlloc(0, sizeof(win32_platform_file_group), (MEM_RESERVE | MEM_COMMIT), PAGE_READWRITE);
    Result.Platform = Win32FileGroup;
    char *WildCard = "*.*";
    switch (Type)
    {
        case PlatformFileType_AssetFile:
        {
            WildCard = "*.png";
        } break;

        case PlatformFileType_SavedGameFile:
        {
            WildCard = "*.gs";
        } break;

        InvalidDefaultCase;
    }
    Result.FileCount = 0;
    WIN32_FIND_DATAA FindData;
    HANDLE FindHandle = FindFirstFileA(WildCard, &FindData);
    while (FindHandle != INVALID_HANDLE_VALUE)
    {
        ++Result.FileCount;
        if (!FindNextFileA(FindHandle, &FindData))
        {
            break;
        }
    }
    FindClose(FindHandle);
    Win32FileGroup->FindHandle = FindFirstFileA(WildCard, &Win32FileGroup->FindData);
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
