/*	shapes.h
 *
 *	Created By: Nick Kane
 */

struct Rectangle
{
    int X;
    int Y;
    int Width;
    int Height;
};

enum CollisionSide
{
    NoCollision         = 0x00,
    TopCollision        = 0x01,
    BottomCollision     = 0x02,
    RightCollision      = 0x03,
    LeftCollision       = 0x04,
};

CollisionSide
CheckRectangleOverlap(Rectangle rectA, Rectangle rectB)
{
    CollisionSide result = NoCollision;

    // rectA coordinates
    int rectALeftX = rectA.X;
    int rectARightX = rectA.X + rectA.Width;
    int rectATopY = rectA.Y;
    int rectABottomY = rectA.Y + rectA.Height;

    // rectB coordinates
    int rectBLeftX = rectB.X;
    int rectBRightX = rectB.X + rectB.Width;
    int rectBTopY = rectB.Y;
    int rectBBottomY = rectB.Y + rectB.Height;
    if (rectALeftX <= rectBRightX &&
        rectARightX >= rectBLeftX &&
        rectATopY <= rectBBottomY &&
        rectABottomY >= rectBTopY
       )
    {
        // TODO(nick): make a mid point / distance function
        float rectACenterX = (rectALeftX + rectARightX / 2.0f);
        float rectBCenterX = (rectBLeftX + rectBRightX / 2.0f);
        float horizontalDistance = fabs(pow(rectACenterX, 2) + pow(rectBCenterX, 2));

        float rectACenterY = ((rectATopY + rectABottomY) / 2.0f);
        float rectBCenterY = ((rectBTopY + rectBBottomY) / 2.0f);
        float verticalDistance = fabs(pow(rectACenterY, 2) + pow(rectBCenterY, 2));
        if (horizontalDistance > verticalDistance)
        {
            if (rectACenterX < rectBCenterX)
            {
                result = RightCollision;
            }
            else
            {
                result = LeftCollision;
            }
        }
        else if (verticalDistance > horizontalDistance)
        {
            if (rectACenterY < rectBCenterY)
            {
                result = BottomCollision;
            }
            else
            {
                result = TopCollision;
            }
        }
    }
    return result;
}
