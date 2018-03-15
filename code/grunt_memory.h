/*	gamememory.h
 *
 *	Created By: Nick Kane
 */

#define Kilobytes(value) ((value)*1024LL)
#define Megabytes(value) (Kilobytes(value)*1024LL)
#define Gigabytes(value) (Megabytes(value)*1024LL)
#define Terabytes(value) (Gigabytes(value)*1024LL)

#define FREED_VALUE 0xFF

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

inline void *
PushMemoryChunk(MemoryBlock *Block, int chunkSize)
{
    Assert(Block->CurrentBytes + chunkSize <= Block->Size);
    uint8 *MemoryChunk = ((uint8 *)Block->Next);
    Block->Next += chunkSize;
    Block->CurrentBytes += chunkSize;
    return (void *)MemoryChunk;
}

// TODO(nick): maybe consider creating a freelist?
inline void
FlagMemoryChunkAsFreed(void *BlockData, int chunkSize)
{
    uint8 *currentByte = ((uint8 *)BlockData);
    uint8 *lastByte = (uint8 *)(currentByte + chunkSize);
    while (currentByte < lastByte)
    {
        *currentByte = FREED_VALUE;
        ++currentByte;
    }
}
