/*
 *	strings.h
 *
 */

void
StringClear(char *stringBuffer, int maxSize)
{
	for (int i = 0; i < maxSize; ++i)
	{
		*(stringBuffer + i) = '\0';
	}
}

void
StringCopyOverwrite(char *sourceString, char *tempString, uint32 maxSourceSize)
{
	StringClear(sourceString, maxSourceSize);
	for (int i = 0; *(tempString + i) != '\0'; ++i)
	{
		*(sourceString + i) = *(tempString + i);
	}
}

// TODO(nick): add a max?
void
StringConcatenate(char *sourceString, char *attachString)
{
	uint32 i = 0;
	uint32 j = 0;

	while (*(sourceString + i) != '\0')
	{
		++i;
	};

	while (*(attachString + j) != '\0')
	{
		*(sourceString + i) = *(attachString + j);
		++i;
		++j;
	}
}

uint32
StringSize(char *string)
{
    int result = 0;
    while (*string)
    {
        ++string;
        ++result;
    }
    return result;
}

inline char
ToLower(char c)
{
    if (c < 'a' && (c >= 'A') || (c <= 'Z'))
    {
        uint8 offset = ((uint8)'a') - ((uint8)'A');
        c += offset;
    }
    return c;
}

// NOTE (nick):
// return result:
//  - (-1), string two comes before string one
//  - ( 0), values are equal
//  - ( 1), string one comes before string two
int8
StringCompare(char *stringOne, char *stringTwo)
{
    int8 result = 0;
    uint32 sizeOne = StringSize(stringOne);
    uint32 sizeTwo = StringSize(stringTwo);
    uint32 i = 0;
    uint32 j = 0;
    while (i < sizeOne && j < sizeTwo)
    {
        if (ToLower(stringOne[i]) > ToLower(stringTwo[j]))
        {
            result = 1;
            break;
        }
        else if (ToLower(stringOne[i]) < ToLower(stringTwo[j]))
        {
            result = -1;
            break;
        }
        ++i;
        ++j;
    }

    // NOTE(nick):
    // strings are currently equal, need to check size
    // cases like:
    //  - there
    //  - the
    //  the word "the" should come before there
    if (sizeOne > sizeTwo)
    {
        result = -1;
    }
    else if (sizeOne < sizeTwo)
    {
        result = 1;
    }

    return result;
}


