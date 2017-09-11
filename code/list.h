/*	list.h
 *
 *	Created By: Nick Kane
 */

struct EntityList_Node
{
	Entity *Value;
	EntityList_Node *Next;
	EntityList_Node *Previous;
};

struct Entity_List
{
	EntityList_Node *Head;
	EntityList_Node *Tail;
};

void
EntityList_Add(Entity_List *CurrentList, Entity *NewEntity)
{
	Assert(CurrentList);
	// check if the head is null
	if (CurrentList->Head)
	{
		// add to tail
	}
	else
	{
		CurrentList->Head->Value = NewEntity;
		CurrentList->Tail->Value = NewEntity;
	}
}

struct TileList_Node
{
	Tile *Value;
	TileList_Node *Next;
	TileList_Node *Previous;
};

struct Tile_List
{
	TileList_Node *Head;
	TileList_Node *Tail;
};

void
TileList_Add(Tile_List *CurrentList, Tile *NewTile)
{
	// TODO(nick):
	// 1) finish this up
}
