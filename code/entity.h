/*	    entity.h
 *
 *	    Created By: Nick Kane
 */

#define PLAYER_MAX_ACCELERATION 2.5f

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
    SDL_Rect CollisionBox;
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
UpdatePlayerPosition(Entity *player, Vector2f acceleration, float delta)
{
    SDL_Rect previousPlayerCollsionBox = player->CollisionBox;
    player->VelocityV2f = Vector2fAdd(player->VelocityV2f, acceleration);

    float magnitude = Vector2fLength(player->VelocityV2f);
    if (magnitude > PLAYER_MAX_ACCELERATION)
    {
        Vector2fScale(&player->VelocityV2f, PLAYER_MAX_ACCELERATION / magnitude);
    }

    Vector2f tempCollisionBoxPosition =
    {
        (float)player->CollisionBox.x,
        (float)player->CollisionBox.y,
    };

    Vector2f testCollisionBoxPosition = Vector2fAdd(tempCollisionBoxPosition, player->VelocityV2f);
    player->CollisionBox.x = testCollisionBoxPosition.X;
    player->CollisionBox.y = testCollisionBoxPosition.Y;

    // TODO(nick): remove this! for testing only
    player->PositionV2f = Vector2fAdd(player->PositionV2f, player->VelocityV2f);

    // TODO(nick):
    // 1) uncomment code!
    /*
    if (CheckCollision(GlobalEntityArray, GlobalCurrentLoadedLevel, PlayerEntity->Id))
    {
        // collided do not update position and reset collision box
        PlayerEntity->CollisionBox.x = previousPlayerCollsionBox.x;
        PlayerEntity->CollisionBox.y = previousPlayerCollsionBox.y;
    }
    else
    {
        PlayerEntity->PositionV2f = Vector2fAdd(PlayerEntity->PositionV2f, tempVector);
    }
    */
}
