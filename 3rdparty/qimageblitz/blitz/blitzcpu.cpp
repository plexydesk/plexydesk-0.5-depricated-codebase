#include "blitzcpu.h"
#include <config-processor.h>
#include <csignal>
#include <csetjmp>

/**
 * CPUID code: This tests the CPU id flags for MMX, SSE, 3dnow, etc...
 * support. This is based on KCPUInfo by Fredrik Hoglund.
 *
 * Copyright (C) 2003 Fredrik Hoglund <fredrik@kde.org>
 * Copyright (C) 2004 Daniel M. Duley <daniel.duley@verizon.net>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#if defined(__i386__) && ( defined(__GNUC__) || defined(__INTEL_COMPILER) )
#  if defined(HAVE_MMX)
#    define USE_MMX_INLINE_ASM
#  endif
#endif

#ifdef USE_MMX_INLINE_ASM
// Sighandler for the SSE OS support check
typedef void (*blitz_sighandler_t)(int);
static jmp_buf env;
static void blitz_sighandler(int)
{
    std::longjmp(env, 1);
}
#elif defined __PPC__ && defined HAVE_PPC_ALTIVEC
// Sighandler for PPC AltiVec check
static sigjmp_buf jmpbuf;
static sig_atomic_t canjump = 0;
static void blitz_sighandler(int sig)
{
    if(!canjump) {
        signal(sig, SIG_DFL);
        raise(sig);
    }
    canjump = 0;
    siglongjmp(jmpbuf, 1);
}
#endif

static unsigned int checkCPUID()
{
    volatile unsigned int flags = 0;
#ifdef USE_MMX_INLINE_ASM // Intel/AMD checks
    bool hasCPUID = false;
    unsigned int result = 0;

    __asm__ __volatile__
        (// Try to toggle the CPUID bit in the EFLAGS register
         "pushf                      \n\t"   // Push the EFLAGS register onto the stack
         "popl   %%ecx               \n\t"   // Pop the value into ECX
         "movl   %%ecx, %%edx        \n\t"   // Copy ECX to EDX
         "xorl   $0x00200000, %%ecx  \n\t"   // Toggle bit 21 (CPUID) in ECX
         "pushl  %%ecx               \n\t"   // Push the modified value onto the stack
         "popf                       \n\t"   // Pop it back into EFLAGS

         // Check if the CPUID bit was successfully toggled
         "pushf                      \n\t"   // Push EFLAGS back onto the stack
         "popl   %%ecx               \n\t"   // Pop the value into ECX
         "xorl   %%eax, %%eax        \n\t"   // Zero out the EAX register
         "cmpl   %%ecx, %%edx        \n\t"   // Compare ECX with EDX
         "je    .Lno_cpuid_support%= \n\t"   // Jump if they're identical
         "movl      $1, %%eax        \n\t"   // Set EAX to true
         ".Lno_cpuid_support%=:      \n\t"
         : "=a"(hasCPUID) : : "%ecx", "%edx" );
    if(!hasCPUID)
        return(flags);

    // Execute CPUID with the feature request bit set
    __asm__ __volatile__
        ("pushl  %%ebx               \n\t"   // Save EBX
         "movl      $1, %%eax        \n\t"   // Set EAX to 1 (features request)
         "cpuid                      \n\t"   // Call CPUID
         "popl   %%ebx               \n\t"   // Restore EBX
         : "=d"(result) : : "%eax", "%ecx");

    // Test bit 23 (MMX support)
    // 25: SSE
    // 26: SSE2
    if(result & 0x00800000)
        flags |= BlitzCPUInfo::MMX;
    if(result & 0x00200000)
        flags |= BlitzCPUInfo::SSE;
    if(result & 0x00400000)
        flags |= BlitzCPUInfo::SSE2;

    // Now try extended feature request
    result = 0;
    __asm__ __volatile__
        ("pushl %%ebx             \n\t"
         "movl $0x80000000, %%eax \n\t"
         "cpuid                   \n\t"
         "cmpl $0x80000000, %%eax \n\t"
         "jbe .Lno_extended%=     \n\t"
         "movl $0x80000001, %%eax \n\t"
         "cpuid                   \n\t"
         ".Lno_extended%=:        \n\t"
         "popl   %%ebx            \n\t"   // Restore EBX
         : "=d"(result) : : "%eax", "%ecx");

    // Bit 31: 3dNow
    // Bit 30: 3dNow+
    // Bit 22: MMX SSE (Integer SSE) - AMD Only
    if(result & 0x80000000){
        flags |= BlitzCPUInfo::AMD3DNOW;
        if(result & 0x40000000)
            flags |= BlitzCPUInfo::AMD3DNOW2;
        if(result & 0x00400000) // This is only valid on AMD cpu's w/ 3dnow
            flags |= BlitzCPUInfo::IntegerSSE;
    }
    else if(flags & BlitzCPUInfo::SSE)
        flags |= BlitzCPUInfo::IntegerSSE;

    if((flags & BlitzCPUInfo::SSE) || (flags & BlitzCPUInfo::SSE2)){
        // SSE OS support check
        // Install our own sighandler for SIGILL.
        blitz_sighandler_t oldhandler =
            std::signal(SIGILL, blitz_sighandler);

        // Try executing an SSE insn to see if we get a SIGILL
        if(setjmp(env)){
            // No go. MMX-SSE should still be okay, tho.
            if(flags & BlitzCPUInfo::SSE)
                flags -= BlitzCPUInfo::SSE;
            if(flags & BlitzCPUInfo::SSE2)
                flags -= BlitzCPUInfo::SSE2;
        }
        else
            __asm__ __volatile__("xorps %xmm0, %xmm0");

        // Restore the default sighandler
        std::signal( SIGILL, oldhandler );
    }

#elif defined __PPC__ && defined HAVE_PPC_ALTIVEC
    signal(SIGILL, blitz_sighandler);
    if(sigsetjmp(jmpbuf, 1)){
        signal(SIGILL, SIG_DFL);
    }
    else{
        canjump = 1;
        __asm__ __volatile__( "mtspr 256, %0\n\t"
                             "vand %%v0, %%v0, %%v0"
                             : /* none */
                             : "r" (-1) );
        signal(SIGILL, SIG_DFL);
        flags |= BlitzCPUInfo::AltiVec;
    }
#endif
    return(flags);
}

bool BlitzCPUInfo::haveExtension(unsigned int extension)
{
    static bool checked = false;
    static unsigned int flags = 0;
    if(!checked){
        flags = checkCPUID();
        checked = true;
        qWarning("MMX: %d, SSE: %d, SSE2: %d, MMX-SSE: %d, 3dNow: %d, 3dNow+: %d",
                 haveExtension(BlitzCPUInfo::MMX),
                 haveExtension(BlitzCPUInfo::SSE),
                 haveExtension(BlitzCPUInfo::SSE2),
                 haveExtension(BlitzCPUInfo::IntegerSSE),
                 haveExtension(BlitzCPUInfo::AMD3DNOW),
                 haveExtension(BlitzCPUInfo::AMD3DNOW2));
    }
    return(flags & extension);
}

