#include "list.h"
#include <stddef.h>
struct listItem InitListItem(int _value, struct listItem* _next)
{
	struct listItem a;
	a.value = _value;
	a.next = _next;
	return a;
}
struct list InitList(struct listItem* _first)
{
	struct list a;
	a.first = _first;
	return a;
}
void AddToList(struct list* l, int _value)
{
	struct listItem* a;
	a->value = _value;
	a->next = l->first;
	l->first = a;
}

void DeleteFromList(struct list* l, int _value)
{
	struct listItem* lItem = l->first;
	while (lItem != NULL && lItem->value != _value)
		lItem = lItem->next;
	if (lItem != NULL)
		lItem = lItem->next;
}
