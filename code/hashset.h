/*	hashset.h
 *
 *	Created By: Nick Kane
 */

struct AssetTexture_Hashset
{
	unsigned int Key;
	AssetTexture *Value;
};

unsigned int
SimpleHash(char *StringKey)
{
	unsigned int key = 0;
	for (int i = 0; StringKey[i] != '\0'; ++i)
	{
		key = ((StringKey[i] + (key << 6) + key) + (key << 16) - key);
	}
	key &= 31;

	return key;
}

// NOTE(nick): probably need to pass in permanentstorage here?
void
insert(AssetTexture_Hashset *CurrentHashset[], char *StringKey, AssetTexture *CurrentAssetTexture)
{
	unsigned int currentKey = SimpleHash(StringKey);

	// check if exist
	if (CurrentHashset[currentKey] != NULL)
	{
		// TODO(nick): finish this up!!
	}
}

