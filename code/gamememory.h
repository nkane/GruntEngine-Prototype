/*
 *	Created By: Nick Kane
 */

#define Kilobytes(value) ((value)i*1024LL)
#define Megabytes(value) (Kilobytes(value)*1024LL)
#define Gigabytes(value) (Megabytes(value)*1024LL)
#define Terabytes(value) (Gigabytes(value)*1024LL)

struct MemoryBlock
{
	MemoryBlock *Next;
	MemoryBlock *Previous;
	uint64 Size;
};

