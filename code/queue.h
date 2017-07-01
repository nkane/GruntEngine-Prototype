/*	queue.h
 *
 *	Created By: Nick Kane
 */

// TODO(nick): finish rendering queues up!
struct Queue_GameEntity
{
	Entity *Head;
	Entity *Tail;
	int Size;
	int Limit;
};

bool
Queue_Enqueue_GameEntity(Queue_GameEntity *, Entity *);

Entity *
Queue_Dequeue_GameEntity(Queue_GameEntity *);

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
