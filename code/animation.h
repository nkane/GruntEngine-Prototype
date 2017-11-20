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
	CurrentAnimation->FrameRateMS = 100;
	CurrentAnimation->PreviousTime = 0;
	CurrentAnimation->CurrentFrame = 0;
	CurrentAnimation->AnimationStrip[frameIndex] = HashSet_Select_AssetTexture(TextureHashSet, "Grunt-Idle");
	OutAnimations[i++] = CurrentAnimation;

	// walking animation
	CurrentAnimation = (Animation *)PushMemoryChunk(Block, sizeof(Animation));
	CurrentAnimation->FrameLength = 2;
	CurrentAnimation->FrameRateMS = 100;
	CurrentAnimation->PreviousTime = 0;
	CurrentAnimation->CurrentFrame = 0;
	CurrentAnimation->AnimationStrip[frameIndex++] = HashSet_Select_AssetTexture(TextureHashSet, "Grunt-Walk-1");
	CurrentAnimation->AnimationStrip[frameIndex++] = HashSet_Select_AssetTexture(TextureHashSet, "Grunt-Walk-2");
	OutAnimations[i++] = CurrentAnimation;

	// TODO(nick): jumping animation
	frameIndex = 0;
	CurrentAnimation = (Animation *)PushMemoryChunk(Block, sizeof(Animation));
	CurrentAnimation->FrameLength = 3;
	CurrentAnimation->FrameRateMS = 100;
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
SelectPlayerAnimationFrame(Entity *CurrentPlayer, Animation *CurrentPlayerAnimation[10])
{
	AssetTexture *Result = NULL;
	Animation *CurrentAnimation = NULL;

	// NOTE(nick): select current animation strip based on 
	if (CurrentPlayer->CurrentState | (Walking))
	{
		CurrentAnimation = CurrentPlayerAnimation[WalkingAnimation];
	}
	else if (CurrentPlayer->CurrentState | (Idle))
	{
		CurrentAnimation = CurrentPlayerAnimation[Idle];
	}
	// TODO(nick): finish up the rest of the animations
	
	if (CurrentAnimation->PreviousTime + CurrentAnimation->FrameRateMS > SDL_GetTicks())
	{
		Result = CurrentAnimation->AnimationStrip[CurrentAnimation->CurrentFrame];
	}
	else
	{
		Result = CurrentAnimation->AnimationStrip[++CurrentAnimation->CurrentFrame];
	}

	// TODO(nick): handling texture direction flipping

	return Result;
}

