/*
 * Copyright (C) 2011 by Nelson Elhage
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "x86_common.h"

#define ARCH_HAVE_MULTIPLE_PERSONALITIES

static struct ptrace_personality arch_personality[2] = {
    {
        offsetof(struct user_regs_struct, rax),
        offsetof(struct user_regs_struct, rdi),
        offsetof(struct user_regs_struct, rsi),
        offsetof(struct user_regs_struct, rdx),
        offsetof(struct user_regs_struct, r10),
        offsetof(struct user_regs_struct, r8),
        offsetof(struct user_regs_struct, r9),
        offsetof(struct user_regs_struct, rip),
    },
    {
        offsetof(struct user_regs_struct, rax),
        offsetof(struct user_regs_struct, rbx),
        offsetof(struct user_regs_struct, rcx),
        offsetof(struct user_regs_struct, rdx),
        offsetof(struct user_regs_struct, rsi),
        offsetof(struct user_regs_struct, rdi),
        offsetof(struct user_regs_struct, rbp),
        offsetof(struct user_regs_struct, rip),
    },
};

struct x86_personality x86_personality[2] = {
    {
        offsetof(struct user_regs_struct, orig_rax),
        offsetof(struct user_regs_struct, rax),
    },
    {
        offsetof(struct user_regs_struct, orig_rax),
        offsetof(struct user_regs_struct, rax),
    },
};

struct syscall_numbers arch_syscall_numbers[2] = {
#include "default-syscalls.h"
    {
        /*
         * These don't seem to be available in any convenient header. We could
         * include unistd_32.h, but those definitions would conflict with the
         * standard ones. So, let's just hardcode the values for now. Probably
         * we should generate this from unistd_32.h during the build process or
         * something.
         */
        .nr_mmap    = 90,
        .nr_mmap2   = 192,
        .nr_munmap  = 91,
        .nr_getsid  = 147,
        .nr_setsid  = 66,
        .nr_setpgid = 57,
        .nr_fork    = 2,
        .nr_wait4   = 114,
        .nr_signal  = 48,
        .nr_rt_sigaction = 174,
        .nr_openat  = 295,
        .nr_close   = 6,
        .nr_ioctl   = 54,
        .nr_dup2    = 63,
        .nr_socketcall = 102,
    }
};

int arch_get_personality(struct ptrace_child *child) {
    unsigned long cs;

    cs = ptrace_command(child, PTRACE_PEEKUSER,
                        offsetof(struct user_regs_struct, cs));
    if (child->error)
        return -1;
    if (cs == 0x23)
        child->personality = 1;
    return 0;
}
