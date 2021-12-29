/* Rohan Athalye */
/* CS47 - Project #1 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Example - add function */
long add (long op1, long op2)
{
	long output=0;
asm
(
	"movl %1, %%eax;" 		
	"addl %2, %%eax;" 
	 : "=a" (output)
	 : "r" (op1), "r" (op2) 
	 : 
	 ); /* add the second operand to eax, eax has result */
	 return output;
}

/* 
1. long mult (long op1, long op2)
-	Can't use the MULQ/IMULQ instructions, meaning you use ADD repeatedly
-	If there are overflow, return the overflowed values in EAX register
*/
long mult (long op1, long op2)
{
	long output=0;
asm
(
	"movl %1, %%ebx;" /* Moves first operand to %ebx */
	"movl $0, %%eax;" /* Moves 0 to %eax */
	"cmpl $0, %2;" /* Compares second operand with 0 */
	"jl .L7;" /* If second operand is less than 0, jump to L7 instruction */
	
	".L5:"
		"cmpl $1, %2;" /* Compares second operand with 1 */
		"jl CONTINUE;" /* If second operand is less than 1, jump to CONTINUE instruction */
		"addl %%ebx, %%eax;" /* Adds %ebx to %eax */
		"subl $1, %2;" /* Subtracts 1 from second operand */
		"jmp .L5;" /* Jumps again to L5 instruction in a loop */

	".L7:"
		"negl %2;" /* Negates second operand */
		"jmp .L6;" /* Jumps to L6 instruction */
		
	".L6:"
		"cmpl $1, %2;" /* Compares second operand with 1 */
		"jl .L4;" /* If second operand is less than 1, jump to L4 instruction */
		"addl %%ebx, %%eax;" /* Adds %ebx to %eax */
		"subl $1, %2;" /* Subtracts 1 from second operand */
		"jmp .L6;" /* Jumps again to L6 instruction in a loop */

	".L4:"
		"negl %%eax;" /* Negates %eax */
		"jmp CONTINUE;" /* Jumps to CONTINUE instruction */

	"CONTINUE:"
	 : "=a" (output)
	 : "r" (op1), "r" (op2) 
	 : 
	 ); /* adds the first operand to eax in a loop "op2" number of times, eax has result */
	 return output;
}

/*
2. long XOR (long op1, long op2)
-	xor will return the result of bit exclusive OR of op1 / op2
-	can use XOR instruction
*/

long XOR (long op1, long op2)
{
	long output = 0;
asm
(
	"movl %1, %%eax;" /* Moves first operand to %eax */
	"xorl %2, %%eax;" /* Computes XOR operation on %eax and second operand */

	 : "=a" (output)
	 : "r" (op1), "r" (op2) 
	 : 
	 ); /* performs XOR operation on %eax and 2nd operand, eax has result */
	 return output;
}

/* 
3. long mod (long op1, long op2)
-	mod will return the remainder of op1 / op2
-	can use IDIVQ instruction  	"idivl 2;"
*/
long mod (long op1, long op2)
{
	long output=0;
	int local_op2=op2;
asm
(
	"movl %1, %%eax;"
	"movl %2, %%ebx;"
    "cdq;"
	"idivl %%ebx;"
	"movl %%edx, %%eax;"
	 : "=a" (output)
	 : "r" (op1), "r" (op2) 
	 :  
	 );
	 return output;
}

/* 
4. long shift (long op1, long direction, long number_of_bits)
-	shift will perform arithmetic (SAR or SAL) bit-shifting of the input operand (op1)
-	direction = 0 for left and 1 for right
-	number_of_bits will dictate how many bits to shift left or right
*/
long shift (long op1, long direction, long number_of_bits)
{
	long output=0;
	long is_left = direction == 0 ? 1 : 0;
	// printf("direction=%ld\n",is_left);
	/* move first operand to eax   */
	/* move direction flag to ebx  */
	/* move bit count to ecx, but use cl only on sar or sal commands */
	/* check if 0 (Left) */
	/* shift right if not 0  */

asm
(
	"movl %1, %%eax;"           
	"movl %2, %%ebx;" 	
	"movl %3, %%ecx;" 
	"cmpl $1, %%ebx;"
	"jz Shift_Left		;"
	"sarl %%cl, %%eax	;"		
    "jmp done;"
"Shift_Left:"
	"sall %%cl, %%eax;"
"done:"
 	 : "=a" (output)
	 : "r" (op1), "r" (is_left), "r" (number_of_bits)
	 : 
	 ); 
	 return output;
}

