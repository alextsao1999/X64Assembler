#include <iostream>
#include <vector>
#include "tiny_asm.h"
int main() {
    std::vector<uint8_t> bytes;
    using namespace Emitter;
    Emit(bytes, "push", rbp);
    Emit(bytes, "add", rcx, rdx);
    Emit(bytes, "mov", rdx, rbx);
    Emit(bytes, "movss", xmm(0), xmm(1));
    Emit(bytes, "push", imm::dword(1234));
    Emit(bytes, "leave");
    Emit(bytes, "ret", imm::word(4));
    std::cout << "uint8_t codes[] = {";
    for (auto &byte : bytes) {
        std::cout << (int) byte ;
        if (&byte != &bytes.back()) {
            std::cout << ",";
        }
    }
    std::cout << "};";
    return 0;
}
