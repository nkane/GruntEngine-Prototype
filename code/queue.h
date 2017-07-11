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
		EntityQueue->Tail->Previous = AllocatedNode;
		EntityQueue->Tail = AllocatedNode;
	}

	EntityQueue->Size++;

	return true;
}

Entity *
Queue_Dequeue_GameEntity(Queue_GameEntity *EntityQueue)
{
	Entity *CurrentEntity = NULL;
	if (EntityQueue && EntityQueue->Size > 0)
	{
		CurrentEntity = EntityQueue->Head->Data;
		EntityQueue->Head = EntityQueue->Head->Previous;
		EntityQueue->Size--;
	}
	return CurrentEntity;
}

Entity_Node
Queue_Null_Entity_Node()
{
	Entity_Node NulledNode = 
	{
		NULL,
		NULL,
	};
	return NulledNode;
}

struct Text_Node
{
	Text *Data;
	Text_Node *Previous;
};

struct Queue_GameText
{
	Text_Node *Head;
	Text_Node *Tail;
	int Size;
	int Limit;
};

bool
Queue_Enqueue_GameText(Queue_GameText *TextQueue, Text_Node TextNode)
{
	Assert(TextQueue->Size < TextQueue->Limit);
	Text_Node *AllocatedNode = NULL;

	AllocatedNode = (Text_Node *)((TextQueue + sizeof(Queue_GameText)) + (sizeof(Text_Node) * TextQueue->Size));
	AllocatedNode->Data = TextNode.Data;
	AllocatedNode->Previous = NULL;

	if (TextQueue->Size == 0)
	{
		TextQueue->Head = AllocatedNode;
		TextQueue->Tail = AllocatedNode;
	}
	else
	{
		TextQueue->Tail->Previous = AllocatedNode;
		TextQueue->Tail = AllocatedNode;
	}

	TextQueue->Size++;

	return true;
}

Text *
Queue_Dequeue_GameText(Queue_GameText *TextQueue)
{
	Text *CurrentText = NULL;
	if (TextQueue && TextQueue->Size > 0)
	{
		CurrentText = TextQueue->Head->Data;
		TextQueue->Head = TextQueue->Head->Previous;
		TextQueue->Size--;
	}
	return CurrentText;
}

Text_Node
Queue_Null_Text_Node()
{
	Text_Node NulledNode = 
	{
		NULL,
		NULL,
	};
	return NulledNode;
}

