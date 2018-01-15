/*     animation.h
 *
 *     Created By: Nick Kane
 */

enum AnimationType
{
	IdleAnimation      = 0,
	WalkingAnimation   = 1,
	JumpingAnimation   = 2,
	ClimbingAnimation  = 3,
};

struct Animation
{
	int FrameRateMS;
	int PreviousTime;
	int FrameLength;
	int CurrentFrame;
	AssetTexture *AnimationStrip[10];
};

// TODO(nick):
// 1) generalize with a callback function instead?
// 2) IMPORTANT! potential memory leaks - need clean up function!
void
BuildPlayerAnimations(Entity *CurrentEntity, HashSet_AssetTexture *TextureHashSet, Animation *OutAnimations[10], MemoryBlock *Block)
{
	Animation *CurrentAnimation = NULL;
	int i = 0;
	int frameIndex = 0;
	// idle animation
	CurrentAnimation = (Animation *)PushMemoryChunk(Block, sizeof(Animation));
	CurrentAnimation->FrameLength = 1;
	CurrentAnimation->FrameRateMS = 500;
	CurrentAnimation->PreviousTime = 0;
	CurrentAnimation->CurrentFrame = 0;
	CurrentAnimation->AnimationStrip[frameIndex] = HashSet_Select_AssetTexture(TextureHashSet, "Grunt-Idle");
	OutAnimations[i++] = CurrentAnimation;

	// walking animation
	CurrentAnimation = (Animation *)PushMemoryChunk(Block, sizeof(Animation));
	CurrentAnimation->FrameLength = 2;
	CurrentAnimation->FrameRateMS = 500;
	CurrentAnimation->PreviousTime = 0;
	CurrentAnimation->CurrentFrame = 0;
	CurrentAnimation->AnimationStrip[frameIndex++] = HashSet_Select_AssetTexture(TextureHashSet, "Grunt-Walk-1");
	CurrentAnimation->AnimationStrip[frameIndex++] = HashSet_Select_AssetTexture(TextureHashSet, "Grunt-Walk-2");
	OutAnimations[i++] = CurrentAnimation;

	// TODO(nick): jumping animation
	frameIndex = 0;
	CurrentAnimation = (Animation *)PushMemoryChunk(Block, sizeof(Animation));
	CurrentAnimation->FrameLength = 3;
	CurrentAnimation->FrameRateMS = 500;
	CurrentAnimation->PreviousTime = 0;
	CurrentAnimation->CurrentFrame = 0;
	CurrentAnimation->AnimationStrip[frameIndex++] = HashSet_Select_AssetTexture(TextureHashSet, "Grunt-Climb-1");
	CurrentAnimation->AnimationStrip[frameIndex++] = HashSet_Select_AssetTexture(TextureHashSet, "Grunt-Climb-2");
	CurrentAnimation->AnimationStrip[frameIndex++] = HashSet_Select_AssetTexture(TextureHashSet, "Grunt-Climb-3");
	OutAnimations[i++] = CurrentAnimation;

	// TODO(nick): climbing animation
}

void
BuildEnemyAnimations()
{
    // TODO(nick):
}

AssetTexture *
SelectPlayerAnimationFrame(Entity *CurrentPlayer, Animation *CurrentPlayerAnimations[10], bool flip)
{
	AssetTexture *Result = NULL;
	Animation *CurrentAnimation = NULL;
	// NOTE(nick): select current animation strip based on player state
	// TODO(nick): finish up the rest of the animations
    if (CurrentPlayer->CurrentState & (Idle))
	{
		CurrentAnimation = CurrentPlayerAnimations[IdleAnimation];
	}
    else if (CurrentPlayer->CurrentState & (Walking))
	{
		CurrentAnimation = CurrentPlayerAnimations[WalkingAnimation];
	}

    unsigned int currentTick = SDL_GetTicks();
    int frameDelta = (currentTick - CurrentAnimation->PreviousTime);
	if (frameDelta > CurrentAnimation->FrameRateMS)
	{
        if (CurrentAnimation->PreviousTime != 0)
        {
            int animationIndex = CurrentAnimation->CurrentFrame + 1;
            if (CurrentAnimation->FrameLength <= animationIndex)
            {
                // reset animation to loop it
                CurrentAnimation->CurrentFrame = 0;
            }
            else
            {
                // go to next frame in animation
                CurrentAnimation->CurrentFrame++;
            }
        }
		Result = CurrentAnimation->AnimationStrip[CurrentAnimation->CurrentFrame];
        CurrentAnimation->PreviousTime = currentTick;
	}
    else
	{
		Result = CurrentAnimation->AnimationStrip[CurrentAnimation->CurrentFrame];
	}

	// NOTE(nick): handling texture direction flipping
    // TODO(nick):
    // 1) all of the frames in the animation strip need to be "flipped" or "unflipped"
    // 2) figure out a better way of handling this?
    if (flip) 
    {
        Result->Flip = SDL_FLIP_HORIZONTAL;
    }
    else
    {
        Result->Flip = SDL_FLIP_NONE;
    }

	return Result;
}

