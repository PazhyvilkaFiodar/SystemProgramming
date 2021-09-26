#ifndef LIST_H
#define LIST_H
#include <stddef.h>
#include "siteadress.h"
struct listItem
{
  struct siteAdress* value;
  struct listItem* next;
};
struct listItem* InitListItem(struct siteAdress* _value, struct listItem* _next);
struct list
{
  struct listItem* first;
};
struct list* InitList(struct listItem* _first);
void AddToList(struct list* l, struct siteAdress* _value);
void DeleteFromList(struct list* l, IPADDRESS IPv4Adress);
struct listItem* FindByHostName(struct list* l, const char* hostName);
#endif