/*
5. long rotate (long op1, long direction, long number_of_bits)
-	rotate will perform logical bit-rotation of input operand (op1)
-	direction = 0 for left and 1 for right
-	number_of_bits will dictate how many bits to rotate left or right
-	you need to use rcl and rcr assembly instructions
*/

long rotate (long op1, long direction, long number_of_bits)
{
	long output=0;
	long is_left = direction == 0 ? 1 : 0;
	// printf("direction=%ld\n",is_left);
	/* move first operand to eax   */
	/* move direction flag to ebx  */
	/* move bit count to ecx, but use cl only on rcr or rcl commands */
	/* check if 0 (Left) */
	/* rotate right if not 0  */

asm
(
	"movl %1, %%eax;" /* Moves first operand to %eax */           
	"movl %2, %%ebx;" /* Moves direction flag to %ebx */	
	"movl %3, %%ecx;" /* Moves bit count to %ecx */
	"cmpl $1, %%ebx;" /* Compares %ebx with 1 */
	
	"jz Rotate_Left;" /* If %ebx is 0, jump to Rotate_Left instruction */
	"clc;" /* Clears the carry flag */
	"rcrl %%cl, %%eax;" /* Rotates %eax right by %cl bits */		
    	"jmp FINISHED;" /* Jumps to FINISHED instruction */

	"Rotate_Left:"
		"clc;" /* Clears the carry flag */
		"rcll %%cl, %%eax;" /* Rotates %eax left by %cl bits */

	"FINISHED:"
 	 : "=a" (output)
	 : "r" (op1), "r" (is_left), "r" (number_of_bits)
	 : 
	 ); 
	 return output;
}

/* 
6.	long factorial ( long op1 )
-	Input a positive integer (>0) and return the result of op1!
-	Must use a loop to compute the result (no recursion)
*/

long factorial (long op1)
{
	long output = 0;
asm
(
	"movl %1, %%ebx;" /* Moves first operand to %ebx */
	"movl $1, %%eax;" /* Moves 1 to %eax */

	"FACT:"
	 	"cmpl $1, %%ebx;" /* Compares %ebx with 1 */
		"jle COMPLETE;" /* If %ebx is less than or equal to 1, jump to COMPLETE instruction */
		"imull %%ebx, %%eax;" /* Multiplies %eax by %ebx */
		"subl $1, %%ebx;" /* Subtracts 1 from %ebx */
		"jmp FACT;" /* Jumps to FACT instruction again in a loop */	
	
	"COMPLETE:"
	  : "=a" (output)
	  : "r" (op1)
	  : 
	  ); /* Returns the result of op1!, eax has the result */
	  return output;
}

int main(int argc, char** argv)
{
	long op1, op2, result;

	op1 = op2 = result = 0;
	
	if (argc != 3)
	{
		printf("Usage: %s op1 op2 (two integers)\n", argv[0]);
		return 1;
	}

	op1 = atol(argv[1]);
	op2 = atol(argv[2]);

    printf("Operand 1 = %ld x%08lx  Operand 2 = %ld x%08lx\n", op1,op1,op2,op2);
	result = add(op1, op2);
	printf("Add():\t%10ld   x%08lx\n", result, result);

	result = XOR(op1, op2);
	printf("XOR():\t%10ld   x%08lx\n", result, result);

	result = mult(op1, op2);
	printf("Mult():\t%10ld   x%08lx\n", result, result);

	if (op2 == 0)
	{
	  printf("Mod Error: Divide by 0\n");
	  result = 0;
	}
	else
        {
  	  result = mod(op1, op2);
	  printf("Mod():\t%10ld   x%08lx\n", result, result);
	}

	if (op2 < 0)
	{
	  printf("Error: Shift count must be >= 0\n");
	  result = 0;
	}
	else
	{
  	  result = shift(op1, 0, op2);
	  printf("ShiftL:\t%10ld   x%08lx\n", result, result);
	  result = shift(op1, 1, op2);
	  printf("ShiftR:\t%10ld   x%08lx\n", result, result);
        }

	if (op2 < 0)
	{
	  printf("Error: Rotate count must be >= 0\n");
	  result = 0;
	}
	else
	{
  	  result = rotate(op1, 0, op2);
	  printf("RotateL:\t%10ld   x%08lx\n", result, result);
	  result = rotate(op1, 1, op2);
	  printf("RotateR:\t%10ld   x%08lx\n", result, result);
        }

	if (op1 <= 0)
	{
	  printf("Error: Factorial input must be a positive integer >=1\n");
	  result = 0;
	}
	else
	{
          result = factorial(op1);
	  printf("Fact():\t%10ld   x%08lx\n\n", result, result);
	}

	return 0;
}