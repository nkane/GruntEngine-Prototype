/*	shapes.h
 *
 *	Created By: Nick Kane
 */

// TODO(nick): IMPORTANT
//  - make a proper rect struct that is just x, y, l, and width
//  - we could make it two vectors?
struct Rectangle
{
	Vector2i TopLine[2];
	Vector2i BottomLine[2];
};

