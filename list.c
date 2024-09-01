#include <stdlib.h>
#include "list.h"
#include "commands.h"

void list_init(struct GradeEntry *head)
{
  head->next = head;
  head->prev = head;
}

void list_add(struct GradeEntry *cur, struct GradeEntry *new)
{
  new->next = cur->next;
  new->prev = cur;
  cur->next->prev = new;
  cur->next = new;
}

void list_remove(struct GradeEntry *cur)
{
  cur->prev->next = cur->next;
  cur->next->prev = cur->prev;
  cur->prev = NULL;
  cur->next = NULL;
}