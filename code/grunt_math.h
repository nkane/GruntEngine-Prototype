/*	gruntmath.h
 *
 *	Created By: Nick Kane
 */

inline float
MidPointf(int p1, int p2)
{
    float result = ((p1 + p2) / 2.0f);
    return result;
}

inline float
PositionSquareDistance(float p1, float p2)
{
    float result = fabs((p1 * p1) - (p2 * p2));
    return result;
}
