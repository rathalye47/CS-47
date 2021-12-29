//* CS47 - Project #3 */
//* Rohan Athalye */

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


/* This function performs searching on a list of names stored in ascending order and returns the Student ID of the student record in the list */
/* Performs comparisons in a case insensitive manner */

long search_by_name (char* list, long count, char* token)
{
	long index = 0;

asm
(
	"movq %1, %%rax;" /* Moves list to rax */           
	"movq %2, %%rbx;" /* Moves token to rbx */	 
	"movq %3, %%rcx;" /* Moves count to rcx */
	"xorq %%rdx, %%rdx;" /* Sets rdx, which is to keep track of the count, to 0 */
	"xorq %%rsi, %%rsi;" /* Sets rsi, which is the index of each character in the Student name of the list and the index of each character of the token, to 0 */
	"xorb %%r8b, %%r8b;" /* Sets r8b, which holds each character in a Student name of the list, to 0 */
	"xorb %%dil, %%dil;" /* Sets dil, which holds each character of the token, to 0 */

	"OUTER_LOOP:" /* Outer Loop Instruction */
		"addq $8, %%rax;" /* Adds 8 to rax to access the Student name */
		"cmpq %%rcx, %%rdx;" /* Compares rdx with rcx */
		"je NOT_FOUND;" /* If rdx is equal to rcx, jump to the NOT_FOUND Instruction */
		"inc %%rdx;" /* Increments rdx by 1 */
		
	"INNER_LOOP:" /* Inner Loop Instruction */
		"movb (%%rax, %%rsi, 1), %%r8b;" /* Moves the character of the Student name in the list to r8b */
		"movb (%%rbx, %%rsi, 1), %%dil;" /* Moves the character of the token to dil */
		"cmpb %%r8b, %%dil;" /* Compares dil with r8b */
		"jne CASE_INSENSITIVE;" /* If dil is not equal to r8b, jump to the CASE_INSENSITIVE Instruction */
		"inc %%rsi;" /* Increments rsi by 1 */
		"testb %%dil, %%dil;" /* Checks if dil & dil is equal to 0 to see if we reached the end of the token */
		"je FOUND;" /* If dil is equal to 0, jump to the FOUND Instruction */
		"jmp INNER_LOOP;" /* Jumps to the INNER_LOOP Instruction */

	"CASE_INSENSITIVE:" /* Case Insensitive Instruction */
		"orb $0x20, %%r8b;" /* Converts r8b to lowercase using the 'or' operation */
		"orb $0x20, %%dil;" /* Converts dil to lowercase using the 'or' operation */
		"cmpb %%r8b, %%dil;" /* Compares dil with r8b */
		"jne NEXT_STUDENT;" /* If dil is not equal to r8b, jump to the NEXT_STUDENT Instruction */
		"inc %%rsi;" /* Increments rsi by 1 */
		"testb %%dil, %%dil;" /* Checks if dil & dil is equal to 0 to see if we reached the end of the token */
		"je FOUND;" /* If dil is equal to 0, jump to the FOUND Instruction */
		"jmp INNER_LOOP;" /* Jumps to the INNER_LOOP Instruction */

	"NEXT_STUDENT:" /* Next Student Instruction */
		"xorq %%rsi, %%rsi;" /* Sets rsi to 0 each time we go to the next Student in the list */
		"addq $24, %%rax;" /* Adds 24 to rax to go to the next Student in the list */
		"jmp OUTER_LOOP;" /* Jumps to the OUTER_LOOP Instruction */

	"NOT_FOUND:" /* Not Found Instruction */
		"xorq %%rax, %%rax;" /* Sets rax to 0 */
		"jmp DONE;" /* Jumps to the DONE Instruction */

	"FOUND:" /* Found Instruction */
		"subq $8, %%rax;" /* Subtracts 8 from rax to access the Student ID */
		"movq (%%rax), %%rax;" /* Deferences the pointer and moves the value into rax */
		
	"DONE:" /* Done Instruction */
	 : "=a" (index)
	 : "r" (list), "r" (token), "r" (count)
	 : 
	 );
	 return index;	
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
