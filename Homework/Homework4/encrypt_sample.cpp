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
  long count = shiftcount % 26;     /* adjust shiftcount if > 26 */
  
asm
(
   /* Add your ASM code here !! */
	"movq %1, %%rax;"
    "movq %2, %%rbx;"
    "movq %3, %%rcx;"
    "movq %4, %%rdi;"   
    "pushq %%rdi;"
    "movq %5, %%rsi;"
"loop_start:"
    "movb (%%rax), %%dl;"
    "movb %%dl, (%%rbx);"       
    "inc %%rax;"
    "inc %%rbx;"
    "dec %%rdi;"
    "cmpq $0, %%rdi;"
    "jne loop_start;"
    "movb $0, (%%rbx);"
	"popq %%rax;"
 	 : "=a" (characters_changed)
	 : "r" (src), "m" (dest), "m" (en_flag), "m" (len), "m" (count)
	 : 
	 );  

	return characters_changed;

}