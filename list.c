#define _CRT_SECURE_NO_WARNINGS
#include "list.h"
#include "siteadress.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
struct listItem* InitListItem(struct siteAdress* _value, struct listItem* _next)
{
  struct listItem* a = (struct listItem*)malloc(sizeof(struct listItem));
  a->next = (struct listItem*)malloc(sizeof(struct listItem));
  if (a == NULL)
    return NULL;
  a->value = (struct siteAdress*)malloc(sizeof(struct siteAdress));
  a->value->IPv4Adress = _value->IPv4Adress;
  strcpy(a->value->internetAdress, _value->internetAdress);
  a->next = _next;
  return a;
}
struct list* InitList(struct listItem* _first)
{
  struct list a;
  a.first = _first;
  return &a;
}
void AddToList(struct list* l, struct siteAdress* _value)
{
  struct listItem* a = (struct listItem*)malloc(sizeof(struct listItem));
  a = InitListItem(_value, l->first);
  l->first = a;
}

void DeleteFromList(struct list* l, IPADDRESS _IPv4Adress)
{
  struct listItem* lItem = l->first;
  while (lItem != NULL && lItem->value->IPv4Adress != _IPv4Adress)
    lItem = lItem->next;
  if (lItem != NULL)
    lItem = lItem->next;
}
struct listItem* FindByHostName(struct list* l, const char* hostName)
{
  if (l != NULL)
  {
    struct listItem* lItem = l->first;
    while (lItem != NULL && strcmp(lItem->value->internetAdress, hostName) != 0)
      lItem = lItem->next;
    return lItem;
  }
  return NULL;
}