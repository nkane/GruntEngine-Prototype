/*
 *	strings.h
 *
 */

void
StringClear(char *stringBuffer, int maxSize);

void
StringCopyOverwrite(char *sourceString, char *tempString);

void
StringConcatenate(char *sourceString, char *attachString);

void
StringClear(char *stringBuffer, int maxSize)
{
	for (int i = 0; i < maxSize; ++i)
	{
		*(stringBuffer + i) = '\0';
	}
}

void
StringCopyOverwrite(char *sourceString, char *tempString, int maxSourceSize)
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
	int i = 0;
	int j = 0;

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

