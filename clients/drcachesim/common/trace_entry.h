/* **********************************************************
 * Copyright (c) 2015-2025 Google, Inc.  All rights reserved.
 * **********************************************************/

/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Google, Inc. nor the names of its contributors may be
 *   used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL VMWARE, INC. OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

/* This is the binary data format for what we send through IPC between the
 * memory tracing clients running inside the application(s) and the simulator
 * process.
 * We aren't bothering to pack it as it won't be over the network or persisted.
 * It's already arranged to minimize padding.
 * We do save space using heterogeneous data via the type field to send
 * thread id data only periodically rather than paying for the cost of a
 * thread id field in every entry.
 */

#ifndef _TRACE_ENTRY_H_
#define _TRACE_ENTRY_H_ 1

#include <memory>
#include <stddef.h>
#include <stdint.h>

#include "utils.h"

#if defined(BUILD_PT_TRACER) || defined(BUILD_PT_POST_PROCESSOR)
#    include "drpttracer_shared.h"
#endif

/**
 * @file drmemtrace/trace_entry.h
 * @brief DrMemtrace trace entry enum types and definitions.
 */

namespace dynamorio {  /**< General DynamoRIO namespace. */
namespace drmemtrace { /**< DrMemtrace tracing + simulation infrastructure namespace. */

typedef uintptr_t addr_t; /**< The type of a memory address. */

/**
 * The version number of the trace format.
 * This is presented to analysis tools as a marker of type
 * #TRACE_MARKER_TYPE_VERSION.
 */
typedef enum {
    /**
     * A prior version where #TRACE_MARKER_TYPE_KERNEL_EVENT
     * provided the module offset (and nothing for restartable sequence aborts) rather
     * than the absolute PC of the interruption point provided today.
     */
    TRACE_ENTRY_VERSION_NO_KERNEL_PC = 2,
    /**
     * #TRACE_MARKER_TYPE_KERNEL_EVENT records provide the absolute
     * PC of the interruption point.
     */
    TRACE_ENTRY_VERSION_KERNEL_PC = 3,
    /**
     * The trace supports embedded instruction encodings, but they are only present
     * if #OFFLINE_FILE_TYPE_ENCODINGS is set.
     */
    TRACE_ENTRY_VERSION_ENCODINGS = 4,
    /**
     * The trace includes branch taken and target information up front.  This means that
     * conditional branches use either #TRACE_TYPE_INSTR_TAKEN_JUMP or
     * #TRACE_TYPE_INSTR_UNTAKEN_JUMP and that the target of indirect branches is in a
     * new field "indirect_branch_target" in #memref_t.
     * This only applies to offline traces whose instructions
     * are not filtered; online traces, and i-filtered offline traces, even at this
     * version, do not contain this information.
     */
    TRACE_ENTRY_VERSION_BRANCH_INFO = 5,
    /**
     * The trace contains additional timestamps to identify and distinguish application
     * instruction execution, application syscall invocation, and trace i/o.  The
     * pre-syscall and post-syscall timestamps are as expected.  Prior versions have the
     * post-syscall timestamp actually containing the pre-syscall time.
     */
    TRACE_ENTRY_VERSION_FREQUENT_TIMESTAMPS = 6,
    /*
     * The trace supports #TRACE_MARKER_TYPE_UNCOMPLETED_INSTRUCTION. The marker is used
     * to indicate an instruction started to execute but didn't retire. The instruction
     * was either preempted by an asynchronous signal or caused a fault. The instruction
     * and corresponding memrefs are removed from the trace.
     *
     * The marker value is the raw encoding bytes of the instruction up to the
     * length of a pointer. The encoding will be incomplete for instructions
     * with long encodings. It is best-effort to help understand the sequence of
     * generated code where encodings are not available offline. The PC of this
     * instruction is available in a subsequent
     * #dynamorio::drmemtrace::TRACE_MARKER_TYPE_KERNEL_EVENT marker.
     */
    TRACE_ENTRY_VERSION_RETIRED_INSTRUCTIONS_ONLY =
        7, /**< Trace version which has only retired instructions in drmemtraces.*/
    /** The latest version of the trace format. */
    TRACE_ENTRY_VERSION = TRACE_ENTRY_VERSION_RETIRED_INSTRUCTIONS_ONLY,
} trace_version_t;

/** The type of a trace entry in a #memref_t structure. */
// The type identifier for trace entries in the raw trace_entry_t passed to
// reader_t and the exposed #memref_t passed to analysis tools.
// XXX: if we want to rely on a recent C++ standard we could try to get
// this enum to be just 2 bytes instead of an int and give it qualified names.
// N.B.: when adding new values, be sure to update trace_type_names[].
typedef enum {
    // XXX: if we want to include OP_ opcode values, we should have them occupy
    // the first 1,000 or so values and use OP_AFTER_LAST for the first of the
    // values below.  That would require including dr_api.h though.  For now we
    // assume we don't need the opcode.

    // These entries describe a memory reference as data:
    TRACE_TYPE_READ,  /**< A data load. */
    TRACE_TYPE_WRITE, /**< A data store. */

    TRACE_TYPE_PREFETCH, /**< A general prefetch. */

    // X86 specific prefetch
    TRACE_TYPE_PREFETCHT0, /**< An x86 prefetch to all levels of the cache. */
    TRACE_TYPE_PREFETCH_READ_L1 =
        TRACE_TYPE_PREFETCHT0, /**< Load prefetch to L1 cache. */
    TRACE_TYPE_PREFETCHT1,     /**< An x86 prefetch to level 2 cache and higher. */
    TRACE_TYPE_PREFETCH_READ_L2 =
        TRACE_TYPE_PREFETCHT1, /**< Load prefetch to L2 cache. */
    TRACE_TYPE_PREFETCHT2,     /**< An x86 prefetch to level 3 cache and higher. */
    TRACE_TYPE_PREFETCH_READ_L3 =
        TRACE_TYPE_PREFETCHT2, /**< Load prefetch to L3 cache. */
    // This prefetches data into a non-temporal cache structure and into a location
    // close to the processor, minimizing cache pollution.
    TRACE_TYPE_PREFETCHNTA, /**< An x86 non-temporal prefetch. */

    // ARM specific prefetch
    TRACE_TYPE_PREFETCH_READ,  /**< An ARM load prefetch. */
    TRACE_TYPE_PREFETCH_WRITE, /**< An ARM store prefetch. */
    TRACE_TYPE_PREFETCH_INSTR, /**< An ARM insruction prefetch. */

    // These entries describe an instruction fetch memory reference.
    // The trace_entry_t stream always has the instr fetch prior to data refs,
    // which the reader can use to obtain the PC for data references.
    // For memref_t, the instruction address is in the addr field.
    // The base type is an instruction *not* of the other sub-types.
    // Enum value == 10.
    TRACE_TYPE_INSTR, /**< A non-branch instruction. */
    // Particular categories of instructions:
    TRACE_TYPE_INSTR_DIRECT_JUMP,   /**< A direct unconditional jump instruction. */
    TRACE_TYPE_INSTR_INDIRECT_JUMP, /**< An indirect jump instruction. */
    /**
     * A direct conditional jump instruction.  \deprecated For offline non-i-filtered
     * traces, this is deprecated and is only present in versions below
     * #TRACE_ENTRY_VERSION_BRANCH_INFO.  Newer version used
     * #TRACE_TYPE_INSTR_TAKEN_JUMP and #TRACE_TYPE_INSTR_UNTAKEN_JUMP instead.
     */
    TRACE_TYPE_INSTR_CONDITIONAL_JUMP,
    TRACE_TYPE_INSTR_DIRECT_CALL,   /**< A direct call instruction. */
    TRACE_TYPE_INSTR_INDIRECT_CALL, /**< An indirect call instruction. */
    TRACE_TYPE_INSTR_RETURN,        /**< A return instruction. */
    // These entries describe a bundle of consecutive instruction fetch
    // memory references.  The trace stream always has a single instr fetch
    // prior to instr bundles which the reader can use to obtain the starting PC.
    // This entry type is hidden by reader_t and expanded into a series of
    // TRACE_TYPE_INSTR* entries for memref_t.
    TRACE_TYPE_INSTR_BUNDLE,

    // A cache flush:
    // On ARM, a flush is requested via a SYS_cacheflush system call,
    // and the flush size could be larger than USHRT_MAX.
    // If the size is smaller than USHRT_MAX, we use one entry with non-zero size.
    // Otherwise, we use two entries, one entry has type TRACE_TYPE_*_FLUSH for
    // the start address of flush, and one entry has type TRACE_TYPE_*_FLUSH_END
    // for the end address (exclusive) of flush.
    // The size field of both entries should be 0.
    // The _END entries are hidden by reader_t as memref_t has space for the size.
    TRACE_TYPE_INSTR_FLUSH, /**< An instruction cache flush. */
    TRACE_TYPE_INSTR_FLUSH_END,
    // Enum value == 20.
    TRACE_TYPE_DATA_FLUSH, /**< A data cache flush. */
    TRACE_TYPE_DATA_FLUSH_END,

    // These entries indicate that all subsequent memory references (until the
    // next entry of this type) came from the thread whose id is in the addr
    // field.
    // These entries are hidden by reader_t and turned into memref_t.tid.
    TRACE_TYPE_THREAD,

    // This entry indicates that the thread whose id is in the addr field exited:
    TRACE_TYPE_THREAD_EXIT, /**< A thread exit. */

    // These entries indicate which process the current thread belongs to.
    // The process id is in the addr field.
    // These entries are hidden by reader_t and turned into memref_t.pid.
    TRACE_TYPE_PID,

    // The initial entry in an offline file.  It stores the version (should
    // match TRACE_ENTRY_VERSION) in the addr field.  Unused for pipes.
    TRACE_TYPE_HEADER,

    /**
     * The final entry in an offline file or a pipe.  Not exposed to tools.
     * This can be in the middle of a derived trace when existing traces are
     * combined into a new trace, as happens with core-sharded-on-disk traces
     * produced by the record_filter tool in core-sharded mode.
     */
    TRACE_TYPE_FOOTER,

    /** A hardware-issued prefetch (generated after tracing by a cache simulator). */
    TRACE_TYPE_HARDWARE_PREFETCH,

    /**
     * A marker containing metadata about this point in the trace.
     * It includes a marker sub-type #trace_marker_type_t and a
     * value.
     */
    TRACE_TYPE_MARKER,

    /**
     * For core simulators, a trace includes instructions that do not incur
     * instruction cache fetches, such as on each subsequent iteration of a
     * rep string loop on x86.
     */
    TRACE_TYPE_INSTR_NO_FETCH,
    // An internal value used for online traces and turned by reader_t into
    // either TRACE_TYPE_INSTR or TRACE_TYPE_INSTR_NO_FETCH.
    // Enum value == 30.
    TRACE_TYPE_INSTR_MAYBE_FETCH,

    /**
     * We separate out the x86 sysenter instruction as it has a hardcoded
     * return point that shows up as a discontinuity in the user mode program
     * counter execution sequence.
     */
    TRACE_TYPE_INSTR_SYSENTER,

    // Architecture-agnostic trace entry types for prefetch instructions.
    TRACE_TYPE_PREFETCH_READ_L1_NT, /**< Non-temporal load prefetch to L1 cache. */
    TRACE_TYPE_PREFETCH_READ_L2_NT, /**< Non-temporal load prefetch to L2 cache. */
    TRACE_TYPE_PREFETCH_READ_L3_NT, /**< Non-temporal load prefetch to L3 cache. */

    TRACE_TYPE_PREFETCH_INSTR_L1,    /**< Instr prefetch to L1 cache. */
    TRACE_TYPE_PREFETCH_INSTR_L1_NT, /**< Non-temporal instr prefetch to L1 cache. */
    TRACE_TYPE_PREFETCH_INSTR_L2,    /**< Instr prefetch to L2 cache. */
    TRACE_TYPE_PREFETCH_INSTR_L2_NT, /**< Non-temporal instr prefetch to L2 cache. */
    TRACE_TYPE_PREFETCH_INSTR_L3,    /**< Instr prefetch to L3 cache. */
    TRACE_TYPE_PREFETCH_INSTR_L3_NT, /**< Non-temporal instr prefetch to L3 cache. */

    TRACE_TYPE_PREFETCH_WRITE_L1,    /**< Store prefetch to L1 cache. */
    TRACE_TYPE_PREFETCH_WRITE_L1_NT, /**< Non-temporal store prefetch to L1 cache. */
    TRACE_TYPE_PREFETCH_WRITE_L2,    /**< Store prefetch to L2 cache. */
    TRACE_TYPE_PREFETCH_WRITE_L2_NT, /**< Non-temporal store prefetch to L2 cache. */
    TRACE_TYPE_PREFETCH_WRITE_L3,    /**< Store prefetch to L3 cache. */
    TRACE_TYPE_PREFETCH_WRITE_L3_NT, /**< Non-temporal store prefetch to L3 cache. */

    // Internal value for encoding bytes.
    // Currently this is only used for offline traces with OFFLINE_FILE_TYPE_ENCODINGS.
    // XXX i#5520: Add to online traces, but under an option since extra
    // encoding entries add runtime overhead.
    TRACE_TYPE_ENCODING,

    /**
     * A direct conditional jump instruction which was taken.
     * This is only used in offline non-i-filtered traces.
     */
    TRACE_TYPE_INSTR_TAKEN_JUMP,
    /**
     * A direct conditional jump instruction which was not taken.
     * This is only used in offline non-i-filtered traces.
     */
    TRACE_TYPE_INSTR_UNTAKEN_JUMP,

    /** An invalid record, meant for use as a sentinel value. */
    TRACE_TYPE_INVALID,

    // Update trace_type_names[] when adding here.
} trace_type_t;

/** The sub-type for TRACE_TYPE_MARKER. */
/* For offline traces, we are not able to place a marker accurately in the middle
 * of a block (except for kernel event markers which contain their interruption PC).
 * Markers should thus generally be at block boundaries.
 */
typedef enum {
    /**
     * The subsequent instruction is the start of a handler for a kernel-initiated
     * event: a signal handler or restartable sequence abort handler on UNIX, or an
     * APC, exception, or callback dispatcher on Windows.
     * The value of this marker contains the program counter at the kernel
     * interruption point.  If the interruption point is just after a branch, this
     * value is the target of that branch.
     * (For trace version #TRACE_ENTRY_VERSION_NO_KERNEL_PC or
     * below, the value is the module offset rather than the absolute program counter.)
     * The value is 0 for some types where this information is not available, namely
     * Windows callbacks.
     * A restartable sequence abort handler is further identified by a prior
     * marker of type #TRACE_MARKER_TYPE_RSEQ_ABORT.
     * A signal handler is optionally further identified by a subsequent marker
     * of type #TRACE_MARKER_TYPE_SIGNAL_NUMBER.
     */
    TRACE_MARKER_TYPE_KERNEL_EVENT,
    /**
     * The subsequent instruction is the target of a system call that changes the
     * context: a signal return on UNIX, or a callback return or NtContinue or
     * NtSetContextThread on Windows.
     */
    TRACE_MARKER_TYPE_KERNEL_XFER,
    // XXX i#5634: Add 64-bit marker value support to 32-bit to avoid truncating.
    /**
     * The marker value contains a timestamp for this point in the trace, in units
     * of microseconds since Jan 1, 1601 (the UTC time).  For 32-bit, the value
     * is truncated to 32 bits.
     */
    TRACE_MARKER_TYPE_TIMESTAMP,
    /**
     * The marker value contains the cpu identifier of the cpu this thread was running
     * on at this point in the trace.  A value of (uintptr_t)-1 indicates that the
     * cpu could not be determined.  This value contains what the operating system
     * set up.  For Linux, the bottom 12 bits hold the cpu identifier and the upper
     * bits hold the socket/node number.
     */
    TRACE_MARKER_TYPE_CPU_ID,

    /**
     * The marker value contains the function id for functions traced by the user via
     * the -record_function (and -record_heap_value if -record_heap is specified)
     * option.  The mapping from this numeric ID to a library-qualified symbolic name
     * is recorded during tracing in a file "funclist.log" whose format is described by
     * the drmemtrace_get_funclist_path() function's documentation.
     *
     * This marker is also used to record parameter values for certain system calls such
     * as for #OFFLINE_FILE_TYPE_BLOCKING_SYSCALLS or -record_syscall.  These use
     * large identifiers equal to
     * #func_trace_t::TRACE_FUNC_ID_SYSCALL_BASE plus the system
     * call number (for 32-bit marker values just the bottom 16 bits of the system call
     * number are added to the base).  These identifiers are not stored in the function
     * list file (drmemtrace_get_funclist_path()).  The system call number used is the
     * value passed to DynamoRIO's dr_register_pre_syscall_event() which is normalized to
     * match SYS_ constants (see the dr_register_pre_syscall_event() documentation
     * regarding MacOS).
     */
    TRACE_MARKER_TYPE_FUNC_ID,

    // XXX i#3048: replace return address with callstack information.
    /**
     * The marker value contains the return address of the just-entered
     * function, whose id is specified by the closest previous
     * #TRACE_MARKER_TYPE_FUNC_ID marker entry.
     */
    TRACE_MARKER_TYPE_FUNC_RETADDR,

    /**
     * The marker value contains one argument value of the just-entered
     * function, whose id is specified by the closest previous
     * #TRACE_MARKER_TYPE_FUNC_ID marker entry. The number of such
     * entries for one function invocation is equal to the specified argument in
     * -record_function (or pre-defined functions in -record_heap_value if
     * -record_heap is specified) or -record_syscall.
     */
    TRACE_MARKER_TYPE_FUNC_ARG,

    /**
     * The marker value contains the return value of the just-entered function,
     * whose id is specified by the closest previous
     * #TRACE_MARKER_TYPE_FUNC_ID marker entry.  This is a
     * pointer-sized value from the conventional return value register.
     *
     * For system calls, this may not be enough to determine whether the call
     * succeeded. See #TRACE_MARKER_TYPE_SYSCALL_FAILED.
     */
    TRACE_MARKER_TYPE_FUNC_RETVAL,

    /* This is a non-public type only present in an offline raw trace. To support a
     * full 64-bit marker value in an offline trace where
     * offline_entry_t.extended.valueA contains <64 bits, we use two consecutive
     * entries.  We rely on these being adjacent in the trace.  This entry must come
     * first, and its valueA is left-shited 32 and then OR-ed with the subsequent
     * entry's valueA to produce the final marker value.
     */
    TRACE_MARKER_TYPE_SPLIT_VALUE,

    /**
     * The marker value contains the OFFLINE_FILE_TYPE_* bitfields of type
     * #offline_file_type_t identifying the architecture and other
     * key high-level attributes of the trace.
     */
    TRACE_MARKER_TYPE_FILETYPE,

    /**
     * The marker value contains the traced processor's cache line size in
     * bytes.
     */
    TRACE_MARKER_TYPE_CACHE_LINE_SIZE,

    /**
     * The marker value contains the count of dynamic instruction executions in
     * this software thread since the start of the trace.  This marker type is only
     * present in online-cache-filtered traces and is placed at thread exit.
     */
    TRACE_MARKER_TYPE_INSTRUCTION_COUNT,

    /**
     * The marker value contains the version of the trace format: a value
     * of type #trace_version_t.  The marker is present in the
     * first few entries of a trace file.
     */
    TRACE_MARKER_TYPE_VERSION,

    /**
     * Serves to further identify #TRACE_MARKER_TYPE_KERNEL_EVENT
     * as a restartable sequence abort handler.  This will always be immediately followed
     * by #TRACE_MARKER_TYPE_KERNEL_EVENT.  The marker value for a
     * signal that interrupted the instrumented execution is the precise interrupted PC,
     * but for all other cases the value holds the continuation program counter, which is
     * the restartable sequence abort handler.  (The precise interrupted point inside the
     * sequence is not provided by the kernel.)
     */
    TRACE_MARKER_TYPE_RSEQ_ABORT,

    /**
     * Identifies in the marker value the ordinal of a window during a multi-window
     * tracing run (see the options -trace_for_instrs and -retrace_every_instrs).
     * When a marker with an ordinal value different from the last-seen marker
     * appears, a time gap may exist immediately before this new marker.
     */
    TRACE_MARKER_TYPE_WINDOW_ID,

    /**
     * The marker value contains the physical address corresponding to the subsequent
     * #TRACE_MARKER_TYPE_VIRTUAL_ADDRESS's virtual address.  A
     * pair of such markers will appear somewhere prior to a regular instruction fetch or
     * data load or store whose page's physical address has not yet been reported, or when
     * a physical mapping change is detected.  If translation failed, a
     * #TRACE_MARKER_TYPE_PHYSICAL_ADDRESS_NOT_AVAILABLE will be
     * present instead, without a corresponding
     * #TRACE_MARKER_TYPE_VIRTUAL_ADDRESS.
     */
    TRACE_MARKER_TYPE_PHYSICAL_ADDRESS,

    /**
     * Indicates a failure to obtain the physical address corresponding to the
     * virtual address contained in the marker value.
     */
    TRACE_MARKER_TYPE_PHYSICAL_ADDRESS_NOT_AVAILABLE,

    /**
     * The marker value contains the virtual address corresponding to the prior
     * #TRACE_MARKER_TYPE_PHYSICAL_ADDRESS's physical address.  A
     * pair of such markers will appear somewhere prior to a regular instruction fetch or
     * data load or store whose page's physical address has not yet been reported, or when
     * a physical mapping change is detected.  If translation failed, a
     * #TRACE_MARKER_TYPE_PHYSICAL_ADDRESS_NOT_AVAILABLE will be
     * present instead, without a corresponding
     * #TRACE_MARKER_TYPE_VIRTUAL_ADDRESS.
     */
    TRACE_MARKER_TYPE_VIRTUAL_ADDRESS,

    /**
     * The marker value contains the traced process's page size in bytes.
     */
    TRACE_MARKER_TYPE_PAGE_SIZE,

    /**
     * This marker is emitted prior to each system call when -enable_kernel_tracing is
     * specified. The marker value contains a unique identifier for the system call within
     * a specific thread.
     * \note This marker serves solely to indicate when to decode the syscall's PT trace
     * and will not be included in the final complete trace. Instead, we utilize
     * #TRACE_MARKER_TYPE_SYSCALL_TRACE_START and #TRACE_MARKER_TYPE_SYSCALL_TRACE_END to
     * signify the beginning and end of a syscall's PT trace in the final trace.
     */
    TRACE_MARKER_TYPE_SYSCALL_IDX,

    /**
     * This top-level marker identifies the instruction count in each chunk
     * of the output file.  This is the granularity of a fast seek.
     */
    TRACE_MARKER_TYPE_CHUNK_INSTR_COUNT,

    /**
     * Marks the end of a chunk.  The final chunk does not have such a marker
     * but instead relies on the #TRACE_TYPE_FOOTER entry.
     */
    TRACE_MARKER_TYPE_CHUNK_FOOTER,

    /**
     * Indicates the record ordinal for this point in the trace.  This is used
     * to identify the visible record ordinal when skipping over chunks, and is
     * not exposed to analysis tools.
     */
    TRACE_MARKER_TYPE_RECORD_ORDINAL,

    /**
     * Indicates the point in the trace where filtering ended. It is accompanied by
     * #dynamorio::drmemtrace::OFFLINE_FILE_TYPE_BIMODAL_FILTERED_WARMUP in the file
     * type. This is added by the record_filter tool and also by the drmemtrace tracer
     * (with -L0_filter_until_instrs) to annotate when the warmup part of the trace
     * ended.
     */
    TRACE_MARKER_TYPE_FILTER_ENDPOINT,

    // We use one marker at the start whose data is the end, instead of a separate
    // marker at the end PC, as this seems easier for users to process as they can plan
    // ahead. Also, when the rseq aborted, if we had the marker at the committing store
    // the user would then not know where it was supposed to be as it would not be
    // present.
    /**
     * Indicates the start of an "rseq" (Linux restartable sequence) region.  The marker
     * value holds the end PC of the region (this is the PC after the committing store).
     */
    TRACE_MARKER_TYPE_RSEQ_ENTRY,

    /**
     * This marker is emitted prior to each system call invocation, after the
     * instruction fetch entry for the system call gateway instruction from user mode. The
     * marker value contains the system call number.  If these markers are present, the
     * file type #OFFLINE_FILE_TYPE_SYSCALL_NUMBERS is set.
     */
    TRACE_MARKER_TYPE_SYSCALL,

    /**
     * This marker is emitted prior to a system call which is known to block under some
     * circumstances.  Whether it blocks may depend on the values of parameters or the
     * state of options set in prior system calls.  Additionally, it is not guaranteed
     * that every system call that might block is identified in the initial
     * implementation, and it may be limited only to certain operating systems (Linux
     * only for now).
     *
     * If these markers are present, the
     * file type #OFFLINE_FILE_TYPE_BLOCKING_SYSCALLS is set.  The
     * marker value is 0.
     */
    TRACE_MARKER_TYPE_MAYBE_BLOCKING_SYSCALL,

    /**
     * Indicates a point in the trace where a syscall's kernel trace starts. The value
     * of the marker is set to the syscall number.
     */
    TRACE_MARKER_TYPE_SYSCALL_TRACE_START,

    /**
     * Indicates a point in the trace where a syscall's trace ends. The value of the
     * marker is set to the syscall number.
     */
    TRACE_MARKER_TYPE_SYSCALL_TRACE_END,

    /**
     * Internal marker present just before each indirect branch instruction in offline
     * non-i-filtered traces.  The marker value holds the actual target of the
     * branch.  The reader converts this to the instr.indirect_branch_target field in
     * #memref_t and does not pass it on, so #memref_t analysis tools never see this
     * marker.
     */
    TRACE_MARKER_TYPE_BRANCH_TARGET,

    // Although it is only for Mac that syscall success requires more than the
    // main return value register, we include the failure marker for all platforms
    // as mmap is complex and it is simpler to not have Mac-only code paths.
    /**
     * This marker is emitted for system calls whose parameters are traced with
     * -record_syscall.  It is emitted immediately after #TRACE_MARKER_TYPE_FUNC_RETVAL
     * if the prior system call (whose id is specified by the closest previous
     * #TRACE_MARKER_TYPE_FUNC_ID marker entry) failed.  Whether it failed is obtained
     * from dr_syscall_get_result_ex() via the "succeeded" field of
     * #dr_syscall_result_info_t.  See the corresponding documentation for caveats about
     * the accuracy of this determination.  The marker value is the "errno_value" field
     * of #dr_syscall_result_info_t.
     */
    TRACE_MARKER_TYPE_SYSCALL_FAILED,

    /**
     * This marker is emitted prior to a system call (but after the system call's
     * #TRACE_MARKER_TYPE_SYSCALL and #TRACE_MARKER_TYPE_MAYBE_BLOCKING_SYSCALL markers)
     * that causes an immediate switch to another thread on the same core (with the
     * current thread entering an unscheduled state), bypassing the kernel scheduler's
     * normal dynamic switch code based on run queues.  The marker value holds the thread
     * id of the target thread.  The current thread will remain unschedulable
     * indefinitely unless another thread resumes it with either
     * #TRACE_MARKER_TYPE_DIRECT_THREAD_SWITCH or #TRACE_MARKER_TYPE_SYSCALL_SCHEDULE;
     * or, if a #TRACE_MARKER_TYPE_SYSCALL_ARG_TIMEOUT marker is present, the thread will
     * become schedulable when that timeout expires.  This marker provides a mechanism to
     * model these semantics while abstracting away whether the underlying system call is
     * a custom kernel extension or a variant of "futex" or other selective wait-notify
     * scheme.  This marker should generally always be after a
     * #TRACE_MARKER_TYPE_MAYBE_BLOCKING_SYSCALL marker as such a switch always has a
     * chance of blocking the source thread.  See also
     * #TRACE_MARKER_TYPE_SYSCALL_ARG_TIMEOUT, #TRACE_MARKER_TYPE_SYSCALL_UNSCHEDULE, and
     * #TRACE_MARKER_TYPE_SYSCALL_SCHEDULE.  The scheduler only models this behavior when
     * #dynamorio::drmemtrace::scheduler_tmpl_t::scheduler_options_t.honor_direct_switches
     * is true.
     */
    TRACE_MARKER_TYPE_DIRECT_THREAD_SWITCH,

    /**
     * This marker is used for core-sharded analyses to indicate that the current
     * core is waiting on another core.  This is primarily only useful for analyses
     * studying the scheduling of threads onto cores.  A new marker is emitted each
     * time the tool analysis framework requests a new record from the scheduler and
     * is given a wait status.  There are no units of time here but each repetition
     * is roughly the time where a regular record could have been read and passed
     * along.
     */
    TRACE_MARKER_TYPE_CORE_WAIT,

    /**
     * This marker is used for core-sharded analyses to indicate that the current
     * core has no available inputs to run (all inputs are on other cores or are
     * blocked waiting for kernel resources).  A new marker is emitted each
     * time the tool analysis framework requests a new record from the scheduler and
     * is given an idle status.  There are no units of time here but each repetition
     * is roughly the time where a regular record could have been read and passed
     * along.  This idle marker indicates that a core actually had no work to do,
     * as opposed to #TRACE_MARKER_TYPE_CORE_WAIT which is an artifact of an
     * imposed re-created schedule.
     */
    TRACE_MARKER_TYPE_CORE_IDLE,

    /**
     * Indicates a point in the trace where context switch's kernel trace starts.
     * The value of the marker is set to the switch type enum value from
     * #dynamorio::drmemtrace::scheduler_tmpl_t::switch_type_t.
     */
    TRACE_MARKER_TYPE_CONTEXT_SWITCH_START,

    /**
     * Indicates a point in the trace where a context switch's kernel trace ends.
     * The value of the marker is set to the switch type enum value from
     * #dynamorio::drmemtrace::scheduler_tmpl_t::switch_type_t.
     */
    TRACE_MARKER_TYPE_CONTEXT_SWITCH_END,

    /**
     * This marker's value is the current thread's vector length in bytes, for
     * architectures with a dynamic vector length. It is currently only used on AArch64.
     *
     * On AArch64 the marker's value contains the SVE vector length. The marker is
     * emitted with the thread header to establish the initial vector length for that
     * thread. In the future it will also be emitted later in the trace if the app
     * changes the vector length at runtime (TODO i#6625). In all cases the vector
     * length value is specific to the current thread.
     * The vector length affects how some SVE instructions are decoded so any tools which
     * decode instructions should clear any cached data and set the vector length used by
     * the decoder using dr_set_vector_length().
     */
    TRACE_MARKER_TYPE_VECTOR_LENGTH,

    /**
     * This marker is emitted prior to a system call (but after the system call's
     * #TRACE_MARKER_TYPE_SYSCALL and #TRACE_MARKER_TYPE_MAYBE_BLOCKING_SYSCALL markers)
     * that causes the current thread to become unschedulable (removed from all queues of
     * runnable threads).  The thread will remain unschedulable indefinitely unless
     * another thread resumes it with either #TRACE_MARKER_TYPE_DIRECT_THREAD_SWITCH or
     * #TRACE_MARKER_TYPE_SYSCALL_SCHEDULE; or, if a
     * #TRACE_MARKER_TYPE_SYSCALL_ARG_TIMEOUT marker is present, the thread will become
     * schedulable when that timeout expires.  This marker provides a mechanism to model
     * these semantics while abstracting away whether the underlying system call is a
     * custom kernel extension or a variant of "futex" or other selective wait-notify
     * scheme.  This marker should generally always be after a
     * #TRACE_MARKER_TYPE_MAYBE_BLOCKING_SYSCALL marker as becoming unschedulable is a
     * form of blocking and results in a context switch.  The scheduler only models this
     * behavior when
     * #dynamorio::drmemtrace::scheduler_tmpl_t::scheduler_options_t.honor_direct_switches
     * is true.
     */
    TRACE_MARKER_TYPE_SYSCALL_UNSCHEDULE,

    /**
     * This marker is emitted prior to a system call (but after the system call's
     * #TRACE_MARKER_TYPE_SYSCALL marker) that causes a target thread identified in the
     * marker value to become schedulable again if it were currently unschedulable or if
     * it is not currently unschedulable to *not* become unschedulable on its next action
     * that would otherwise do so.  See also #TRACE_MARKER_TYPE_SYSCALL_UNSCHEDULE and
     * #TRACE_MARKER_TYPE_DIRECT_THREAD_SWITCH.  This marker provides a mechanism to
     * model these semantics while abstracting away whether the underlying system call is
     * a custom kernel extension or a variant of "futex" or other selective wait-notify
     * scheme.  The scheduler only models this behavior when
     * #dynamorio::drmemtrace::scheduler_tmpl_t::scheduler_options_t.honor_direct_switches
     * is true.
     */
    TRACE_MARKER_TYPE_SYSCALL_SCHEDULE,

    /**
     * This marker is emitted prior to a system call (but after the system call's
     * #TRACE_MARKER_TYPE_SYSCALL and #TRACE_MARKER_TYPE_MAYBE_BLOCKING_SYSCALL markers)
     * which also has a #TRACE_MARKER_TYPE_DIRECT_THREAD_SWITCH or
     * #TRACE_MARKER_TYPE_SYSCALL_UNSCHEDULE marker.  This indicates a timeout provided
     * by the application after which the thread will become schedulable again.  The
     * marker value holds the timeout duration in microseconds.
     */
    TRACE_MARKER_TYPE_SYSCALL_ARG_TIMEOUT,

    /**
     * This marker is emitted prior to the invocation of a signal handler,
     * after the #TRACE_MARKER_TYPE_KERNEL_EVENT record for the handler.
     * The marker value holds the signal number.
     */
    TRACE_MARKER_TYPE_SIGNAL_NUMBER,

    /**
     * This marker is used to indicate an instruction started to execute but
     * didn't retire. The instruction was either preempted by an asynchronous
     * signal or caused a fault. The instruction and corresponding memrefs
     * are removed from the trace.
     *
     * The marker value is the raw encoding bytes of the instruction up to the
     * length of a pointer. The encoding will be incomplete for instructions
     * with long encodings. It is best-effort to help understand the sequence of
     * generated code where encodings are not available offline. The PC of this
     * instruction is available in a subsequent
     * #dynamorio::drmemtrace::TRACE_MARKER_TYPE_KERNEL_EVENT marker.
     */
    TRACE_MARKER_TYPE_UNCOMPLETED_INSTRUCTION,

    // ...
    // These values are reserved for future built-in marker types.
    // ...
    TRACE_MARKER_TYPE_RESERVED_END = 100,
    // Values below here are available for users to use for custom markers.
} trace_marker_type_t;

// As documented in TRACE_MARKER_TYPE_CPU_ID, this value indicates an unknown CPU.
#define INVALID_CPU_MARKER_VALUE static_cast<uintptr_t>(-1)

/** Constants related to function or system call parameter tracing. */
enum class func_trace_t : uint64_t { // VS2019 won't infer 64-bit with "enum {".
/**
 * When system call parameter and return values are provided, they use the function
 * tracing markers #TRACE_MARKER_TYPE_FUNC_ID, #TRACE_MARKER_TYPE_FUNC_ARG, and
 * #TRACE_MARKER_TYPE_FUNC_RETVAL.  The identifier used for #TRACE_MARKER_TYPE_FUNC_ID is
 * equal to this base value plus the 32-bit system call number for 64-bit marker values
 * or this base value plus the lower 16 bits of the system call number for 32-bit marker
 * values.
 */
#ifdef X64
    TRACE_FUNC_ID_SYSCALL_BASE = 0x100000000ULL,
#else
    TRACE_FUNC_ID_SYSCALL_BASE = 0x10000U,
#endif
};

extern const char *const trace_type_names[];

/**
 * Returns whether the type represents an instruction fetch.
 * Deliberately excludes TRACE_TYPE_INSTR_NO_FETCH and TRACE_TYPE_INSTR_BUNDLE.
 */
static inline bool
type_is_instr(const trace_type_t type)
{
    return (type >= TRACE_TYPE_INSTR && type <= TRACE_TYPE_INSTR_RETURN) ||
        type == TRACE_TYPE_INSTR_SYSENTER || type == TRACE_TYPE_INSTR_TAKEN_JUMP ||
        type == TRACE_TYPE_INSTR_UNTAKEN_JUMP;
}

/**
 * Returns whether \p type represents any type of instruction record whether an
 * instruction fetch or operation hint. This is a superset of type_is_instr() and includes
 * #TRACE_TYPE_INSTR_NO_FETCH.
 */
static inline bool
is_any_instr_type(const trace_type_t type)
{
    return type_is_instr(type) || type == TRACE_TYPE_INSTR_MAYBE_FETCH ||
        type == TRACE_TYPE_INSTR_NO_FETCH;
}

/** Returns whether the type represents the fetch of a branch instruction. */
static inline bool
type_is_instr_branch(const trace_type_t type)
{
    return (type >= TRACE_TYPE_INSTR_DIRECT_JUMP && type <= TRACE_TYPE_INSTR_RETURN) ||
        type == TRACE_TYPE_INSTR_TAKEN_JUMP || type == TRACE_TYPE_INSTR_UNTAKEN_JUMP;
}

/** Returns whether the type represents the fetch of a direct branch instruction. */
static inline bool
type_is_instr_direct_branch(const trace_type_t type)
{
    return type == TRACE_TYPE_INSTR_DIRECT_JUMP ||
        type == TRACE_TYPE_INSTR_CONDITIONAL_JUMP ||
        type == TRACE_TYPE_INSTR_DIRECT_CALL || type == TRACE_TYPE_INSTR_TAKEN_JUMP ||
        type == TRACE_TYPE_INSTR_UNTAKEN_JUMP;
}

/** Returns whether the type represents the fetch of a conditional branch instruction. */
static inline bool
type_is_instr_conditional_branch(const trace_type_t type)
{
    return type == TRACE_TYPE_INSTR_CONDITIONAL_JUMP ||
        type == TRACE_TYPE_INSTR_TAKEN_JUMP || type == TRACE_TYPE_INSTR_UNTAKEN_JUMP;
}

/** Returns whether the type represents a prefetch request. */
static inline bool
type_is_prefetch(const trace_type_t type)
{
    return (type >= TRACE_TYPE_PREFETCH && type <= TRACE_TYPE_PREFETCH_INSTR) ||
        (type >= TRACE_TYPE_PREFETCH_READ_L1_NT &&
         type <= TRACE_TYPE_PREFETCH_WRITE_L3_NT) ||
        type == TRACE_TYPE_HARDWARE_PREFETCH;
}

/**
 * Returns whether the type contains an address.  This includes both instruction
 * fetches and instruction operands.
 */
static inline bool
type_has_address(const trace_type_t type)
{
    return type_is_instr(type) || type == TRACE_TYPE_INSTR_NO_FETCH ||
        type == TRACE_TYPE_INSTR_MAYBE_FETCH || type_is_prefetch(type) ||
        type == TRACE_TYPE_READ || type == TRACE_TYPE_WRITE ||
        type == TRACE_TYPE_INSTR_FLUSH || type == TRACE_TYPE_INSTR_FLUSH_END ||
        type == TRACE_TYPE_DATA_FLUSH || type == TRACE_TYPE_DATA_FLUSH_END;
}

/**
 * Returns whether the type represents an address operand of an instruction.
 * This is a subset of type_has_address() as type_has_address() includes
 * instruction fetches.
 */
static inline bool
type_is_data(const trace_type_t type)
{
    return type_is_prefetch(type) || type == TRACE_TYPE_READ ||
        type == TRACE_TYPE_WRITE || type == TRACE_TYPE_INSTR_FLUSH ||
        type == TRACE_TYPE_INSTR_FLUSH_END || type == TRACE_TYPE_DATA_FLUSH ||
        type == TRACE_TYPE_DATA_FLUSH_END;
}

/**
 * Returns whether the type represents a memory read access.
 */
static inline bool
type_is_read(const trace_type_t type)
{
    return type_is_prefetch(type) || type == TRACE_TYPE_READ ||
        type == TRACE_TYPE_INSTR_FLUSH || type == TRACE_TYPE_INSTR_FLUSH_END ||
        type == TRACE_TYPE_DATA_FLUSH || type == TRACE_TYPE_DATA_FLUSH_END;
}

static inline bool
marker_type_is_function_marker(const trace_marker_type_t mark)
{
    return mark >= TRACE_MARKER_TYPE_FUNC_ID && mark <= TRACE_MARKER_TYPE_FUNC_RETVAL;
}

// The longest instruction on any architecture.
// This matches DR's MAX_INSTR_LENGTH for x86 but we want the same
// size for all architectures and DR's define is available ifdef X86 only.
#define MAX_ENCODING_LENGTH 17

/**
 * This is the data format generated by the online tracer and produced after
 * post-processing of raw offline traces.
 * The #dynamorio::drmemtrace::reader_t class transforms this into
 * #memref_t before handing to analysis tools. Each trace entry is
 * a <type, size, addr> tuple representing:
 * - a memory reference
 * - an instr fetch
 * - a bundle of instrs
 * - a flush request
 * - a prefetch request
 * - a thread/process.
 * All fields are stored as little-endian.  The raw records from the tracer may
 * be big-endian (per the architecture trace type field), in which case raw2trace must
 * convert them to little-endian.  The #memref_t fields may be presented as big-endian
 * to simplify analyzers running on big-endian machines, in which case the conversion
 * from the trace format #trace_entry_t to big-endian is performed by the
 * #dynamorio::drmemtrace::reader_t class.
 */
START_PACKED_STRUCTURE
struct _trace_entry_t {
    unsigned short type; // 2 bytes: trace_type_t
    // 2 bytes: mem ref size, instr length, or num of instrs for instr bundle,
    // or marker sub-type, or num of bytes (max sizeof(addr_t)) in encoding[] array.
    unsigned short size;
    union {
        addr_t addr; // 4/8 bytes: mem ref addr, instr pc, tid, pid, marker val
        // The length of each instr in the instr bundle
        unsigned char length[sizeof(addr_t)];
        // The raw encoding bytes for the subsequent instruction fetch entry.
        // There may be multiple consecutive records to hold long instructions.
        // The reader should keep concatenating these bytes until the subsequent
        // instruction fetch entry is found.
        unsigned char encoding[sizeof(addr_t)];
    };
} END_PACKED_STRUCTURE;

/** See #dynamorio::drmemtrace::_trace_entry_t. */
typedef struct _trace_entry_t trace_entry_t;

///////////////////////////////////////////////////////////////////////////
//
// Offline trace format

// For offline traces, the tracing overhead is no longer overshadowed by online
// simulation.  Consequently, we aggressively shrink the tracer's trace entries,
// reconstructing the trace_entry_t format that the readers expect via a
// post-processing step before feeding it to analysis tools.

// We target 64-bit addresses and do not bother to shrink the module or timestamp
// entries for 32-bit apps.
// We assume that a 64-bit address has far fewer real bits, typically
// 48 bits, and that the top bits 48..63 are always identical.  Thus we can store
// a type field in those top bits.
// For the most common, a memref, we have both all 0's and all 1's be its
// type to reduce instrumentation overhead.
// The type simply identifies which union alternative:
typedef enum {
    OFFLINE_TYPE_MEMREF, // We rely on this being 0.
    OFFLINE_TYPE_PC,
    OFFLINE_TYPE_THREAD,
    OFFLINE_TYPE_PID,
    OFFLINE_TYPE_TIMESTAMP,
    // An ARM SYS_cacheflush: always has two addr entries for [start, end).
    OFFLINE_TYPE_IFLUSH,
    // The ext field identifies this further.
    OFFLINE_TYPE_EXTENDED,
    OFFLINE_TYPE_MEMREF_HIGH = 7,
} offline_type_t;

// Sub-type when the primary type is OFFLINE_TYPE_EXTENDED.
// These differ in what they store in offline_entry_t.extended.value.
typedef enum {
    // The initial entry in trace files with version older than
    // OFFLINE_FILE_VERSION_HEADER_FIELDS_SWAP.  The valueA field holds the
    // version (OFFLINE_FILE_VERSION*) while valueB holds the type
    // (OFFLINE_FILE_TYPE*).
    OFFLINE_EXT_TYPE_HEADER_DEPRECATED,
    // The final entry in the file.  The value fields are 0.
    OFFLINE_EXT_TYPE_FOOTER,
    // A marker type.  The valueB field holds the sub-type and valueA the value.
    OFFLINE_EXT_TYPE_MARKER,
    // Stores the type of access in valueB and the size in valueA.
    // Used for filters on multi-memref instrs where post-processing can't tell
    // which memref passed the filter.
    OFFLINE_EXT_TYPE_MEMINFO,
    // The initial entry in trace files (this is the expected header in current
    // traces, as opposed to OFFLINE_EXT_TYPE_HEADER_DEPRECATED).  The valueA
    // field holds the type (OFFLINE_FILE_TYPE*), while valueB holds the
    // version (OFFLINE_FILE_VERSION*).
    OFFLINE_EXT_TYPE_HEADER,
} offline_ext_type_t;

#define EXT_VALUE_A_BITS 48
#define EXT_VALUE_B_BITS 8

#define PC_MODOFFS_BITS 33
#define PC_MODIDX_BITS 16
// We reserve the top value to indicate non-module generated code.
// TODO i#2062: Filtered traces use a different scheme for modoffs (see
// ENCODING_FILE_TYPE_SEPARATE_NON_MOD_INSTRS) where the total non-module
// code is limited to 8GB (33 bytes worth of addressing). We can potentially
// allow more gencode by using multiple modidx (and not just
// PC_MODIDX_INVALID) for pointing to non-module code, growing downward from
// PC_MODIDX_INVALID.
#define PC_MODIDX_INVALID ((1 << PC_MODIDX_BITS) - 1)
#define PC_INSTR_COUNT_BITS 12
#define PC_TYPE_BITS 3

#define OFFLINE_FILE_VERSION_NO_ELISION 2
#define OFFLINE_FILE_VERSION_OLDEST_SUPPORTED OFFLINE_FILE_VERSION_NO_ELISION
#define OFFLINE_FILE_VERSION_ELIDE_UNMOD_BASE 3
#define OFFLINE_FILE_VERSION_KERNEL_INT_PC 4
#define OFFLINE_FILE_VERSION_HEADER_FIELDS_SWAP 5
#define OFFLINE_FILE_VERSION_ENCODINGS 6
#define OFFLINE_FILE_VERSION_XFER_ABS_PC \
    7 /**< Use the absolute PC for kernel interruption PC for 64-bit mode.*/
#define OFFLINE_FILE_VERSION_NO_OP 8 /**< There are no changes in this version.*/
#define OFFLINE_FILE_VERSION OFFLINE_FILE_VERSION_NO_OP

/**
 * Bitfields used to describe the high-level characteristics of both an
 * offline final trace and a raw not-yet-postprocessed trace, as well as
 * (despite the OFFLINE_ prefix) an online trace.
 * In a final trace these are stored in a marker of type
 * #TRACE_MARKER_TYPE_FILETYPE.
 */
typedef enum {
    OFFLINE_FILE_TYPE_DEFAULT = 0x00,
    /**
     * DEPRECATED: Addresses filtered online. Newer trace files use
     * #OFFLINE_FILE_TYPE_IFILTERED and #OFFLINE_FILE_TYPE_DFILTERED.
     */
    OFFLINE_FILE_TYPE_FILTERED = 0x01,
    OFFLINE_FILE_TYPE_NO_OPTIMIZATIONS = 0x02,
    OFFLINE_FILE_TYPE_INSTRUCTION_ONLY = 0x04, /**< Trace has no data references. */
    OFFLINE_FILE_TYPE_ARCH_AARCH64 = 0x08,     /**< Recorded on AArch64. */
    OFFLINE_FILE_TYPE_ARCH_ARM32 = 0x10,       /**< Recorded on ARM (32-bit). */
    OFFLINE_FILE_TYPE_ARCH_X86_32 = 0x20,      /**< Recorded on x86 (32-bit). */
    OFFLINE_FILE_TYPE_ARCH_X86_64 = 0x40,      /**< Recorded on x86 (64-bit). */
    /**
     * Instruction addresses filtered online.
     * Note: this file type may transition to non-filtered. If so, the transition is
     * indicated by the #dynamorio::drmemtrace::TRACE_MARKER_TYPE_FILTER_ENDPOINT marker
     * and the #OFFLINE_FILE_TYPE_BIMODAL_FILTERED_WARMUP file type. Each window (which is
     * indicated by the #dynamorio::drmemtrace::TRACE_MARKER_TYPE_WINDOW_ID marker) starts
     * out filtered. This applies to #dynamorio::drmemtrace::OFFLINE_FILE_TYPE_DFILTERED
     * also. Note that threads that were created after the transition will also have this
     * marker - right at the beginning.
     */
    OFFLINE_FILE_TYPE_IFILTERED = 0x80,
    OFFLINE_FILE_TYPE_DFILTERED = 0x100, /**< Data addresses filtered online. */
    OFFLINE_FILE_TYPE_ENCODINGS = 0x200, /**< Instruction encodings are included. */
    /** System call number markers (#TRACE_MARKER_TYPE_SYSCALL) are
       included. */
    OFFLINE_FILE_TYPE_SYSCALL_NUMBERS = 0x400,
    /**
     * Kernel scheduling information is included:
     * #TRACE_MARKER_TYPE_MAYBE_BLOCKING_SYSCALL markers and system call parameters and
     * return values for kernel locks (SYS_futex on Linux) using the function tracing
     * markers #TRACE_MARKER_TYPE_FUNC_ID, #TRACE_MARKER_TYPE_FUNC_ARG, and
     * #TRACE_MARKER_TYPE_FUNC_RETVAL with an identifier equal to
     * #func_trace_t::TRACE_FUNC_ID_SYSCALL_BASE plus the system call number (or its
     * bottom 16 bits for 32-bit marker values).  These identifiers are not stored in the
     * function list file (drmemtrace_get_funclist_path()).
     *
     * The #TRACE_MARKER_TYPE_FUNC_RETVAL for system calls is
     * either 0 (failure) or 1 (success).
     */
    OFFLINE_FILE_TYPE_BLOCKING_SYSCALLS = 0x800,
    /**
     * Kernel traces (both instructions and memory addresses) of syscalls are included. If
     * only kernel instructions are included the file type is
     * #OFFLINE_FILE_TYPE_KERNEL_SYSCALL_INSTR_ONLY instead. The included kernel traces
     * are provided by the -syscall_template_file to raw2trace (see
     * #OFFLINE_FILE_TYPE_KERNEL_SYSCALL_TRACE_TEMPLATES).
     */
    OFFLINE_FILE_TYPE_KERNEL_SYSCALLS = 0x1000,
    /**
     * Partially filtered trace. The initial part up to the
     * #TRACE_MARKER_TYPE_FILTER_ENDPOINT marker is filtered, and the later part is not.
     * Look for other filtering-related file types (#OFFLINE_FILE_TYPE_IFILTERED and
     * #OFFLINE_FILE_TYPE_DFILTERED) to determine how the initial part was filtered.
     * The initial part can be used by a simulator for warmup.
     */
    OFFLINE_FILE_TYPE_BIMODAL_FILTERED_WARMUP = 0x2000,
    /**
     * Indicates an offline trace that contains trace templates for some system calls.
     * The individual traces are enclosed within a pair of
     * #TRACE_MARKER_TYPE_SYSCALL_TRACE_START and #TRACE_MARKER_TYPE_SYSCALL_TRACE_END
     * markers which also specify what system call the contained trace belongs to. This
     * file can be used to create an #OFFLINE_FILE_TYPE_KERNEL_SYSCALLS trace with
     * -syscall_template_file to raw2trace, with -sched_syscall_file to the
     * drmemtrace analyzer framework, and also with #dynamorio::drmemtrace::
     * scheduler_tmpl_t::scheduler_options_t.kernel_syscall_trace_path and #dynamorio::
     * drmemtrace::scheduler_tmpl_t::scheduler_options_t.kernel_syscall_reader to the
     * scheduler. Each system call trace template uses the regular drmemtrace format,
     * including using paired #TRACE_MARKER_TYPE_KERNEL_EVENT and
     * #TRACE_MARKER_TYPE_KERNEL_XFER markers to represent kernel interrupts during
     * system call execution. Each system call trace should end with an indirect
     * branch instruction (e.g., iret/sysret/sysexit on x86, or eret on AArch64) which
     * must be preceded by a #TRACE_MARKER_TYPE_BRANCH_TARGET marker with any value;
     * the marker's value will be appropriately set to point to the fallthrough pc of
     * the prior syscall instruction when the trace template is injected. Note: the
     * marker value will not be the actual next pc in the trace in some cases (i#7496):
     * - if a #TRACE_MARKER_TYPE_KERNEL_EVENT immediately follows the syscall trace,
     *   it indicates interruption of the syscall by a signal; in this case, the next
     *   pc after the signal is the #TRACE_MARKER_TYPE_KERNEL_EVENT marker value,
     *   which for auto-restart syscalls would be the same as the syscall instr pc.
     * - for the sigreturn syscall, the next pc in the trace is what was specified
     *   in the prior #TRACE_MARKER_TYPE_KERNEL_EVENT marker.
     * See the sample file written by the burst_syscall_inject.cpp test for more
     * details on the expected format for the system call template file.
     *
     * TODO i#6495: Add support for reading a zipfile where each trace template is in
     * a separate component. This will make it easier to manually append, update, or
     * inspect the individual templates, and also allow streaming the component with the
     * required template when needed instead of reading the complete file into memory
     * ahead of time. Note that we may drop support for non-zipfile template files in
     * the future.
     */
    OFFLINE_FILE_TYPE_KERNEL_SYSCALL_TRACE_TEMPLATES = 0x4000,
    /**
     * Kernel instruction traces of syscalls are included. When memory addresses are
     * also included for kernel execution, the file type is
     * #OFFLINE_FILE_TYPE_KERNEL_SYSCALLS instead.
     * On x86, the kernel trace is enabled by the -enable_kernel_tracing option that
     * uses Intel® Processor Trace to collect an instruction trace for system call
     * execution.
     */
    OFFLINE_FILE_TYPE_KERNEL_SYSCALL_INSTR_ONLY = 0x8000,
    /**
     * Each trace shard represents one core and contains interleaved software threads.
     * Such a trace is already scheduled, so it is run through the scheduler in a
     * non-scheduled mode where interfaces such as get_workload_id() will not return the
     * original separate inputs but rather the new inputs as seen by the scheduler which
     * are a single workload with one input per core.  Use the modified #memref_t tid and
     * pid fields with the helpers workload_from_memref_pid() and
     * workload_from_memref_tid() to obtain the workload in this case.
     */
    OFFLINE_FILE_TYPE_CORE_SHARDED = 0x10000,
    /**
     * Trace filtered by the record_filter tool using -filter_encodings2regdeps.
     * The encodings2regdeps filter replaces real ISA encodings with #DR_ISA_REGDEPS
     * encodings. Note that these encoding changes do not update the instruction length,
     * hence encoding size and instruction fetch size may not match.
     */
    OFFLINE_FILE_TYPE_ARCH_REGDEPS = 0x20000,
    /**
     * All possible architecture types, including synthetic ones.
     */
    OFFLINE_FILE_TYPE_ARCH_ALL = OFFLINE_FILE_TYPE_ARCH_AARCH64 |
        OFFLINE_FILE_TYPE_ARCH_ARM32 | OFFLINE_FILE_TYPE_ARCH_X86_32 |
        OFFLINE_FILE_TYPE_ARCH_X86_64 | OFFLINE_FILE_TYPE_ARCH_REGDEPS,
} offline_file_type_t;

static inline const char *
trace_arch_string(offline_file_type_t type)
{
    if (TESTANY(OFFLINE_FILE_TYPE_ARCH_AARCH64, type))
        return "aarch64";
    else if (TESTANY(OFFLINE_FILE_TYPE_ARCH_ARM32, type))
        return "arm";
    else if (TESTANY(OFFLINE_FILE_TYPE_ARCH_X86_32, type))
        return "i386";
    else if (TESTANY(OFFLINE_FILE_TYPE_ARCH_X86_64, type))
        return "x86_64";
    else if (TESTANY(OFFLINE_FILE_TYPE_ARCH_REGDEPS, type))
        return "regdeps";
    else
        return "unspecified";
}

/* We have non-client targets including this header that do not include API
 * headers defining IF_X86_ELSE, etc.  Those don't need this function so we
 * simply exclude them.
 *
 * TODO i#7236: If trace_entry.h is included before IF_X86_ELSE is defined by
 * dr_defines.h, it shows up as a build failure without an obvious cause because
 * the order between the two headers is not always immediately clear (since they
 * may be transitively included). i#7236 notes a workaround, but this should be
 * cleaned up.
 */
#ifdef IF_X86_ELSE
static inline offline_file_type_t
build_target_arch_type()
{
    return IF_X86_ELSE(
        IF_X64_ELSE(OFFLINE_FILE_TYPE_ARCH_X86_64, OFFLINE_FILE_TYPE_ARCH_X86_32),
        IF_X64_ELSE(OFFLINE_FILE_TYPE_ARCH_AARCH64, OFFLINE_FILE_TYPE_ARCH_ARM32));
}
#endif

// This structure may be big- or little-endian, but when converted to trace_entry_t
// it must be converted to litte-endian.
START_PACKED_STRUCTURE
struct _offline_entry_t {
    union {
        // Unfortunately the compiler won't combine bitfields across the union border
        // so we have to duplicate the type field in each alternative.
        struct {
            uint64_t addr : 61;
            uint64_t type : 3;
        } addr;
        struct {
            // This describes the entire basic block.
            uint64_t modoffs : PC_MODOFFS_BITS;
            uint64_t modidx : PC_MODIDX_BITS;
            uint64_t instr_count : PC_INSTR_COUNT_BITS;
            uint64_t type : PC_TYPE_BITS;
        } pc;
        struct {
            uint64_t tid : 61;
            uint64_t type : 3;
        } tid;
        struct {
            uint64_t pid : 61;
            uint64_t type : 3;
        } pid;
        struct {
            uint64_t usec : 61; // Microseconds since Jan 1, 1601.
            uint64_t type : 3;
        } timestamp;
        struct {
            uint64_t valueA : EXT_VALUE_A_BITS; // Meaning is specific to ext type.
            uint64_t valueB : EXT_VALUE_B_BITS; // Meaning is specific to ext type.
            uint64_t ext : 5;                   // Holds an offline_ext_type_t value.
            uint64_t type : 3;
        } extended;
        uint64_t combined_value;
    };
} END_PACKED_STRUCTURE;
typedef struct _offline_entry_t offline_entry_t;

// This is the raw marker value for TRACE_MARKER_TYPE_KERNEL_*
// for legacy raw traces prior to OFFLINE_FILE_VERSION_XFER_ABS_PC.
// It occupies 49 bits and so may require two raw entries.
typedef union {
    struct {
        uint64_t modoffs : PC_MODOFFS_BITS;
        uint64_t modidx : PC_MODIDX_BITS;
    } pc;
    uint64_t combined_value;
} kernel_interrupted_raw_pc_t;

// The encoding file begins with a 64-bit integer holding a version number,
// followed by a series of records of type encoding_entry_t.
#define ENCODING_FILE_INITIAL_VERSION 0
// Encoding files have a file type as the second uint64_t in their header.
#define ENCODING_FILE_VERSION_HAS_FILE_TYPE 1
#define ENCODING_FILE_VERSION ENCODING_FILE_VERSION_HAS_FILE_TYPE

/**
 * Bitfields used to describe the type of the encoding file. This is stored as the
 * second uint64_t after the encoding file version.
 */
typedef enum {
    /**
     * Default encoding file type.
     */
    ENCODING_FILE_TYPE_DEFAULT = 0x0,
    /**
     * This encoding file type tells the module_mapper_t that the non-module PC
     * entries in the trace correspond to an individual instr. The modoffs field is
     * interpreted as the cumulative encoding length of all instrs written to the
     * encoding file before the recorded instr. Note that the encoding file itself
     * is still written one mon-module block at a time because it is too inefficient
     * to write one encoding_entry_t for just one non-module instr.
     *
     * If this file type is not set, then the PC entries' modoffs fields are
     * interpreted as the non-mod block's idx.
     */
    ENCODING_FILE_TYPE_SEPARATE_NON_MOD_INSTRS = 0x1,
} encoding_file_type_t;

// All fields are little-endian.
START_PACKED_STRUCTURE
struct _encoding_entry_t {
    size_t length; // Size of the entire structure.
    uint64_t id;   // Incremented for each new non-module fragment DR creates.
    uint64_t start_pc;
#ifdef WINDOWS
#    pragma warning(push)
#    pragma warning(disable : 4200) // Zero-sized array warning.
#endif
    // Variable-length encodings for entire block, of length 'length'.
    unsigned char encodings[0];
#ifdef WINDOWS
#    pragma warning(pop)
#endif
} END_PACKED_STRUCTURE;
typedef struct _encoding_entry_t encoding_entry_t;

// A thread schedule file is a series of these records.
// There is no version number here: we increase the version number in
// the trace files when we change the format of this file.
// All fields are little-endian.
START_PACKED_STRUCTURE
struct schedule_entry_t {
    schedule_entry_t(uint64_t thread, uint64_t timestamp, uint64_t cpu,
                     uint64_t start_instruction)
        : thread(thread)
        , timestamp(timestamp)
        , cpu(cpu)
        , start_instruction(start_instruction)
    {
    }
    bool
    operator!=(const schedule_entry_t &rhs)
    {
        return thread != rhs.thread || timestamp != rhs.timestamp || cpu != rhs.cpu ||
            start_instruction != rhs.start_instruction;
    }
    uint64_t thread;
    uint64_t timestamp;
    uint64_t cpu;
    uint64_t start_instruction;
} END_PACKED_STRUCTURE;

#if defined(BUILD_PT_TRACER) || defined(BUILD_PT_POST_PROCESSOR)

/******************************************************
 * Trace entries related to the kernel trace -- start.
 */
/**
 * The type of a syscall PT entry in the raw offline output.
 */
typedef enum {
    /**
     * The instances of this type store the PID, signifying which process the data in the
     * buffer has been collected from.
     */
    SYSCALL_PT_ENTRY_TYPE_PID = 0,
    /**
     * The instances of this type store the thread Id, signifying which thread the data in
     * the buffer has been collected from.
     */
    SYSCALL_PT_ENTRY_TYPE_THREAD_ID,
    /**
     * The instance with this type demonstrates that the leftover portion of the buffer
     * holds metadata while also providing information about the metadata's size.
     */
    SYSCALL_PT_ENTRY_TYPE_PT_METADATA_BOUNDARY,
    /**
     * The instance with this type demonstrates that the leftover portion of the buffer
     * holds one syscall's metadata and PT data while also providing information about all
     * these data's size.
     */
    SYSCALL_PT_ENTRY_TYPE_PT_DATA_BOUNDARY,
    /**
     * The instances of this type store the sysnum, indicating the type of syscall.
     */
    SYSCALL_PT_ENTRY_TYPE_SYSNUM,
    /**
     * The instances of this type store the syscall's index, which indicates the calling
     * index of the syscall in the current thread.
     */
    SYSCALL_PT_ENTRY_TYPE_SYSCALL_IDX,
    /**
     * The instances of this type store the syscall's arguments number, which indicates
     * the number of arguments of the syscall.
     */
    SYSCALL_PT_ENTRY_TYPE_SYSCALL_ARGS_NUM,
    SYSCALL_PT_ENTRY_TYPE_MAX
} syscall_pt_entry_type_t;

// All fields are little-endian.
START_PACKED_STRUCTURE
struct _syscall_pt_entry_t {
    union {
        struct {
            uint64_t pid : 59;
            uint64_t type : 5;
        } pid;
        struct {
            uint64_t tid : 59;
            uint64_t type : 5;
        } tid;
        struct {
            uint64_t data_size : 59;
            uint64_t type : 5;
        } pt_metadata_boundary;
        struct {
            uint64_t data_size : 59;
            uint64_t type : 5;
        } pt_data_boundary;
        struct {
            uint64_t sysnum : 59;
            uint64_t type : 5;
        } sysnum;
        struct {
            uint64_t idx : 59;
            uint64_t type : 5;
        } syscall_idx;
        struct {
            uint64_t args_num : 59;
            uint64_t type : 5;
        } syscall_args_num;
        uint64_t combined_value;
    };
} END_PACKED_STRUCTURE;
typedef struct _syscall_pt_entry_t syscall_pt_entry_t;

/**
 * The per-thread metadata and each syscall's PT data are stored in one PT DATA
 * Buffer(PDB). The PDB format is:
 * +----------+--------+
 * |PDB Header|PDB Data|
 * +----------+--------+
 *
 * The PDB Header is a list of syscall_pt_entry_t. There are two types of PDB Header:
 * a. The format of per-thread metadata's PDB header is:
 * +---+---+--------------------+
 * |pid|tid|pt_metadata_boundary|
 * +---+---+--------------------+
 * This header is stored in the first PDB of each thread. And the PDB Data of this PDB is
 * PT's metadata.
 *
 * b. The format of syscalls' PT data's PDB header is:
 * +---+---+----------------+------------------+
 * |pid|tid|pt_data_boundary|syscall's metadata|
 * +---+---+----------------+------------------+
 * This header is stored in the PDBs of each syscall. And the PDB Data of this PDB
 * contains the arguments of the syscall and the PT data of the syscall.
 */

/* The header of per-thread metadata buffer contains 3 syscall_pt_entry_t instances:
 * 1. The first instance is used to store the pid.
 * 2. The second instance is used to store the tid.
 * 3. The 3rd instance is used to store the output buffer's type and size.
 */
#    define PT_METADATA_PDB_HEADER_ENTRY_NUM 3
#    define PT_METADATA_PDB_HEADER_SIZE     \
        (PT_METADATA_PDB_HEADER_ENTRY_NUM * \
         sizeof(dynamorio::drmemtrace::syscall_pt_entry_t))
#    define PT_METADATA_PDB_DATA_OFFSET PT_METADATA_PDB_HEADER_SIZE

/* The header of each syscall's PT data buffer contains max 6 syscall_pt_entry_t
 * instances:
 * 1. The first instance is used to store the pid.
 * 2. The second instance is used to store the tid.
 * 3. The 3rd instance is used to store the output buffer's type and size.
 * 4. The 4th-6th instance is used to store the syscall's metadata.
 */
#    define PT_DATA_PDB_HEADER_ENTRY_NUM 6
#    define PT_DATA_PDB_HEADER_SIZE \
        (PT_DATA_PDB_HEADER_ENTRY_NUM * sizeof(dynamorio::drmemtrace::syscall_pt_entry_t))
#    define PT_DATA_PDB_DATA_OFFSET PT_DATA_PDB_HEADER_SIZE

/* The metadata of each syscall is stored in the PDB header. The metadata contains 3
 * syscall_pt_entry_t instances:
 * +--------+-------------+------------------+
 * |1.sysnum|2.syscall_idx|3.syscall_args_num|
 * +--------+-------------+------------------+
 */
#    define SYSCALL_METADATA_ENTRY_NUM 3
#    define SYSCALL_METADATA_SIZE \
        (SYSCALL_METADATA_ENTRY_NUM * sizeof(dynamorio::drmemtrace::syscall_pt_entry_t))

typedef enum {
    // TODO i#5505: We perhaps do not need to add the PID and TID in the header. They can
    // be obtained from the thread's user-space trace files.

    /* Index of a syscall PT entry of type SYSCALL_PT_ENTRY_TYPE_PID in the PDB header. */
    PDB_HEADER_PID_IDX = 0,
    /* Index of a syscall PT entry of type SYSCALL_PT_ENTRY_TYPE_THREAD_ID in the PDB
     * header.
     */
    PDB_HEADER_TID_IDX = 1,
    /* Index of a syscall PT entry of type SYSCALL_PT_ENTRY_TYPE_PT_DATA_BOUNDARY in the
     * PDB header.
     */
    PDB_HEADER_DATA_BOUNDARY_IDX = 2,
    /* Index of a syscall PT entry of type SYSCALL_PT_ENTRY_TYPE_SYSNUM in the PDB header.
     */
    PDB_HEADER_SYSNUM_IDX = 3,
    /* Index of a syscall PT entry of type SYSCALL_PT_ENTRY_TYPE_SYSCALL_IDX in the PDB
     * header.
     */
    PDB_HEADER_SYSCALL_IDX_IDX = 4,
    /* Index of a syscall PT entry of type SYSCALL_PT_ENTRY_TYPE_SYSCALL_ARGS_NUM in the
     * PDB header.
     */
    PDB_HEADER_NUM_ARGS_IDX = 5
} pdb_header_entry_idx_t;

/**
 * This is the format in which syscall_pt_trace writes the PT metadata for each
 * thread before writing any system call's PT data.
 * All fields are little-endian.
 */
START_PACKED_STRUCTURE
struct _pt_metadata_buf_t {
    /**
     * The header of the PT metadata.
     */
    syscall_pt_entry_t header[PT_METADATA_PDB_HEADER_ENTRY_NUM];

