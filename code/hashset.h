/*	hashset.h
 *
 *	Created By: Nick Kane
 */

#define HASHSET_LIMIT 256
#define HASHSET_STRING_KEY_LIMIT 32

struct HashSet_AssetTexture
{
    char Key[HASHSET_STRING_KEY_LIMIT];
    AssetTexture *Value;
};

uint8
SimpleHash(char *StringKey)
{
    uint8 key = 0;
    for (int i = 0; StringKey[i] != '\0'; ++i)
    {
        key = ((StringKey[i] + (key << 6) + (key << 16) - key));
    }
    key &= (HASHSET_LIMIT - 1);
    return key;
}

void
HashSet_Zero_AssetTexture(HashSet_AssetTexture *CurrentHashSet)
{
    for (int i = 0; i < HASHSET_LIMIT; ++i)
    {
        StringClear(CurrentHashSet[i].Key, HASHSET_STRING_KEY_LIMIT);
        CurrentHashSet[i].Value = NULL;
    }
}

void
HashSet_Insert_AssetTexture(HashSet_AssetTexture *CurrentHashSet, char *stringKey, AssetTexture *Data)
{
    uint8 currentKey = 0;
    currentKey = SimpleHash(stringKey);
    while (CurrentHashSet[currentKey].Value != NULL)
    {
        ++currentKey;
    }
    StringCopyOverwrite(CurrentHashSet[currentKey].Key, stringKey, HASHSET_STRING_KEY_LIMIT);
    CurrentHashSet[currentKey].Value = Data;
}

AssetTexture *
HashSet_Select_AssetTexture(HashSet_AssetTexture *CurrentHashSet, char *stringKey)
{
    AssetTexture *SelectedTexture = NULL;
    uint8 currentKey = SimpleHash(stringKey);
    while (StringCompare(CurrentHashSet[currentKey].Key, stringKey) != 0)
    {
        ++currentKey;
    }
    Assert(currentKey < HASHSET_LIMIT);
    SelectedTexture = CurrentHashSet[currentKey].Value;
    return SelectedTexture;
}

uint8
HashSet_Select_AssetTextureIndex(HashSet_AssetTexture *CurrentHashSet, char *stringKey)
{
    uint8 currentKey = SimpleHash(stringKey);
    while (StringCompare(CurrentHashSet[currentKey].Key, stringKey) != 0)
    {
        ++currentKey;
    }
    Assert(currentKey < HASHSET_LIMIT);
    return currentKey;
}

void
HashSet_Delete_AssetTexture(HashSet_AssetTexture *CurrentHashSet, char *stringKey)
{
    uint8 currentKey = HashSet_Select_AssetTextureIndex(CurrentHashSet, stringKey);
    AssetTexture *SelectedTexture = CurrentHashSet[currentKey].Value;
    if (SelectedTexture != NULL && SelectedTexture->Texture != NULL)
    {
        // NOTE(nick): clear out the key and value in the hashset slot
        StringClear(CurrentHashSet[currentKey].Key, StringSize(CurrentHashSet[currentKey].Key));
        CurrentHashSet[currentKey].Value = NULL;
        // NOTE(nick): destroy the gpu texture
        SDL_DestroyTexture(SelectedTexture->Texture);
        // NOTE(nick): flag the memory block as freed
        FlagMemoryChunkAsFreed((void *)SelectedTexture, sizeof(AssetTexture));
    }
}
