/*	entity.h
 *
 *	Created By: Nick Kane
 */

// TODO(nick): bit mask may not be needed, think about it some more ...
enum EntityState
{
	Idle      = (1u << 0),
	FaceLeft  = (1u << 1),
	FaceRight = (1u << 2),
};

// TODO(nick):
// remove Entity Texture hashset and replace with
//
struct Entity
{
	int Id;
	AssetTexture *CurrentTexture;
	HashSet_AssetTexture TextureSet[128];
	//HashSet_AssetTexture TextureSet[32];
	EntityState CurrentState;
	Vector2 PositionV2;
	SDL_Rect CollisionBox;
};

struct Tile
{
	int Id;
	bool IsStatic;
	bool IsCollidable;
	AssetTexture *CurrentTexture;
	Vector2 PositionV2;
	SDL_Rect CollisionBox;
};

void
DecodeAssetName(char *code, char *outBuffer, int max)
{
	int i = 0;
	StringClear(outBuffer, max);
	while (*(code + i) != '\0')
	{
		if (i == 0)
		{
			switch (*(code + i))
			{
				case 'L':
				{
					StringConcatenate(outBuffer, "Ladder-");
				} break;

				case 'T':
				{
					StringConcatenate(outBuffer, "Tile-");
				} break;
		
				default:
				{
					// TODO(nick): error logging
				} break;
			}
		} 
		else
		{
			char temp[2] =
			{
				*(code + i),
				'\0',
			};
			StringConcatenate(outBuffer, temp); 
		}
		++i;
	}
}

// TODO(nick): this this process out a bit more ...
// NOTE(nick): list of "tiles" that are not collidable
// 1) Ladders should not stop plays, but players should be allowed to "climb" ladders
bool
IsCollidable(char *code)
{
	bool result = true;
	switch (*(code + 0))
	{
		case 'L':
		{
			result = false;
		} break;

		default:
		{

		} break;
	}
	return result;
}

