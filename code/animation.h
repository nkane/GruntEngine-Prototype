/*     animation.h
 *
 *     Created By: Nick Kane
 */

enum AnimationType
{
    IdleAnimation     = (1u << 0),
    WalkingAnimation  = (1u << 1),
    JumpingAnimation  = (1U << 2),
};

struct Animation
{
    int AnimationType;
    int FrameLength;
    AssetTexture *AnimationStrip[10];
};

// TODO(nick): generalize with a callback function instead
void
BuildPlayerAnimations(Entity *CurrentEntity, HashSet_AssetTexture *TextureHashSet, Animation *OutAnimations[10], MemoryBlock *Block)
{
    Animation *CurrentAnimation = NULL;
    int i = 0;
    int frameIndex = 0;
    // idle animation
    CurrentAnimation = (Animation *)PushMemoryChunk(Block, sizeof(Animation));
    CurrentAnimation->AnimationType = IdleAnimation;
    CurrentAnimation->FrameLength = 1;
    CurrentAnimation->AnimationStrip[frameIndex] = HashSet_Select_AssetTexture(TextureHashSet, "Grunt-Idle");
    OutAnimations[i++] = CurrentAnimation;
    
    // walking animation
    CurrentAnimation = (Animation *)PushMemoryChunk(Block, sizeof(Animation));
    // TODO(nick): finish up static loading for player to get it working at first!
}

void
BuildEnemyAnimations()
{
    // TODO(nick):
}