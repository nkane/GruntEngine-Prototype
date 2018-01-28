/*	vector2i.h
 *
 *	Created By: Nick Kane
 */

struct Vector2f
{
    union 
    {
        struct
        {
            float X;
	        float Y;
        };
        struct 
        {
            float Width;
            float Height;
        };
    };
};

inline Vector2f
DefaultVector2fPosition()
{
    return Vector2f
	{
		0.0f,
		0.0f,
	};
}

inline float
Vector2fLengthSquared(Vector2f v2)
{
    float result = 0.0f;
    result = (v2.X * v2.X + v2.Y * v2.Y);
    return result;
}

inline float
Vector2fLength(Vector2f v2)
{
    float result = 0.0f;
    result = sqrt(v2.X * v2.X + v2.Y * v2.Y);
    return result;
}

inline Vector2f
Vector2fAdd(Vector2f v1, Vector2f v2)
{
    Vector2f result
    {
        (v1.X + v2.X),
        (v1.Y + v2.Y),
    };
    return result;
}

inline Vector2f
Vector2fSubtract(Vector2f v1, Vector2f v2)
{
    Vector2f result
    {
        (v1.X - v2.X),
        (v1.Y - v2.Y),
    };
    return result;
}

inline Vector2f
Vector2fMultiply(Vector2f v2, float scalar)
{
    Vector2f result =
    {
        (v2.X * scalar),
        (v2.Y * scalar),
    };
    return result;
}

inline Vector2f
Vector2fDivide(Vector2f v2, float scalar)
{
    Vector2f result =
    {
        (v2.X / scalar),
        (v2.Y / scalar),
    };
    return result;
}

inline void
Vector2fScale(Vector2f *v2, float scale)
{
    v2->X *= scale;
    v2->Y *= scale;
}
