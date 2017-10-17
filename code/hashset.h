/*	hashset.h
 *
 *	Created By: Nick Kane
 */

struct HashSet_AssetTexture
{
	unsigned int Key;
	AssetTexture *Value;
};

// TODO(nick): finish this implementation
struct HashSet_AssetText
{
	unsigned int Key;
	Text *Value;
};

unsigned int
SimpleHash(char *StringKey)
{
	unsigned int key = 0;
	for (int i = 0; StringKey[i] != '\0'; ++i)
	{
		key = ((StringKey[i] + (key << 6) + (key << 16) - key));
	}
	//key &= 31;
	key &= 63;
	return key;
}

// TODO(nick): collision checking!
void
HashSet_Insert_AssetTexture(HashSet_AssetTexture CurrentHashSet[], char *StringKey, AssetTexture *Data)
{
	unsigned int CurrentKey = 0;
	CurrentKey = SimpleHash(StringKey);
	CurrentHashSet[CurrentKey].Key = CurrentKey;
	CurrentHashSet[CurrentKey].Value = Data;
}

// TODO(nick): write a proper select function
AssetTexture *
HashSet_Select_AssetTexture(HashSet_AssetTexture CurrentHashSet[], char *StringKey)
{
	AssetTexture *SelectedTexture = NULL;
	SelectedTexture = CurrentHashSet[SimpleHash(StringKey)].Value;
	return SelectedTexture;
}

