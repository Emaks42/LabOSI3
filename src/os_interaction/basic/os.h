#pragma once

#include "defines.h"

using BYTE = unsigned char;

namespace os {
struct FileHandleDummy { };
typedef FileHandleDummy* FileHandle;

struct ProcessHandleDummy { };
typedef ProcessHandleDummy* ProcessHandle;

class Process {
public:
    int pid = -1;
    ProcessHandle handle = nullptr;
    Process(int pid_, ProcessHandle handle_): pid(pid_), handle(handle_) {};
    ~Process() {
        delete handle;
    }
};

class File {
public:
    int id = -1;
    FileHandle handle = nullptr;
    File(int id_, FileHandle handle_): id(id_), handle(handle_) {};
    File(std::string filename);
    ~File() {
        delete handle;
    }
};

struct SharedMemory {
public:
    void* memptr = nullptr;
    File file;
    int memsize = 0;
    SharedMemory(void* ptr, File f, int memsize_): memptr(ptr), file(f), memsize(memsize_) {};
    ~SharedMemory();
};


struct ProcessStartInfo {

    std::string cmd;
    std::vector<std::string> args;

    File redirectStdin = {-1,nullptr};
    File redirectStdout = {-1,nullptr};

    ProcessStartInfo() = default;
};

Process create_proc(ProcessStartInfo& args);

void close_proc(Process proc);

void kill_proc(Process proc, uint32_t ret = -1);

bool create_pipe(File& readPipe, File& writePipe);

int pipe_write(File pipe, const void* buf, int count);

int pipe_read(File pipe, void* buf, int count);

int shared_mem_write(File mem, const void* buf, int count);

int shared_mem_read(File mem, void* buf, int count);

void lock_file(File mem);

void unlock_file(File mem);

SharedMemory create_shared_mem(File& file, int size = 256);

int get_current_pid();

File get_stdin();

File get_stdout();

void close_file(File f);
} // namespace