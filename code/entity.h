/*	    entity.h
 *
 *	    Created By: Nick Kane
 */

#define PLAYER_MAX_ACCELERATION 8.0f

// TODO(nick): bit mask may not be needed, think about it some more ...
enum EntityState
{
    Stateless = (0u),           // 0
    Idle      = (1u << 0),      // 1
    Walking   = (1u << 1),      // 2 
    Jumping   = (1U << 2),      // 4
};

enum EntityFaceDirection
{
    Directionless    = 0x00,
    FaceLeft         = 0x01,
    FaceRight        = 0x02,
};

// TODO(nick):
// remove Entity Texture hashset and replace with
struct Entity
{
    unsigned int Id;
    AssetTexture *CurrentTexture;
    unsigned int CurrentState;
    unsigned int CurrentFaceDirection;
    unsigned int CurrentFrame;
    Vector2f PositionV2f;
    Vector2f VelocityV2f;
    Vector2f CollisionPositionV2f;
    Vector2f CollisionDimensionV2f;
};

struct Tile
{
    int Id;
    bool IsStatic;
    bool IsCollidable;
    AssetTexture *CurrentTexture;
    Vector2i PositionV2i;
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

void
UpdatePlayerPosition(Entity *player, Vector2f acceleration, float decayRate)
{
    player->VelocityV2f.X *= decayRate;
    player->VelocityV2f = Vector2fAdd(player->VelocityV2f, acceleration);
    float magnitude = Vector2fLength(player->VelocityV2f);
    if (magnitude > PLAYER_MAX_ACCELERATION)
    {
        Vector2fScale(&player->VelocityV2f, PLAYER_MAX_ACCELERATION / magnitude);
    }
    player->CollisionPositionV2f = Vector2fAdd(player->CollisionPositionV2f, player->VelocityV2f);
    player->PositionV2f = Vector2fAdd(player->PositionV2f, player->VelocityV2f);
}
