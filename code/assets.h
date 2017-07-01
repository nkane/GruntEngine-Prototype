/*	assets.h
 *
 *	Created By: Nick Kane
 */

struct AssetTexture
{
	double Rotation;
	SDL_RendererFlip Flip;
	SDL_Texture *Texture;
	int Width;
	int Height;
};

struct Vector2
{
	int X;
	int Y;
};

inline Vector2
DefaultVector2Position()
{
	return Vector2 
	{
		0,
		0,
	};
}

