
#include <stdio.h>

typedef unsigned int uint;

int intel_pclmul = 0;
int intel_sse42= 0;
int intel_sse41= 0;
int intel_ssse3= 0;
int intel_sse3= 0;
int intel_sse2= 0;

#ifdef LINUX
#ifdef __x86_64__
static void cpu_id(uint *eax, uint *ebx, uint *ecx, uint *edx)
{
    int id = *eax;
    asm("movl %4, %%eax;"
        "cpuid;"
        "movl %%eax, %0;"
        "movl %%ebx, %1;"
        "movl %%ecx, %2;"
        "movl %%edx, %3;"
        );
        
}

#else

#endif

#else
#ifdef WIN32

#endif





