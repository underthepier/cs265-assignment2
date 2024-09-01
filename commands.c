#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "commands.h"
#include "list.h"

int add_entry(char *input, struct GradeEntry *head, struct GradeEntry *cur, struct GradeEntry *student)
{ 
  //Get rid of the add
  char *token = strtok(input, " ");

  if(!(token = strtok(NULL, ":")))
  {
    printf("Invalid entry\n");
    return 5;
  }

  //Check valid student ID
  if (strlen(token) != 10)
  {
    printf("Invalid student ID\n");
    return 1;
  }
  else
  {
    strcpy(cur->studentId, token);
  }

  if(!(token = strtok(NULL, ":")))
  {
    printf("Invalid entry\n");
    return 5;
  }

  //Check valid assignment name
  if (strlen(token) > 20)
  {
    printf("Assignment name too long\n");
    return 2;
  }
  else
  {
    strcpy(cur->assignmentName, token);
  }

  if(!(token = strtok(NULL, ":")))
  {
    printf("Invalid entry\n");
    return 5;
  }

  //Check valid grade
  if (atoi(token) < 0 || atoi(token) > 100)
  {
    printf("Invalid grade entry\n");
    return 3;
  }
  else
  {
    unsigned short grade = (unsigned short) atoi(token);
    cur->grade = grade;
  }

  //Check if entry already exists
  if (!check_entry(head, cur, student, 'a'))
    list_add(head, cur);
  else
  {
    printf("Student %s already has a grade for %s\n", cur->studentId, cur->assignmentName);
    return 4;
  }

  //Valid entry
  return 0;
}

int remove_entry(char *input, struct GradeEntry *head, struct GradeEntry *student)
{
  struct GradeEntry *cur = malloc(sizeof(*cur));
  if(!cur)
  {
    fprintf(stderr, "Failed to allocate memory during remove_entry function\n");
    return EXIT_FAILURE;
  }

  //Get rid of the remove
  char *token = strtok(input, " ");

  if(!(token = strtok(NULL, ":")))
  {
    printf("Invalid entry\n");
    return 5;
  }

  //Check valid student ID
  if (strlen(token) != 10)
  {
    printf("Invalid student ID\n");
    return 1;
  }
  else
  {
    strcpy(cur->studentId, token);
  }

  if(!(token = strtok(NULL, "\n")))
  {
    printf("Invalid entry\n");
    return 5;
  }

  //Check valid assignment name
  if (strlen(token) > 20)
  {
    printf("Assignment name too long\n");
    return 2;
  }
  else
  {
    strcpy(cur->assignmentName, token);
  }

  if(!check_entry(head, cur, student, 'r'))
  {
    printf("Invalid entry. Check Student ID and/or assignment name again\n");
    return 3;
  }

  //Remove successful
  free(cur);
  return 0;
}

int check_entry(struct GradeEntry *head, struct GradeEntry *cur, struct GradeEntry *student, const char option)
{
  for(student = head->prev; student!=head; student = student->prev)
    {
      //Duplicate found
      if(!strcmp(student->studentId, cur->studentId) && !strcmp(student->assignmentName, cur->assignmentName))
      {
        if(option == 'a')
          return 1;

        else if (option == 'r')
        {
          list_remove(student);
          return 1;
        }
      }
    }

  //No duplicate found
  return 0;
}

void print_entries(struct GradeEntry *head, struct GradeEntry *student)
{
  printf("%-10s | %-20s | %-5s\n", "Student ID", "Assignment Name", "Grade");
  printf("------------------------------------------\n");
  for (student = head->prev; student!=head; student = student->prev)
  {
    printf("%-10s |", student->studentId);
    printf(" %-20s |", student->assignmentName);
    printf(" %hu\n", student->grade);
  }
}

void stats_entries(char *input, struct GradeEntry *head, struct GradeEntry *student)
{
  unsigned short min = 100;
  unsigned short max = 0;
  int num_grades = 0;

  float mean = 0;

  char assignment_name[21];
  //Get rid of the stats
  char *token = strtok(input, " ");

  if(!(token = strtok(NULL, "\n")))
  {
    printf("Invalid entry\n");
    return;
  }

  //Check valid assignment name
  if (strlen(token) > 20)
  {
    printf("Invalid assignment name\n");
    return;
  }
  else
  {
    strcpy(assignment_name, token);
  }

  for(student = head->prev; student!=head; student = student->prev)
    {
      if(!strcmp(student->assignmentName, assignment_name))
      {
        //Determine min
        if(student->grade < min)
          min = student->grade;

        //Determine max
        if(student->grade > max)
          max = student->grade;

        mean+=student->grade;

        num_grades++;
      }
    }

  if(!num_grades)
  {
    printf("No grades found for %s\n", assignment_name);
    return;
  }
  else
  {
    mean /= num_grades;

    printf("Grade statistics for %s\n", assignment_name);
    printf("Min: %hu\n", min);
    printf("Max: %hu\n", max);
    printf("Mean: %.2f\n", mean);
  }
}

void write_to_file(struct GradeEntry *head, char *input_file)
{
  struct GradeEntry *student;
  char new_file[] = "newdatabaseXXXXXX";
  char colon = ':';
  char newline = '\n';
  char student_grade[4];

  int filedes;
  filedes = mkstemp(new_file);

  for(student = head->prev; student!=head; student = student->prev)
  {
    size_t assignment_bytes = strlen(student->assignmentName);
    sprintf(student_grade, "%hu", student->grade);
    size_t grade_bytes = strlen(student_grade);

    if(-1 == write(filedes,student->studentId,10))
      {
          fprintf(stderr, "Error writing student id\n");
          return;
      }

      if(-1 == write(filedes,&colon,1))
      {
          fprintf(stderr, "Error writing colon\n");
          return;
      }

      if(-1 == write(filedes,student->assignmentName,assignment_bytes))
      {
          fprintf(stderr, "Error writing assignment name\n");
          return;
      }

      if(-1 == write(filedes,&colon,1))
      {
          fprintf(stderr, "Error writing colon\n");
          return;
      }

      if(-1 == write(filedes,student_grade,grade_bytes))
      {
          fprintf(stderr, "Error writing grade\n");
          return;
      }

      if(-1 == write(filedes,&newline,1))
      {
          fprintf(stderr, "Error writing newline\n");
          return;
      }
  }

  int ren = rename(new_file, input_file);
  unlink(new_file);
}
