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
