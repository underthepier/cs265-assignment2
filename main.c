#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "list.h"
#include "commands.h"
#include "openfile.h"

int main(int argc, char *argv[]) 
{
  FILE *fp;

  fp = open(argv[1]);
  
  int ret;
  struct GradeEntry *student; 
  struct GradeEntry *head = malloc(sizeof(*head));
  if(!head)
  {
    fprintf(stderr, "Failed to allocate memory for linked list head\n");
    return EXIT_FAILURE;
  }

  list_init(head);

  //Lecture 07v2
  //Read from file and add to linked list
  while(1)
  {
    student = malloc(sizeof(*student));
    if(!student)
    {
      fprintf(stderr, "Failed to allocate memory for entry while reading input file\n");
      return EXIT_FAILURE;
    }
    ret = fscanf(fp, "%[0-9]: %[^:]: %hu\n", student->studentId, &student->assignmentName, &student->grade);

    if (ret == EOF)
    {
      free(student);
      break;
    }

    list_add(head, student);
  }

  fclose(fp);

  //Interactivity loop
  char input[40];

  while(fgets(input, sizeof(input), stdin) != NULL)
  {
    //Add
    if (!strncmp(input, "add", 3))
    {
      struct GradeEntry *cur = malloc(sizeof(*cur));
      if(!cur)
      {
        fprintf(stderr, "Failed to allocate memory while adding entry\n");
        return EXIT_FAILURE;
      }
      if(add_entry(input, head, cur, student))
      {
        free(cur);
        printf("Try again\n");
      }
    }

    //Remove
    else if(!strncmp(input, "remove", 6))
    {
      if(remove_entry(input, head, student))
        printf("Try again\n");
    } 

    //Print
    else if(!strncmp(input, "print", 5))
    {
      print_entries(head, student);
    }

    //Stats
    else if(!strncmp(input, "stats", 5))
    {
      stats_entries(input, head, student);
    }     

  }

  //Write to temp file and replace original file
  write_to_file(head, argv[1]);

  //Freeing memory
  struct GradeEntry *next;
  for (student=head->next, next=student->next; student!=head; student=next, next=student->next)
  {
    list_remove(student);
    free(student);
  }

  free(head);

  return 0;
}
