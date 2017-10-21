/*	gamememory.h
 *
 *	Created By: Nick Kane
 */

#define Kilobytes(value) ((value)*1024LL)
#define Megabytes(value) (Kilobytes(value)*1024LL)
#define Gigabytes(value) (Megabytes(value)*1024LL)
#define Terabytes(value) (Gigabytes(value)*1024LL)

struct MemoryBlock
{
    MemoryBlock *Next;
    uint64 Size;
    uint64 CurrentBytes;
};

struct GameMemory
{
    MemoryBlock *PermanentStorage;
    MemoryBlock *TransientStorage;
};

// TODO(nick): iron this process out ...
inline void *
PushMemoryChunk(MemoryBlock *Block, int chunkSize)
{
    Assert(Block->CurrentBytes + chunkSize <= Block->Size);
    
    uint8 *MemoryChunk = ((uint8 *)Block->Next);
    Block->Next += chunkSize;
    Block->CurrentBytes += chunkSize;
    
    return (void *)MemoryChunk;
}

inline void
PopMemoryChunk()
{
    // TODO(nick)
}

