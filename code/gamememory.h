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
	MemoryBlock *Previous;
	uint64 Size;
	// TODO(nick): Add remaining size?
};

struct GameMemory
{
	MemoryBlock *PermanentStorage;
	MemoryBlock *TransientStorage;
};

inline void *
PushMemoryChunk(MemoryBlock *Block, int chunkSize)
{
	// TODO(nick): iron this process out ...
	void *MemoryChunk;
	MemoryChunk = (void *)Block->Next;
	Block->Next += chunkSize;
	Block->Next->Previous = Block;
	return MemoryChunk;
}

inline void
PopMemoryChunk()
{
	// TODO(nick)
}

