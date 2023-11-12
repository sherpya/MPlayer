/*
 * small utility to extract CPU information
 * Used by configure to set CPU optimization levels on some operating
 * systems where /proc/cpuinfo is non-existent or unreliable.
 *
 * This file is part of MPlayer.
 *
 * MPlayer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * MPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with MPlayer; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef __MINGW32__
#define MISSING_USLEEP
#include <windows.h>
#define sleep(t) Sleep(1000*t);
#endif

#ifdef M_UNIX
typedef long long int64_t;
#define MISSING_USLEEP
#else
#include <inttypes.h>
#endif

#define CPUID_FEATURE_DEF(bit, desc, description) \
    { bit, desc }

typedef struct cpuid_regs {
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
} cpuid_regs_t;

static cpuid_regs_t
cpuid(int func, int sub) {
    cpuid_regs_t regs;
#define CPUID   ".byte 0x0f, 0xa2; "
#ifdef __x86_64__
    __asm__("mov %%rbx, %%rsi\n\t"
#else
    __asm__("mov %%ebx, %%esi\n\t"
#endif
            CPUID"\n\t"
#ifdef __x86_64__
            "xchg %%rsi, %%rbx\n\t"
#else
            "xchg %%esi, %%ebx\n\t"
#endif
            : "=a" (regs.eax), "=S" (regs.ebx), "=c" (regs.ecx), "=d" (regs.edx)
            : "0" (func), "2" (sub));
    return regs;
}


static int64_t
rdtsc(void)
{
    uint32_t hi, lo;
#define RDTSC   ".byte 0x0f, 0x31; "
    __asm__ volatile (RDTSC : "=a"(lo), "=d"(hi) : );
    return (uint64_t) hi << 32 | lo;
}

static const char*
brandname(int i)
{
    static const char* brandmap[] = {
        NULL,
        "Intel(R) Celeron(R) processor",
        "Intel(R) Pentium(R) III processor",
        "Intel(R) Pentium(R) III Xeon(tm) processor",
        "Intel(R) Pentium(R) III processor",
        NULL,
        "Mobile Intel(R) Pentium(R) III processor-M",
        "Mobile Intel(R) Celeron(R) processor"
    };

    if (i >= sizeof(brandmap))
        return NULL;
    else
        return brandmap[i];
}

static void
store32(char *d, unsigned int v)
{
    d[0] =  v        & 0xff;
    d[1] = (v >>  8) & 0xff;
    d[2] = (v >> 16) & 0xff;
    d[3] = (v >> 24) & 0xff;
}


int
main(void)
{
    cpuid_regs_t regs, regs_ext;
    char idstr[13];
    unsigned max_cpuid;
    unsigned max_ext_cpuid;
    unsigned int amd_flags;
    unsigned int amd_flags2;
    unsigned int ext_flags;
    unsigned int ext_flags2;
    unsigned int ext_flags3;
    unsigned int ext_flags4;
    const char *model_name = NULL;
    int i;
    char processor_name[49];

    regs = cpuid(0, 0);
    max_cpuid = regs.eax;
    /* printf("%d CPUID function codes\n", max_cpuid+1); */

    store32(idstr+0, regs.ebx);
    store32(idstr+4, regs.edx);
    store32(idstr+8, regs.ecx);
    idstr[12] = 0;
    printf("vendor_id\t: %s\n", idstr);

    regs_ext = cpuid((1<<31) + 0, 0);
    max_ext_cpuid = regs_ext.eax;
    if (max_ext_cpuid >= (1<<31) + 1) {
        regs_ext = cpuid((1<<31) + 1, 0);
        amd_flags = regs_ext.edx;
        amd_flags2 = regs_ext.ecx;

        if (max_ext_cpuid >= (1<<31) + 4) {
            for (i = 2; i <= 4; i++) {
                regs_ext = cpuid((1<<31) + i, 0);
                store32(processor_name + (i-2)*16, regs_ext.eax);
                store32(processor_name + (i-2)*16 + 4, regs_ext.ebx);
                store32(processor_name + (i-2)*16 + 8, regs_ext.ecx);
                store32(processor_name + (i-2)*16 + 12, regs_ext.edx);
            }
            processor_name[48] = 0;
            model_name = processor_name;
            while (*model_name == ' ') {
                model_name++;
            }
        }
    } else {
        amd_flags = 0;
        amd_flags2 = 0;
    }

    if (max_cpuid >= 7) {
        regs_ext = cpuid(7, 0);
        ext_flags = regs_ext.ebx;
        ext_flags2 = regs_ext.ecx;
        ext_flags3 = regs_ext.edx;
        if (regs_ext.eax >= 1) {
            regs_ext = cpuid(7, 1);
            ext_flags4 = regs_ext.eax;
        } else {
            ext_flags4 = 0;
        }
    } else {
        ext_flags = ext_flags2 = ext_flags3 = ext_flags4 = 0;
    }

    if (max_cpuid >= 1) {
        static struct {
            int bit;
            char *desc;
        } cap[] = {
            CPUID_FEATURE_DEF(0, "fpu", "Floating-point unit on-chip"),
            CPUID_FEATURE_DEF(1, "vme", "Virtual Mode Enhancements"),
            CPUID_FEATURE_DEF(2, "de", "Debugging Extension"),
            CPUID_FEATURE_DEF(3, "pse", "Page Size Extension"),
            CPUID_FEATURE_DEF(4, "tsc", "Time Stamp Counter"),
            CPUID_FEATURE_DEF(5, "msr", "Pentium Processor MSR"),
            CPUID_FEATURE_DEF(6, "pae", "Physical Address Extension"),
            CPUID_FEATURE_DEF(7, "mce", "Machine Check Exception"),
            CPUID_FEATURE_DEF(8, "cx8", "CMPXCHG8B Instruction Supported"),
            CPUID_FEATURE_DEF(9, "apic", "On-chip APIC Hardware Enabled"),
            CPUID_FEATURE_DEF(11, "sep", "SYSENTER and SYSEXIT"),
            CPUID_FEATURE_DEF(12, "mtrr", "Memory Type Range Registers"),
            CPUID_FEATURE_DEF(13, "pge", "PTE Global Bit"),
            CPUID_FEATURE_DEF(14, "mca", "Machine Check Architecture"),
            CPUID_FEATURE_DEF(15, "cmov", "Conditional Move/Compare Instruction"),
            CPUID_FEATURE_DEF(16, "pat", "Page Attribute Table"),
            CPUID_FEATURE_DEF(17, "pse36", "Page Size Extension 36-bit"),
            CPUID_FEATURE_DEF(18, "pn", "Processor Serial Number"),
            CPUID_FEATURE_DEF(19, "clflush", "CFLUSH instruction"),
            CPUID_FEATURE_DEF(21, "dts", "Debug Store"),
            CPUID_FEATURE_DEF(22, "acpi", "Thermal Monitor and Clock Ctrl"),
            CPUID_FEATURE_DEF(23, "mmx", "MMX Technology"),
            CPUID_FEATURE_DEF(24, "fxsr", "FXSAVE/FXRSTOR"),
            CPUID_FEATURE_DEF(25, "sse", "SSE Extensions"),
            CPUID_FEATURE_DEF(26, "sse2", "SSE2 Extensions"),
            CPUID_FEATURE_DEF(27, "ss", "Self Snoop"),
            CPUID_FEATURE_DEF(28, "ht", "Multi-threading"),
            CPUID_FEATURE_DEF(29, "tm", "Therm. Monitor"),
            CPUID_FEATURE_DEF(30, "ia64", "IA-64 Processor"),
            CPUID_FEATURE_DEF(31, "pbe", "Pend. Brk. EN."),
            { -1 }
        };
        static struct {
            int bit;
            char *desc;
        } cap2[] = {
            CPUID_FEATURE_DEF(0, "pni", "SSE3 Extensions"),
            CPUID_FEATURE_DEF(1, "pclmulqdq", "Carryless Multiplication"),
            CPUID_FEATURE_DEF(2, "dtes64", "64-bit Debug Store"),
            CPUID_FEATURE_DEF(3, "monitor", "MONITOR/MWAIT"),
            CPUID_FEATURE_DEF(4, "ds_cpl", "CPL Qualified Debug Store"),
            CPUID_FEATURE_DEF(5, "vmx", "Virtual Machine Extensions"),
            CPUID_FEATURE_DEF(6, "smx", "Safer Mode Extensions"),
            CPUID_FEATURE_DEF(7, "est", "Enhanced Intel SpeedStep Technology"),
            CPUID_FEATURE_DEF(8, "tm2", "Thermal Monitor 2"),
            CPUID_FEATURE_DEF(9, "ssse3", "Supplemental SSE3"),
            CPUID_FEATURE_DEF(10, "cid", "L1 Context ID"),
            CPUID_FEATURE_DEF(11, "sdbg", "Silicon Debug"),
            CPUID_FEATURE_DEF(12, "fma", "Fused Multiply Add"),
            CPUID_FEATURE_DEF(13, "cx16", "CMPXCHG16B Available"),
            CPUID_FEATURE_DEF(14, "xtpr", "xTPR Disable"),
            CPUID_FEATURE_DEF(15, "pdcm", "Perf/Debug Capability MSR"),
            CPUID_FEATURE_DEF(17, "pcid", "Processor-context identifiers"),
            CPUID_FEATURE_DEF(18, "dca", "Direct Cache Access"),
            CPUID_FEATURE_DEF(19, "sse4_1", "SSE4.1 Extensions"),
            CPUID_FEATURE_DEF(20, "sse4_2", "SSE4.2 Extensions"),
            CPUID_FEATURE_DEF(21, "x2apic", "x2APIC Feature"),
            CPUID_FEATURE_DEF(22, "movbe", "MOVBE Instruction"),
            CPUID_FEATURE_DEF(23, "popcnt", "Pop Count Instruction"),
            CPUID_FEATURE_DEF(24, "tsc_deadline_timer", "TSC Deadline"),
            CPUID_FEATURE_DEF(25, "aes", "AES Instruction"),
            CPUID_FEATURE_DEF(26, "xsave", "XSAVE/XRSTOR Extensions"),
            CPUID_FEATURE_DEF(27, "osxsave", "XSAVE/XRSTOR Enabled in the OS"),
            CPUID_FEATURE_DEF(28, "avx", "Advanced Vector Extension"),
            CPUID_FEATURE_DEF(29, "f16c", "Float 16 Instructions"),
            CPUID_FEATURE_DEF(30, "rdrand", "RDRAND Instruction"),
            CPUID_FEATURE_DEF(31, "hypervisor", "Reserved for Use by Hypervisor"),
            { -1 }
        };
        static struct {
            int bit;
            char *desc;
        } cap_amd[] = {
            CPUID_FEATURE_DEF(11, "syscall", "SYSCALL and SYSRET"),
            CPUID_FEATURE_DEF(19, "mp", "MP Capable"),
            CPUID_FEATURE_DEF(20, "nx", "No-Execute Page Protection"),
            CPUID_FEATURE_DEF(22, "mmxext", "MMX Technology (AMD Extensions)"),
            CPUID_FEATURE_DEF(25, "fxsr_opt", "Fast FXSAVE/FXRSTOR"),
            CPUID_FEATURE_DEF(26, "pdpe1gb", "PDP Entry for 1GiB Page"),
            CPUID_FEATURE_DEF(27, "rdtscp", "RDTSCP Instruction"),
            CPUID_FEATURE_DEF(29, "lm", "Long Mode Capable"),
            CPUID_FEATURE_DEF(30, "3dnowext", "3DNow! Extensions"),
            CPUID_FEATURE_DEF(31, "3dnow", "3DNow!"),
            { -1 }
        };
        static struct {
            int bit;
            char *desc;
        } cap_amd2[] = {
            CPUID_FEATURE_DEF(0, "lahf_lm", "LAHF/SAHF Supported in 64-bit Mode"),
            CPUID_FEATURE_DEF(1, "cmp_legacy", "Chip Multi-Core"),
            CPUID_FEATURE_DEF(2, "svm", "Secure Virtual Machine"),
            CPUID_FEATURE_DEF(3, "extapic", "Extended APIC Space"),
            CPUID_FEATURE_DEF(4, "cr8_legacy", "CR8 Available in Legacy Mode"),
            CPUID_FEATURE_DEF(5, "abm", "Advanced Bit Manipulation"),
            CPUID_FEATURE_DEF(6, "sse4a", "SSE4A Extensions"),
            CPUID_FEATURE_DEF(7, "misalignsse", "Misaligned SSE Mode"),
            CPUID_FEATURE_DEF(8, "3dnowprefetch", "3DNow! Prefetch/PrefetchW"),
            CPUID_FEATURE_DEF(9, "osvw", "OS Visible Workaround"),
            CPUID_FEATURE_DEF(10, "ibs", "Instruction Based Sampling"),
            CPUID_FEATURE_DEF(11, "xop", "XOP Extensions"),
            CPUID_FEATURE_DEF(12, "skinit", "SKINIT, STGI, and DEV Support"),
            CPUID_FEATURE_DEF(13, "wdt", "Watchdog Timer Support"),
            CPUID_FEATURE_DEF(15, "lwp", "Lightweight Profiling"),
            CPUID_FEATURE_DEF(16, "fma4", "Fused Multiple Add with 4 Operands"),
            CPUID_FEATURE_DEF(17, "tce", "Translation cache extension"),
            CPUID_FEATURE_DEF(19, "nodeid_msr", "Support for MSRC001_100C"),
            CPUID_FEATURE_DEF(21, "tbm", "Trailing Bit Manipulation"),
            CPUID_FEATURE_DEF(22, "topoext", "CPUID Fn 8000001d - 8000001e"),
            CPUID_FEATURE_DEF(23, "perfctr_core", "Core performance counter"),
            CPUID_FEATURE_DEF(24, "perfctr_nb", "NB performance counter"),
            CPUID_FEATURE_DEF(26, "bpext", "Data breakpoint extensions"),
            CPUID_FEATURE_DEF(27, "perftsc", "Performance TCS"),
            CPUID_FEATURE_DEF(28, "perfctr_l2", "L2 performance counter"),
            CPUID_FEATURE_DEF(29, "mwaitx", "MONITORX/MWAITX"),
            { -1 }
        };
        static struct {
            int bit;
            char *desc;
        } cap_ext[] = {
            CPUID_FEATURE_DEF(0, "fsgsbase", "{RD/WR}{FS/GS}BASE instructions"),
            CPUID_FEATURE_DEF(1, "tsc_adjust", "TSC adjustment MSR 0x3b"),
            CPUID_FEATURE_DEF(2, "sgx", "Software Guard Extensions"),
            CPUID_FEATURE_DEF(3, "bmi1", "1st group bit manipulation"),
            CPUID_FEATURE_DEF(4, "hle", "Hardware Lock Elision"),
            CPUID_FEATURE_DEF(5, "avx2", "AVX2 instructions"),
            CPUID_FEATURE_DEF(7, "smep", "Supervisor mode execution protection"),
            CPUID_FEATURE_DEF(8, "bmi2", "2nd group bit manipulation"),
            CPUID_FEATURE_DEF(9, "erms", "Enhanced REP MOVSB/STOSB"),
            CPUID_FEATURE_DEF(10, "invpcid", "Invalidate processor context ID"),
            CPUID_FEATURE_DEF(11, "rtm", "Restricted Transactional Memory"),
            CPUID_FEATURE_DEF(12, "cqm", "Cache QoS Monitoring"),
            CPUID_FEATURE_DEF(14, "mpx", "Memory Protection Extension"),
            CPUID_FEATURE_DEF(16, "avx512f", "AVX-512 Foundation"),
            CPUID_FEATURE_DEF(17, "avx512dq", "AVX-512 Double/Quad granular"),
            CPUID_FEATURE_DEF(18, "rdseed", "The RDSEED instruction"),
            CPUID_FEATURE_DEF(19, "adx", "ADCX and ADOX instructions"),
            CPUID_FEATURE_DEF(20, "smap", "Supservisor mode access prevention"),
            CPUID_FEATURE_DEF(21, "avx512ifma", "AVX-512 Integer FMA"),
            CPUID_FEATURE_DEF(22, "pcommit", "PCOMMIT instruction"),
            CPUID_FEATURE_DEF(23, "clflushopt", "CLFLUSHOPT instruction"),
            CPUID_FEATURE_DEF(24, "clwb", "CLWB instruction"),
            CPUID_FEATURE_DEF(26, "avx512pf", "AVX-512 Prefetch"),
            CPUID_FEATURE_DEF(27, "avx512er", "AVX-512 Exponential and Reciprocal"),
            CPUID_FEATURE_DEF(28, "avx512cd", "AVX-512 Conflict Detection"),
            CPUID_FEATURE_DEF(29, "sha_ni", "SHA extensions"),
            CPUID_FEATURE_DEF(30, "avx512bw", "AVX-512 Byte/Word granular"),
            CPUID_FEATURE_DEF(31, "avx512vl", "AVX-512 128/256 Vector Length"),
            { -1 }
        };
        static struct {
            int bit;
            char *desc;
        } cap_ext2[] = {
            CPUID_FEATURE_DEF(1, "avx512vbmi", "AVX-512 Vector Bit Manipulation"),
            CPUID_FEATURE_DEF(2, "umip", "User-mode instruction prevention"),
            CPUID_FEATURE_DEF(3, "pku", "Protection Keys for Userspace"),
            CPUID_FEATURE_DEF(4, "ospke", "OS Protection Keys Enable"),
            CPUID_FEATURE_DEF(6, "avx512_vbmi2", "AVX-512 Vector Bit Manipulation 2"),
            CPUID_FEATURE_DEF(8, "gfni", "Galois Field instructions"),
            CPUID_FEATURE_DEF(9, "vaes", "VEX-256/EVEX AES"),
            CPUID_FEATURE_DEF(10, "vpclmulqdq", "Carryless Multiplication Quadword"),
            CPUID_FEATURE_DEF(11, "avx512_vnni", "Vector Neural Network Instructions"),
            CPUID_FEATURE_DEF(12, "avx512_bitalg", "Support for VPOPCNT[B,W] and VPSHUF-BITQMB"),
            CPUID_FEATURE_DEF(13, "tme", "Intel total memory encryption"),
            CPUID_FEATURE_DEF(14, "avx512_vpopcntdq", "POPCNT for vectors of DW/QW"),
            CPUID_FEATURE_DEF(16, "la57", "5-level page tables"),
            CPUID_FEATURE_DEF(22, "rdpid", "Read Processor ID"),
            CPUID_FEATURE_DEF(24, "bus_lock_detect", "Bus lock detect"),
            CPUID_FEATURE_DEF(25, "cldemote", "CLDEMOTE instruction"),
            CPUID_FEATURE_DEF(27, "movdiri", "MOVDIRI instruction"),
            CPUID_FEATURE_DEF(28, "movdir64b", "MOVDIR64B instruction"),
            CPUID_FEATURE_DEF(29, "enqcmd", "ENQCMD and ENQCMDS instructions"),
            CPUID_FEATURE_DEF(30, "sgx_lc", "SGX launch configuration"),
            { -1 }
        };
        static struct {
            int bit;
            char *desc;
        } cap_ext3[] = {
            CPUID_FEATURE_DEF(2, "avx512_4vnniw", "4 Register AVX-512 Neural Network Instructions"),
            CPUID_FEATURE_DEF(3, "avx512_4fmaps", "4 Register AVX-512 Multiply Accumulation Single Precision"),
            CPUID_FEATURE_DEF(4, "fsrm", "Fast short REP MOV"),
            CPUID_FEATURE_DEF(8, "avx512_vp2intersect", "AVX-512 Intersect for D/Q"),
            CPUID_FEATURE_DEF(10, "md_clear", "VERW clears CPU buffers"),
            CPUID_FEATURE_DEF(14, "serialize", "SERIALIZE instruction"),
            CPUID_FEATURE_DEF(16, "tsxldtrk", "TSX suspend load address tracking"),
            CPUID_FEATURE_DEF(18, "pconfig", "Intel PCONFIG"),
            CPUID_FEATURE_DEF(19, "arch_lbr", "Intel ARCH LBR"),
            CPUID_FEATURE_DEF(23, "avx512_fp16", "AVX512 FP16"),
            CPUID_FEATURE_DEF(28, "flush_l1d", "Flush L1D cache"),
            CPUID_FEATURE_DEF(29, "arch_capabilities", "Intel IA32_ARCH_CAPABILITIES MSR"),
            { -1 }
        };
        static struct {
            int bit;
            char *desc;
        } cap_ext4[] = {
            CPUID_FEATURE_DEF(4, "avx_vnni", "AVX VNNI instructions"),
            CPUID_FEATURE_DEF(5, "avx512_bf16", "AVX512 BFLOAT16 instructions"),
            { -1 }
        };

        unsigned int family, model, stepping;

        regs = cpuid(1, 0);
        family = (regs.eax >> 8) & 0xf;
        model = (regs.eax >> 4) & 0xf;
        stepping = regs.eax & 0xf;

        if (family == 0xf || family == 6)
            model += ((regs.eax >> 16) & 0xf) << 4;
        if (family == 0xf)
            family += (regs.eax >> 20) & 0xff;

        printf("cpu family\t: %d\n"
               "model\t\t: %d\n"
               "stepping\t: %d\n" ,
               family,
               model,
               stepping);

        if (strstr(idstr, "Intel") && !model_name) {
            if (family == 6 && model == 0xb && stepping == 1)
                model_name = "Intel (R) Celeron (R) processor";
            else
                model_name = brandname(regs.ebx & 0xf);
        }

        printf("flags\t\t:");
        for (i = 0; cap[i].bit >= 0; i++) {
            if (regs.edx & (1 << cap[i].bit)) {
                printf(" %s", cap[i].desc);
            }
        }
        for (i = 0; cap2[i].bit >= 0; i++) {
            if (regs.ecx & (1 << cap2[i].bit)) {
                printf(" %s", cap2[i].desc);
            }
        }
        /* k6_mtrr is supported by some AMD K6-2/K6-III CPUs but
           it is not indicated by a CPUID feature bit, so we
           have to check the family, model and stepping instead. */
        if (strstr(idstr, "AMD") &&
            family == 5 &&
            (model >= 9 || (model == 8 && stepping >= 8)))
            printf(" %s", "k6_mtrr");
        /* similar for cyrix_arr. */
        if (strstr(idstr, "Cyrix") &&
            (family == 5 && (model < 4 || family == 6)))
            printf(" %s", "cyrix_arr");
        /* as well as centaur_mcr. */
        if (strstr(idstr, "Centaur") &&
            family == 5)
            printf(" %s", "centaur_mcr");

        for (i = 0; cap_amd[i].bit >= 0; i++) {
            if (amd_flags & (1 << cap_amd[i].bit)) {
                printf(" %s", cap_amd[i].desc);
            }
        }
        for (i = 0; cap_amd2[i].bit >= 0; i++) {
            if (amd_flags2 & (1 << cap_amd2[i].bit)) {
                printf(" %s", cap_amd2[i].desc);
            }
        }
        for (i = 0; cap_ext[i].bit >= 0; i++) {
            if (ext_flags & (1 << cap_ext[i].bit)) {
                printf(" %s", cap_ext[i].desc);
            }
        }
        for (i = 0; cap_ext2[i].bit >= 0; i++) {
            if (ext_flags2 & (1 << cap_ext2[i].bit)) {
                printf(" %s", cap_ext2[i].desc);
            }
        }
        for (i = 0; cap_ext3[i].bit >= 0; i++) {
            if (ext_flags3 & (1 << cap_ext3[i].bit)) {
                printf(" %s", cap_ext3[i].desc);
            }
        }
        for (i = 0; cap_ext4[i].bit >= 0; i++) {
            if (ext_flags4 & (1 << cap_ext4[i].bit)) {
                printf(" %s", cap_ext4[i].desc);
            }
        }
        printf("\n");

        if (regs.edx & (1 << 4)) {
            int64_t tsc_start, tsc_end;
            struct timeval tv_start, tv_end;
            int usec_delay;

            tsc_start = rdtsc();
            gettimeofday(&tv_start, NULL);
#ifdef  MISSING_USLEEP
            sleep(1);
#else
            usleep(100000);
#endif
            tsc_end = rdtsc();
            gettimeofday(&tv_end, NULL);

            usec_delay = 1000000 * (tv_end.tv_sec - tv_start.tv_sec)
                + (tv_end.tv_usec - tv_start.tv_usec);

            printf("cpu MHz\t\t: %.3f\n",
                   (double)(tsc_end-tsc_start) / usec_delay);
        }
    }

    printf("model name\t: ");
    if (model_name)
        printf("%s\n", model_name);
    else
        printf("Unknown %s CPU\n", idstr);
}
