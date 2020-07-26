# x64Assembler
a tiny cpp x64 assembler

## Usage

```c++
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
    // constexpr instruction
    auto instr = GetInstruct("add", ParamReg64, ParamImm32);
    instr.emit(bytes, rax, imm::dword(1234));
    
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
```
## Output
```C
uint8_t codes[] = {85,72,1,209,72,139,211,243,15,16,193,104,210,4,0,0,201,194,4,0,72,5,192,210,4,0,0};
```

