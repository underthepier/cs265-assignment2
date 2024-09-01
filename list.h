#ifndef LIST_H
#define LIST_H
#include "commands.h"


void list_init(struct GradeEntry *head);

void list_add(struct GradeEntry *cur, struct GradeEntry *new);

void list_remove(struct GradeEntry *cur);

#endif