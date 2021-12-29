//* CS47 - Project #2 template */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct student
{
  long	ID;          /* 8 bytes in 64-bit, and 4 bytes in 32-bit */
  char	name[24];
};

/* This function returns the char* to the name string if the input sid has a match on the student ID
   from the array of student structures */

long search_by_id (char* list, long count, long sid)
{
	long index=0;
asm
(
	"movq %1, %%rax;"           
	"movq %2, %%rbx;" 	
	"movq %3, %%rcx;"
	"xorq %%rdx, %%rdx;"
"loop_start:"
	"cmpq %%rdx, %%rcx;"
	"je not_found;"
	"movq (%%rax), %%rsi;"
	"cmpq %%rsi, %%rbx;"
	"je found;"
	"addq $32, %%rax;"   /* 32 = size of each record for 64-bit; In 32-bit, it is 28 */ 
	"inc %%rdx;"	
    "jmp loop_start;"
"not_found:"
	"xorq %%rax, %%rax;"
	"jmp done;"
"found:"
	"addq $8, %%rax;"
"done:"
 	 : "=a" (index)
	 : "r" (list), "r" (sid), "r" (count)
	 : 
	 );
	 return index;
}

long search_by_name (char* list, long count, char* token)
{
	long index = 0;

	/* Replace the following C code with the ASM block */
	struct student *clist = (struct student*)list;
	
	for (index = 0; index < count; index++)
	  if ( strcasecmp( clist[index].name, token ) == 0)
		return clist[index].ID;

	return 0;
}

int main(int argc, char** argv)
{
	char *token;
	char *sname=NULL;
	FILE *fptr;
	long id = 0;
	long sid = 0;
	long i = 0;
	struct student list[100];	/* array of structure */
	
	if (argc != 4)
	{
		printf("Usage: %s filename token student_id\n", argv[0]);
		return 1;
	}

	token = argv[2];

	sid = atol(argv[3]);

	printf("\n");

	fptr = fopen((argv[1]), "rtc");
	if (fptr == NULL)
      printf( "File %s was not opened\n",argv[1] );
	else
   {
      /* Set pointer to beginning of file: */
      fseek( fptr, 0L, SEEK_SET );

      /* Read data from file: */
	  while ( fscanf(fptr, "%s", (char*) &list[i].name) != EOF )
	  {
	  list[i].ID = i+1001;
          printf( "%s ", list[i].name );
		  i++;
	  }

      printf( "\n\nNumber of names = %ld\n", i );
      printf( "Search Token = %s\n", token );
      fclose( fptr );
   }

	printf( "\nStudent Id = %ld; Name = %s.\n\n", sid, (char*)search_by_id((char*)list, i, sid));

   id = search_by_name ((char*)list, i, token);

   if (id)
	   printf( "Student Id = %ld; Name = %s\n", id, list[id-(1001)].name );
   else
     printf( "Student is not found.\n");

   return 0;
}



	


