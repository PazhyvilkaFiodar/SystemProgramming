#ifndef LIST_H
#define LIST_H
#include <stddef.h>
struct listItem
{
	int value;
	struct listItem* next;
};
struct listItem InitListItem(int _value, struct listItem* _next);
struct list
{
	struct listItem* first;
};
struct list InitList(struct listItem* _first);
void AddToList(struct list* l, int value);
void DeleteFromList(struct list* l, int value);
#endif