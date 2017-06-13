/*	hashset.h
 *
 *	Created By: Nick Kane
 */

struct HashSet
{
	unsigned int Key;
	void *Value;
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

// TODO(nick):
// 1) Handle collisions
void
HashSetInsertItem(HashSet CurrentHashset[], char *StringKey, void *CurrentAssetTexture)
{
	unsigned int currentKey = 0;
	currentKey = SimpleHash(StringKey);
	CurrentHashset[currentKey].Key = currentKey;
	CurrentHashset[currentKey].Value = CurrentAssetTexture;
}

// TODO(nick): write a select function

void
HashDeleteItem()
{
	NotImplemented;
}

