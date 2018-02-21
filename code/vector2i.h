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

inline float
Vector2iLengthSquared(Vector2i v2)
{
    float result = 0.0f;
    result = ((float)v2.X * (float)v2.X + (float)v2.Y * (float)v2.Y);
    return result;
}

inline float
Vector2iLength(Vector2i v2)
{
    float result = 0.0f;
    result = sqrt((float)v2.X * (float)v2.X + (float)v2.Y * (float)v2.Y);
    return result;
}

inline Vector2i
Vector2iAdd(Vector2i v1, Vector2i v2)
{
    Vector2i result
    {
        (v1.X + v2.X),
        (v1.Y + v2.Y),
    };
    return result;
}

inline Vector2i
Vector2iSubtract(Vector2i v1, Vector2i v2)
{
    Vector2i result =
    {
        (v1.X - v2.X),
        (v1.Y - v2.Y),
    };
    return result;
}

inline Vector2i
Vector2iMultiply(Vector2i v2, int scalar)
{
    Vector2i result =
    {
        (v2.X * scalar),
        (v2.Y * scalar),
    };
    return result;
}

inline Vector2i
Vector2iDivide(Vector2i v2, int scalar)
{
    Vector2i result =
    {
        (v2.X / scalar),
        (v2.Y / scalar),
    };
    return result;
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

inline void
Vector2iScale(Vector2i *v2, float scale)
{
    v2->X *= scale;
    v2->Y *= scale;
}
