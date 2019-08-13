#ifndef ERROR_H
#define ERROR_H

enum error_code {
    // Operation completed successfully
    OK,
    // Operation couldn't be completed
    ERROR,
    // ---------- VFS ----------
    // End of file reached, operation partially completed
    VFS_END_OF_FILE,
    // Tried to write to a read-only file
    VFS_NO_WRITE,
    // Tried to read a write-only file
    VFS_NO_READ,
    // ---------- SYSCALLS ----------
    // Address outside of 3GiB virtual range
    SYSCALL_INVALID_ADDRESS,
    // Unknown syscall
    SYSCALL_UNKNOWN,
};

#endif