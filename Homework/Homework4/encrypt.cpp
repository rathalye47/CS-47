//* CS47 - Project #4 */
//* Rohan Athalye */

/* 
  encrypt.cpp - This file contains the code to encrypt and decrypt an input string
                and output the modified string.
*/

#include "key.h"
#include <string.h>

__declspec() long s_encrypt (char *src, char* dest, long en_flag);
__declspec() void get_key (long *dir, long *count);
__declspec() void set_key (long dir, long count);

__declspec() void get_key (long *dir, long *count)
{
	*dir = direction;
	*count = shiftcount;
}

__declspec() void set_key (long dir, long count)
{
	direction = dir;
	shiftcount = count;
}


__declspec() long s_encrypt (char *src, char* dest, long en_flag)
{
  
  long characters_changed = 0;
  long len = strlen(src);
  long dir = direction;
  long count = shiftcount % 26; /* Adjust shiftcount if > 26 */
  
asm
(
	"movq %1, %%rax;" /* Moves src to rax */
	"movq %2, %%rbx;" /* Moves dest to rbx */
	"movq %3, %%rcx;" /* Moves en_flag, which is the encryption flag, to rcx */
	"movq %4, %%rdi;" /* Moves len to rdi */  
	"movq %5, %%rsi;" /* Moves count to rsi */
	"movq %6, %%rdx;" /* Moves dir, which is the direction, to rdx */
	"xorq %%r8, %%r8;" /* Sets r8, which is the numbers of characters changed, to 0 */
	"xorq %%r9, %%r9;" /* Sets r9, which is the index to keep track of the count, to 0 */
	"xorb %%r10b, %%r10b;" /* Sets r10b, which is the character of the source string, to 0 */
	"xorb %%r11b, %%r11b;" /* Sets r11b, which is the character of the target string, to 0 */

	"cmpq $1, %%rcx;" /* Compares rcx with 1 */
	"je COMPARE_DIRECTION;" /* If rcx is equal to 1, jump to the COMPARE_DIRECTION Instruction */
	"jmp LOOP;" /* Jump to the LOOP Instruction */

	"COMPARE_DIRECTION:" /* Compare Direction Instruction */
		"cmpq $0, %%rdx;" /* Compares rdx with 0 */
		"je UPDATE_DIRECTION;" /* If rdx is equal to 0, jump to the UPDATE_DIRECTION Instruction */
		"movq $0, %%rdx;" /* Moves 0 to rdx */
		"jmp LOOP;" /* Jump to the LOOP Instruction */

	"UPDATE_DIRECTION:" /* Update Direction Instruction */
		"movq $1, %%rdx;" /* Moves 1 to rdx */

	"LOOP:" /* Loop Instruction */
    		"cmpq %%r9, %%rdi;" /* Compares rdi with r9 */
		"jle UPDATE_DESTINATION_FINAL;" /* If rdi is less than or equal to r9, jump to the UPDATE_DESTINATION_FINAL Instruction */
		"movb (%%rax, %%r9, 1), %%r10b;" /* Moves src[i] to r10b */
		"movb (%%rax, %%r9, 1), %%r11b;" /* Moves src[i] to r11b */

	"UPPERCASE_CHECK_A:" /* UpperCase Check A Instruction */
		"cmpb $0x41, %%r10b;" /* Compares r10b with 'A' */
		"jge UPPERCASE_CHECK_Z;" /* If r10b is greater than or equal to 'A', jump to the UPPERCASE_CHECK_Z Instruction */
		"jmp LOWERCASE_CHECK_A;" /* Jump to the LOWERCASE_CHECK_A Instruction */

	"UPPERCASE_CHECK_Z:" /* UpperCase Check Z Instruction */
		"cmpb $0x5A, %%r10b;" /* Compares r10b with 'Z' */
		"jle INCREMENT_CHARACTERS_CHANGED;" /* If r10b is less than or equal to 'Z', jump to the INCREMENT_CHARACTERS_CHANGED Instruction */
		"jmp LOWERCASE_CHECK_A;" /* Jump to the LOWERCASE_CHECK_A Instruction */

	"INCREMENT_CHARACTERS_CHANGED:" /* Increment Characters Changed Instruction */
		"inc %%r8;" /* Increments r8 by 1 */

	"CHECK_DIRECTION:" /* Check Direction Instruction */
		"cmpq $0, %%rdx;" /* Compares rdx with 0 */
		"je ADD_NEW_CH;" /* If rdx is equal to 0, jump to the ADD_NEW_CH Instruction */
		"jmp SUBTRACT_NEW_CH;" /* Jump to the SUBTRACT_NEW_CH Instruction */

	"ADD_NEW_CH:" /* Add New Character Instruction */
		"addb %%sil, %%r10b;" /* Adds sil, the byte 0 register of rsi, to r10b */
		"movb %%r10b, %%r11b;" /* Moves r10b to r11b */
		"cmpb $0x5A, %%r11b;" /* Compares r11b with 'Z' */
		"jbe XOR_INSTRUCTION;" /* If r11b is less than or equal to 'Z', jump to the XOR_INSTRUCTION Instruction */ 
		"subb $26, %%r11b;" /* Subtracts 26 from r11b */
		"jmp XOR_INSTRUCTION;" /* Jump to the XOR_INSTRUCTION Instruction */

	"SUBTRACT_NEW_CH:" /* Subtract New Character Instruction */
		"subb %%sil, %%r10b;" /* Subtracts sil, the byte 0 register of rsi, from r10b */
		"movb %%r10b, %%r11b;" /* Moves r10b to r11b */
		"cmpb $0x41, %%r11b;" /* Compares r11b with 'A' */
		"jae XOR_INSTRUCTION;" /* If r11b is greater than or equal to 'A', jump to the XOR_INSTRUCTION Instruction */
		"addb $26, %%r11b;" /* Adds 26 to r11b */

	"XOR_INSTRUCTION:" /* XOR Instruction */
		"xorb $0x20, %%r11b;" /* Performs an xor operation on r11b with 0x20 */
		"jmp UPDATE_DESTINATION;" /* Jump to the UPDATE_DESTINATION Instruction */

	"LOWERCASE_CHECK_A:" /* LowerCase Check A Instruction */
		"cmpb $0x61, %%r10b;" /* Compares r10b with 'a' */
		"jge LOWERCASE_CHECK_Z;" /* If r10b is greater than or equal to 'a', jump to the LOWERCASE_CHECK_Z Instruction */
		"jmp UPDATE_DESTINATION;" /* Jump to the UPDATE_DESTINATION Instruction */

	"LOWERCASE_CHECK_Z:" /* LowerCase Check Z Instruction */
		"cmpb $0x7A, %%r10b;" /* Compares r10b with 'z' */
		"jle INCREMENT_CHARACTERS_CHANGED_2;" /* If r10b is less than or equal to 'z', jump to the INCREMENT_CHARACTERS_CHANGED_2 Instruction */
		"jmp UPDATE_DESTINATION;" /* Jump to the UPDATE_DESTINATION Instruction */

	"INCREMENT_CHARACTERS_CHANGED_2:" /* Increment Characters Changed (2) Instruction */
		"inc %%r8;" /* Increments r8 by 1 */
    		
	"CHECK_DIRECTION_2:" /* Check Direction (2) Instruction */
		"cmpq $0, %%rdx;" /* Compares rdx with 0 */
		"je ADD_NEW_CH_2;" /* If rdx is equal to 0, jump to the ADD_NEW_CH_2 Instruction */
		"jmp SUBTRACT_NEW_CH_2;" /* Jump to the SUBTRACT_NEW_CH_2 Instruction */

	"ADD_NEW_CH_2:" /* Add New Character (2) Instruction */
		"addb %%sil, %%r10b;" /* Adds sil, the byte 0 register of rsi, to r10b */
		"movb %%r10b, %%r11b;" /* Moves r10b to r11b */
		"cmpb $0x7A, %%r11b;" /* Compares r11b with 'z' */
		"jbe XOR_INSTRUCTION_2;" /* If r11b is less than or equal to 'z', jump to the XOR_INSTRUCTION_2 Instruction */
		"subb $26, %%r11b;" /* Subtracts 26 from r11b */
		"jmp XOR_INSTRUCTION_2;" /* Jump to the XOR_INSTRUCTION_2 Instruction */

	"SUBTRACT_NEW_CH_2:" /* Subtract New Character (2) Instruction */
		"subb %%sil, %%r10b;" /* Subtracts sil, the byte 0 register of rsi, from r10b */
		"movb %%r10b, %%r11b;" /* Moves r10b to r11b */
		"cmpb $0x61, %%r11b;" /* Compares r11b with 'a' */
		"jae XOR_INSTRUCTION_2;" /* If r11b is greater than or equal to 'a', jump to the XOR_INSTRUCTION_2 Instruction */
		"addb $26, %%r11b;" /* Adds 26 to r11b */

	"XOR_INSTRUCTION_2:" /* XOR (2) Instruction */
		"xorb $0x20, %%r11b;" /* Performs an xor operation on r11b with 0x20 */

	"UPDATE_DESTINATION:" /* Update Destination Instruction */
		"movb %%r11b, (%%rbx, %%r9, 1);" /* Moves r11b to dest[i] */
		"inc %%r9;" /* Increments r9 by 1 */
		"jmp LOOP;" /* Jump to the LOOP Instruction */

	"UPDATE_DESTINATION_FINAL:" /* Update Destination Final Instruction */
		"movb $0x00, (%%rbx, %%r9, 1);" /* Moves 0x00 to dest[i] */
		"movq %%r8, %%rax;" /* Moves r8 to rax */

	"DONE:" /* Done Instruction */
 	 : "=a" (characters_changed)
	 : "r" (src), "m" (dest), "m" (en_flag), "m" (len), "m" (count), "m" (dir)
	 : 
	 );  

	return characters_changed;
}
