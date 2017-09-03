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

struct Entity
{
	int Id;
	AssetTexture *CurrentTexture;
	HashSet_AssetTexture TextureSet[32];
	EntityState CurrentState;
	Vector2 PositionV2;
	SDL_Rect CollisionBox;
};

struct Tile
{
	int Id;
	bool IsStatic;
	AssetTexture *CurrentTexture;
	Vector2 PositionV2;
	SDL_Rect CollisionBox;
};

