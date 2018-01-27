/*	vector2i.h
 *
 *	Created By: Nick Kane
 */

struct Vector2i
{
    union 
    {
        struct
        {
            int X;
            int Y;
        };
        struct 
        {
            int Width;
            int Height;
        };
    };
};

inline Vector2i
DefaultVector2iPosition()
{
	return Vector2i
	{
		0,
		0,
	};
}

inline Vector2i
DefaultVector2iCenterScreen(int screenWidth, int screenHeight)
{
	return Vector2i
	{
		(screenWidth / 2),
		(screenHeight / 2),
	};
}