    /**
     * The PT metadata itself. Note that the struct is already marked packed
     * in its definition.
     */
    pt_metadata_t metadata;
} END_PACKED_STRUCTURE;

/** See #dynamorio::drmemtrace::_pt_metadata_buf_t. */
typedef struct _pt_metadata_buf_t pt_metadata_buf_t;

/**
 * This is the format in which syscall_pt_trace writes each system call's PT
 * data.
 */
struct _pt_data_buf_t {
    /**
     * The header of the PT data.
     */
    syscall_pt_entry_t header[PT_DATA_PDB_HEADER_ENTRY_NUM];
    /**
     * The actual trace data written by PT.
     */
    std::unique_ptr<uint8_t[]> data;
};

/** See #dynamorio::drmemtrace::_pt_data_buf_t. */
typedef struct _pt_data_buf_t pt_data_buf_t;

/****************************************************
 * Trace entries related to the kernel trace -- end.
 */

#endif // defined(BUILD_PT_TRACER) || defined(BUILD_PT_POST_PROCESSOR)

/**
 * The name of the file in -offline mode where module data is written.
 * Its creation can be customized using drmemtrace_custom_module_data()
 * and then modified before passing to raw2trace via
 * drmodtrack_add_custom_data() and drmodtrack_offline_write().
 * Use drmemtrace_get_modlist_path() to obtain the full path.
 */
#define DRMEMTRACE_MODULE_LIST_FILENAME "modules.log"

/**
 * The name of the file in -offline mode where function tracing names
 * are written.  Use drmemtrace_get_funclist_path() to obtain the full path.
 */
#define DRMEMTRACE_FUNCTION_LIST_FILENAME "funclist.log"

/**
 * The name of the file in -offline mode where non-module instruction encodings
 * are written.  Use drmemtrace_get_encoding_path() to obtain the full path.
 */
#define DRMEMTRACE_ENCODING_FILENAME "encodings.bin"

/**
 * The base name of the file in -offline mode where the serial thread schedule
 * is written during post-processing.  A compression suffix may be appended.
 */
#define DRMEMTRACE_SERIAL_SCHEDULE_FILENAME "serial_schedule.bin"

/**
 * The name of the archive file in -offline mode where the cpu thread schedule
 * is written during post-processing.  A separate sub-archive is written for
 * each cpu.
 */
#define DRMEMTRACE_CPU_SCHEDULE_FILENAME "cpu_schedule.bin.zip"

/**
 * The name of the folder in -offline mode where the kernel's per thread trace
 * data is stored.
 */
#define DRMEMTRACE_KERNEL_TRACE_SUBDIR "kernel.raw"

/**
 * The name of the file in -offline mode where the kernel code segments are stored. This
 * file is copied from '/proc/kcore' during tracing.
 */
#define DRMEMTRACE_KCORE_FILENAME "kcore"

/**
 * The name of the file in -offline mode where kallsyms is stored. This file is copied
 * from '/proc/kallsyms' during tracing.
 */
#define DRMEMTRACE_KALLSYMS_FILENAME "kallsyms"

/**
 * The name of the file in -offline mode where virtual to physical information is stored.
 * This file contains a mapping from virtual to physical addresses, the page size used,
 * the number of pages, and the number of bytes mapped.
 */
#define DRMEMTRACE_V2P_FILENAME "v2p.textproto"

} // namespace drmemtrace
} // namespace dynamorio

#endif /* _TRACE_ENTRY_H_ */
