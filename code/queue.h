/*	queue.h
 *
 *	Created By: Nick Kane
 */

struct Entity_Node
{
	Entity *Data;
	Entity_Node *Previous;
};

// TODO(nick): finish rendering queues up!
struct Queue_GameEntity
{
	Entity_Node *Head;
	Entity_Node *Tail;
	int Size;
	int Limit;
};

bool
Queue_Enqueue_GameEntity(Queue_GameEntity *EntityQueue, Entity_Node EntityNode)
{
	Assert(EntityQueue->Size < EntityQueue->Limit);
	Entity_Node *AllocatedNode = NULL;

	AllocatedNode = (Entity_Node *)((EntityQueue + sizeof(Queue_GameEntity)) + (sizeof(Entity_Node) * EntityQueue->Size));
	AllocatedNode->Data = EntityNode.Data;
	AllocatedNode->Previous = NULL;

	if (EntityQueue->Size == 0)
	{	
		EntityQueue->Head = AllocatedNode;
		EntityQueue->Tail = AllocatedNode;
	}
	else 
	{
		Entity_Node *Temp = EntityQueue->Tail;
		EntityQueue->Tail = AllocatedNode;
		EntityQueue->Tail->Previous = Temp;
	}

	EntityQueue->Size++;

	return true;
}

Entity *
Queue_Dequeue_GameEntity(Queue_GameEntity *EntityQueue)
{
	// TODO(nick)
	return NULL;
}

bool
Queue_IsEmpty_GameEntity(Queue_GameEntity *);

struct Queue_GameText
{
	Text *Head;
	Text *Tail;
	int Size;
	int Limit;
};

bool
Queue_Enqueue_GameText(Queue_GameText *, Text *);

Text *
Queue_Dequeue_GameText(Queue_GameText *);

bool 
Queue_IsEmpty_GameText(Queue_GameText *);

