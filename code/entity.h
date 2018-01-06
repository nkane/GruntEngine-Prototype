/*	entity.h
 *
 *	Created By: Nick Kane
 */

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

void
UpdatePlayerState(Entity *PlayerEntity, int NewState, HashSet_AssetTexture *TextureSet)
{
    if (PlayerEntity->CurrentState & (Walking))
    {
        if (PlayerEntity->CurrentState & (NewState))
        {
            // TODO(nick): move to next frame in animation
            // NOTE(nick): walk animation for player is
            // 1) idle 
            // 2) walk-1
            // 3) walk-2
            switch (PlayerEntity->CurrentFrame)
            {
                case 0:
                { 
                    //PlayerEntity->CurrentTexture = ;
                    PlayerEntity->CurrentFrame++;
                } break;
                
                case 1:
                {
                    PlayerEntity->CurrentFrame++;
                } break;
                
                case 2:
                {
                    PlayerEntity->CurrentFrame = 0;
                } break;
            }
        }
        else
        {
            
        }
    }
    else if (PlayerEntity->CurrentState & (Idle))
    {
        
    }
    else if (PlayerEntity->CurrentState & (Jumping))
    {
        
    }
}

void UpdateEnemyState(Entity *EnemyEntity)
{
    
}

void 
UpdateEntityPosition()
{
    
}
