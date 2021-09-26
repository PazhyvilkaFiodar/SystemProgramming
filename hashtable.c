#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "hashtable.h"
const HashValue F = 149;
const HashValue A = 54101;
const HashValue B = 77003;
const HashValue C = 87011;
#define SIZE 15000;
HashValue ComputeHash(const char* string)
{
  HashValue hash = F;
  while (*string) {
    hash = (hash * A) ^ (*string * B);
    ++string;
  }
  hash %= C;
  return hash % SIZE;
}

void AddToHashTable(struct hashTable* table, struct siteAdress* adress)
{
  HashValue _hash = ComputeHash(adress->internetAdress);
  AddToList(&(table->hashes[_hash]), adress);
}
struct list* FindByHash(struct hashTable* table, HashValue hash)
{
  return &(table->hashes[hash]);
}