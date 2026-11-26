#include <iostream>
#include <vector>
#include <string>
#include "os.h"

using namespace os;

int main() {
    std::vector<char> prev_c{0}, c{0};
    std::string s = "", s_ = "";
    File input = get_stdin();
    File output = get_stdout();
    while (1) {
        s = "";
        lock_file(input);
        while (1) {
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
        }

        if (c[0] == 1) {
            break;
        }

        if (s.size() == 0) {
            continue;
        }

        s += '\n';

        s_ = "";
        s_ += s[0];
        for (std::size_t i{1}; i < s.size(); ++i) {
            if (!((s[i-1] == s[i]) && (s[i] == ' '))) {
                s_ += s[i];
            }
        }
        os::shared_mem_write(output, s_.data(), s_.size());
    }
    return 0;
}