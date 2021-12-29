/* Rohan Athalye */
/* CS 47 - Project #2 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Example - add function */
long add (long op1, long op2)
{
	long output=0;
asm
(
	"movq %1, %%rax;" 		
	"addq %2, %%rax;" 
	 : "=a" (output)
	 : "r" (op1), "r" (op2) 
	 : 
	 ); /* add the second operand to eax, eax has result */
	 return output;
}

void sorter (long* list, long count, long opcode)
{

/* Move the array pointer to rax, opcode to rbx, count to rcx */
/* The following code sorts a list of integer key values in either ascending or descending order */

asm
(
	"movq %0, %%rax;" /* Moves the array pointer to rax */          
	"movq %1, %%rbx;" /* Moves opcode to rbx */	
	"movq %2, %%rcx;" /* Moves count to rcx, rcx acts as count */
	"movq %%rcx, %%rsi;" /* Moves rcx to rsi */
	"subq $1, %%rsi;" /* Subtracts 1 from rsi, rsi acts as count - 1 */
	"xorq %%rdx, %%rdx;" /* Sets rdx to 0, rdx acts as x, x is initialized to 0 */
	"movq %%rdx, %%rdi;" /* Moves rdx to rdi, rdi acts as y, y is initialized to 0 */

	"OUTER_LOOP:" /* Outer Loop Jump Instruction */
		"cmpq %%rdx, %%rsi;" /* Compares rsi with rdx */
		"je DONE;" /* If rsi is equal to rdx, jump to DONE Instruction */
		"movq %%rdx, %%rdi;" /* Moves rdx to rdi */

	"INNER_LOOP:" /* Inner Loop Jump Instruction */
		"cmpq %%rdi, %%rcx;" /* Compares rcx with rdi */
		"je UPDATE_X;" /* If rcx is equal to rdi, jump to UPDATE_X Instruction */

	"TEST_OPCODE:" /* Test Opcode Jump Instruction */
		"cmpq $1, %%rbx;" /* Compares rbx, which is the opcode, with 1 */
		"je ASCENDING;" /* If rbx is equal to 1, jump to ASCENDING Instruction */

	"DESCENDING:" /* Descending Jump Instruction */
		"movq (%%rax, %%rdx, 8), %%r12;" /* Moves type long list[x] to r12 */
		"movq (%%rax, %%rdi, 8), %%r13;" /* Moves type long list[y] to r13 */
		"cmpq %%r12, %%r13;" /* Compares r13 with r12 */
		"jg SWAP;" /* If r13 is greater than r12, jump to SWAP Instruction */
		"jmp UPDATE_Y;" /* Else, jump to UPDATE_Y Instruction */

	"ASCENDING:" /* Ascending Jump Instruction */
		"movq (%%rax, %%rdx, 8), %%r14;" /* Moves type long list[x] to r14 */
		"movq (%%rax, %%rdi, 8), %%r15;" /* Moves type long list[y] to r15 */
		"cmpq %%r14, %%r15;" /* Compares r15 with r14 */
		"jl SWAP;" /* If r15 is less than r14, jump to SWAP Instruction */
		"jmp UPDATE_Y;" /* Else, jump to UPDATE_Y Instruction */

	"SWAP:" /* Swap Jump Instruction */
		"movq (%%rax, %%rdx, 8), %%r10;" /* Moves type long list[x] to r10 */
		"movq (%%rax, %%rdi, 8), %%r11;" /* Moves type long list[y] to r11 */
		"xchgq %%r10, (%%rax, %%rdi, 8);" /* Exchanges the data of r10 with list[y] */		
		"xchgq %%r11, (%%rax, %%rdx, 8);" /* Exchanges the data of r11 with list[x] */
		"jmp UPDATE_Y;" /* Jumps to UPDATE_Y Instruction */

	"UPDATE_X:" /* Update x Jump Instruction */
		"inc %%rdx;" /* Increments rdx by 1, x++ */
		"jmp OUTER_LOOP;" /* Jumps to OUTER_LOOP Instruction */

	"UPDATE_Y:" /* Update y Jump Instruction */
		"inc %%rdi;" /* Increments rdi by 1, y++ */
		"jmp INNER_LOOP;" /* Jumps to INNER_LOOP Instruction */

	"DONE:" /* End of Assembly Code */
 	 : 
	 : "r" (list), "r" (opcode), "r" (count)
	 : 
	 ); 
}

int main(int argc, char** argv)
{
	long numlist[1000], asc;
	FILE *fptr;

	long i = 0;
	
	if (argc != 3)
	{
		printf("Usage: %s filename asc_des\n", argv[0]);
		return 1;
	}

	asc = atol (argv[2]);
	asc = (asc == 1) ? 1 : 2;

	printf("\n");

	fptr = fopen((argv[1]), "rtc");
	if (fptr == NULL)
      printf( "File %s was not opened\n",argv[1] );
	else
   {
      /* Set pointer to beginning of file: */
      fseek( fptr, 0L, SEEK_SET );

      /* Read data from file: */
	  while ( fscanf(fptr, "%ld", &numlist[i]) != EOF )
	  {
          printf( "%ld ", numlist[i] );
		  i++;
	  }

      printf( "\n\nNumber of integer = %ld\n", i );
      printf( "Ascend_or_Descend = %ld\n\n", asc );
      fclose( fptr );
   }

   sorter( numlist, i, asc);

   for (int j = 0; j < i; j++)
          printf( "%ld ", numlist[j] );

   printf("\n");

   return 0;
}
