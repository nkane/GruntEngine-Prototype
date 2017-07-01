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
	AssetTexture *CurrentTexture;
	HashSet_AssetTexture TextureSet[32];
	EntityState CurrentState;
	Vector2 PositionV2;
};

