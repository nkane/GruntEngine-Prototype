/*
 *	Created By: Nick Kane
 *
 */

#include <stdint.h>

// int typedefs
typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef int32 bool32;

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef intptr_t  intptr;
typedef uintptr_t uintptr;

typedef size_t memory_index;

typedef float real32;
typedef double real64;

#define array_len(array) (sizeof(array)/sizeof(array[0]))
#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}
#define NotImplemented Assert(!"NotImplemented")
#define InvalidCodePath Assert(!"InvalidCodePath")
#define InvalidDefaultCase default: {InvalidCodePath;}break

#define local_persist   static
#define internal 	    static
#define global_variable static

enum platform_file_type
{
    PlatformFileType_AssetFile     = 0x00,
    PlatformFileType_SavedGameFile = 0x01,
    PlatformFileType_Count         = 0x02,
};

struct platform_file_group
{
    uint32 FileCount;
    void *Platform;
};

#define PLATFORM_GET_ALL_FILE_OF_TYPE_BEGIN(name) platform_file_group name(platform_file_type Type)
typedef PLATFORM_GET_ALL_FILE_OF_TYPE_BEGIN(platform_get_all_files_of_type_begin);

#define PLATFORM_GET_ALL_FILE_OF_TYPE_END(name) void name(platform_file_group *FileGroup)
typedef PLATFORM_GET_ALL_FILE_OF_TYPE_END(platform_get_all_files_of_type_end);

struct PlatformAPI
{
    platform_get_all_files_of_type_begin *GetAllFileOfTypeBegin;
    platform_get_all_files_of_type_end   *GetAllFileOfTypeEnd;
};
