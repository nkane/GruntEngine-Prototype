/*	text.h
 *
 * 	Created By: Nick Kane
 */

/*
struct Text
{
	TTF_Font *PrimaryFont;
	AssetTexture *PrimaryText;
	Vector2 PrimaryPositionV2;

	TTF_Font *SecondaryFont;
	AssetTexture *SecondaryText;
	Vector2 SecondaryPositionV2;
};
*/

struct Text
{
	AssetTexture *Text;
	Vector2 Position;
};

