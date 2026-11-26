#include <iostream>
#include <vector>
#include <string>
#include "os.h"

using namespace os;

int main() {
    std::vector<char> c{0};
    std::string s = "";
    File input = get_stdin();
    File output = get_stdout();
    while (1) {
        s = "";
        lock_file(input);
        do {
            os::shared_mem_read(input, c.data(), 1);
            if (c[0] == 0 || c[0] == '\n') {
                unlock_file(input);
                // sleep(1);
                break;
            }
            if (c[0] == 1) {
                unlock_file(input);
                os::shared_mem_write(output, c.data(), 1);
                break;
            }
            s += c[0];
        } while (c[0] != 0);

        if (c[0] == 1) {
            break;
        }

        if (s.size() == 0) {
            continue;
        }

        for (std::size_t i{}; i < s.size(); ++i) {
            if (('A' <= s[i]) && (s[i] <= 'Z')) {
                s[i] += 'a' - 'A';
            }
        }
        s += '\n';
        os::shared_mem_write(output, s.data(), s.size());
    }    
    return 0;
}