/* clone for ceph/arch , modified by zhodj, under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation
 */

#include <stdint.h>
#include "intel.h"

int arch_intel_pclmul = 0;
int arch_intel_sse42= 0;
int arch_intel_sse41= 0;
int arch_intel_ssse3= 0;
int arch_intel_sse3= 0;
int arch_intel_sse2= 0;

#ifdef LINUX
#ifdef __x86_64__
static void cpu_id(uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx)
{
    int id = *eax;
    asm("movl %4, %%eax;"
            "cpuid;"
            "movl %%eax, %0;"
            "movl %%ebx, %1;"
            "movl %%ecx, %2;"
            "movl %%edx, %3;"
            : "=r" (*eax), "=r" (*ebx), "=r" (*ecx), "=r" (*edx)
            : "r" (id)
            : "eax", "ebx", "ecx", "edx");
       
}
/*  http://en.wikipedia.org/wiki/CPUID#EAX.3D1:_Processor_Info_and_Feature_Bits */
#define CPUID_PCLMUL    (1 << 1)
#define CPUID_SSE42     (1 << 20)
#define CPUID_SSE41     (1 << 19)
#define CPUID_SSSE3     (1 << 9)
#define CPUID_SSE3      (1)
#define CPUID_SSE2      (1 << 26)

int arch_intel_probe(void)
{
    /* for x86_64 check features */
    uint32_t eax = 1, ebx, ecx, edx;
    cpu_id(&eax, &ebx, &ecx, &edx);
    if((ecx & CUPID_PCLMUL) != 0) arch_intel_pclmul = 1;
    if((ecx & CUPID_SSE42) != 0) arch_intel_sse42 = 1;
    if((ecx & CUPID_SSE41) != 0) arch_intel_sse41 = 1;
    if((ecx & CUPID_SSSE3) != 0) arch_intel_ssse3 = 1;
    if((ecx & CUPID_SSE3) != 0) arch_intel_sse3 = 1;
    if((edx & CUPID_SSE2) != 0) arch_intel_sse2 = 1;
    return 0;
}

#else
int arch_intel_probe(void)
{
    return 0;
}
#endif // __x86_64__

#else

#endif // LINUX





