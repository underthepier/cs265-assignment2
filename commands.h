#ifndef COMMANDS_H
#define COMMANDS_H

struct GradeEntry {
  char studentId[11]; // 10 digit ID with space for \0
  char assignmentName[21]; // max 20 char name with space for \0
  unsigned short grade; // use format specifer %hu to print
  struct GradeEntry *next;
  struct GradeEntry *prev;
};

int add_entry(char *input, struct GradeEntry *head, struct GradeEntry *cur, struct GradeEntry *student);

int remove_entry(char *input, struct GradeEntry *head, struct GradeEntry *student);

int check_entry(struct GradeEntry *head, struct GradeEntry *cur, struct GradeEntry *student, const char option);

void print_entries(struct GradeEntry *head, struct GradeEntry *student);

void stats_entries(char *input, struct GradeEntry *head, struct GradeEntry *student);

void write_to_file(struct GradeEntry *head, char *input_file);

#endif