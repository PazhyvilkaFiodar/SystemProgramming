#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "list.h"
#include "siteadress.h"
typedef unsigned int HashValue;
struct hashTable
{
  struct list hashes[15000];
};
HashValue ComputeHash(const char* string);
void AddToHashTable(struct hashTable* table, struct siteAdress* string);
struct list* FindByHash(struct hashTable* table, HashValue hash);
#endif