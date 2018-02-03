/*	assets.h
 *
 *	Created By: Nick Kane
 */

struct AssetTexture
{
	double Rotation;
	SDL_RendererFlip Flip;
	SDL_Texture *Texture;
	int PixelWidth;
	int PixelHeight;
};
