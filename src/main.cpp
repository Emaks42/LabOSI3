#include <iostream>
#include <cstring>
#include "os_interaction/basic/os.h"

using namespace os;

int main() {
    File f1("1.sh_mem");
    File f2("2.sh_mem");
    File f3("3.sh_mem");
    ProcessStartInfo proc_info1, proc_info2;
    proc_info1.cmd = "./lab1_exe_child1";
    proc_info2.cmd = "./lab1_exe_child2";
    proc_info1.redirectStdin = f1;
    proc_info1.redirectStdout = f2;
    proc_info2.redirectStdin = f2;
    proc_info2.redirectStdout = f3;
    SharedMemory sm1 = create_shared_mem(f1);
    SharedMemory sm2 = create_shared_mem(f2);
    SharedMemory sm3 = create_shared_mem(f3);
    Process child1 = create_proc(proc_info1);
    Process child2 = create_proc(proc_info2);
    std::string inp;
    bool running = true;
    while (running) {
        inp = "";
        char c{};
        while (c != '\n') {
            c = getchar();
            inp.push_back(c);
            if (inp == "exit\n") {
                running = false;
                break;
            }
        }
        if (!running) {
            inp[0] = 1;
            shared_mem_write(f1, inp.data(), 1);
            break;
        }
        shared_mem_write(f1, inp.data(), inp.size());
        char result{};
        while (result != '\n') {
            lock_file(f3);
            shared_mem_read(f3, &result, 1);
            unlock_file(f3);
            if (result == 0) {
                // sleep(1);
                continue;
            }
            std::cout << result;
        }   
    }
    kill_proc(child1);
    kill_proc(child2);
    return 0;
}