#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
enum SizeBit {
    Size0,
    Size8 = 1 << 1,
    Size16 = 1 << 2,
    Size32 = 1 << 3,
    Size64 = 1 << 4
};
constexpr static SizeBit DispSize(uint64_t disp) {
    if (!disp)
        return SizeBit::Size0;
    if (disp < 0x100)
        return SizeBit::Size8;
    if (disp < 0x10000)
        return SizeBit::Size16;
    if (disp < 0x100000000)
        return SizeBit::Size32;
    return SizeBit::Size64;
}
enum InstructMod {
    SizeModNone = 0,
    SizeModOvrd = 1,
    SizeModREX = 2,
    SizeModReg = 4,
    SizeModCon = 8,
    SizeModFlt = 16,
};
enum InstrParamType {
    ParamNone = 0,
    ParamST0 = 1 << 4,
    ParamSTi = 1 << 5,
    ParamImm = 1 << 6,
    ParamRel = 1 << 7,
    ParamReg = 1 << 8,
    ParamMem = 1 << 9,
    ParamMM = 1 << 10,
    ParamXMM = 1 << 11,
    ParamForce = 1 << 12,
    ParamRegAH = 1 << 13 | ParamForce | Size8,
    ParamRegAX = 1 << 13 | ParamForce | Size16,
    ParamRegEAX = 1 << 13 | ParamForce | Size32,
    ParamRegRAX = 1 << 13 | ParamForce | Size64,
    ParamRegAL = 1 << 14 | ParamForce | Size8,
    ParamRegCL = 1 << 15 | ParamForce | Size8,
    ParamValue1 = 1 << 16 | ParamForce,
    ParamValue3 = 1 << 17 | ParamForce,
    ParamValuen = 1 << 18 | ParamForce,
    ParamMMMem = ParamMM | ParamMem,
    ParamXMMMem = ParamXMM | ParamMem,
    ParamImm8 = ParamImm | Size8,
    ParamImm16 = ParamImm | Size16,
    ParamImm32 = ParamImm | Size32,
    ParamReg8 = ParamReg | Size8,
    ParamReg16 = ParamReg | Size16,
    ParamReg32 = ParamReg | Size32,
    ParamReg64 = ParamReg | Size64,
    ParamMem8 = ParamMem | Size8,
    ParamMem16 = ParamMem | Size16,
    ParamMem32 = ParamMem | Size32,
    ParamMem64 = ParamMem | Size64,
    ParamRel8 = ParamRel | Size8,
    ParamRel16 = ParamRel | Size16,
    ParamRel32 = ParamRel | Size32,
    ParamRM = ParamReg | ParamMem,
    ParamRM8 = ParamReg8 | ParamMem8,
    ParamRM16 = ParamReg16 | ParamMem16,
    ParamRM32 = ParamReg32 | ParamMem32,
    ParamRM64 = ParamReg64 | ParamMem64,
};
struct Register {
    enum class ID {
        ax,
        cx,
        dx,
        bx,
        sp,
        bp,
        si,
        di,
        r8,
        r9,
        r10,
        r11,
        r12,
        r13,
        r14,
        r15,
        r0 = ax,
        r1 = cx,
        r2 = dx,
        r3 = bx,
        r4 = sp,
        r5 = bp,
        r6 = si,
        r7 = di,
        SIB_Index_ax = r0,
        SIB_Index_cx = r1,
        SIB_Index_dx = r2,
        SIB_Index_bx = r3,
        SIB_Index_bp = r5, // 无sp
        SIB_Index_si = r6,
        SIB_Index_di = r7,
        SIB_Index_None = r4, // Only Base 不允许[sp * 2] 如果为[esp+disp] 则SIB Index必须为r4
        ModRM_RM_ax = r0,
        ModRM_RM_cx = r1,
        ModRM_RM_dx = r2,
        ModRM_RM_bx = r3,
        ModRM_RM_bp = r5, // 无sp
        ModRM_RM_si = r6,
        ModRM_RM_di = r7,
        ModRM_RM_EnableSIB = r4,
        al = r0,
        cl = r1,
        dl = r2,
        bl = r3,
        ah = r4,
        ch = r5,
        dh = r6,
        bh = r7,
        /*ModRM_RM_Disp16 = r6, // 16位
        ModRM_RM_bx_si = r0,
        ModRM_RM_bx_di = r1,
        ModRM_RM_bp_si = r2,
        ModRM_RM_bp_di = r3,
        ModRM_RM_si = r4,
        ModRM_RM_di = r5,
        ModRM_RM_bp = r6,
        ModRM_RM_bx = r7,*/
    };
    SizeBit size;
    ID reg;
    inline InstrParamType type() {
        if (reg == ID::ax)
            return (InstrParamType) (ParamRegAL | size);
        if (reg == ID::cx && size == Size8)
            return ParamRegCL;
        return (InstrParamType) (ParamRM | size);

    }
    constexpr static Register al() { return {SizeBit::Size8, ID::ax}; }
    constexpr static Register bl() { return {SizeBit::Size8, ID::bx}; }
    constexpr static Register cl() { return {SizeBit::Size8, ID::cx}; }
    constexpr static Register dl() { return {SizeBit::Size8, ID::dx}; }
    constexpr static Register ah() { return {SizeBit::Size8, ID::ah}; }
    constexpr static Register bh() { return {SizeBit::Size8, ID::bh}; }
    constexpr static Register ch() { return {SizeBit::Size8, ID::ch}; }
    constexpr static Register dh() { return {SizeBit::Size8, ID::dh}; }
    constexpr static Register eax() { return {SizeBit::Size32, ID::ax}; }
    constexpr static Register ebx() { return {SizeBit::Size32, ID::bx}; }
    constexpr static Register ecx() { return {SizeBit::Size32, ID::cx}; }
    constexpr static Register edx() { return {SizeBit::Size32, ID::dx}; }
    constexpr static Register rax() { return {SizeBit::Size64, ID::ax}; }
    constexpr static Register rbx() { return {SizeBit::Size64, ID::bx}; }
    constexpr static Register rcx() { return {SizeBit::Size64, ID::cx}; }
    constexpr static Register rdx() { return {SizeBit::Size64, ID::dx}; }
};
using RegID = Register::ID;
struct X64Instruct {
    const char *name;
    int mod;
    uint8_t reg_op_extern;
    uint32_t opcode = 0;
    InstrParamType op1 = ParamNone;
    InstrParamType op2 = ParamNone;
    InstrParamType op3 = ParamNone;
    inline bool empty() { return !opcode; }
    inline bool need_REX_W() { return (mod & SizeModREX); }
    inline bool need_ModRM() {
        return (op1 & ParamReg) || (op2 & ParamReg) || (op3 & ParamReg);
    }
    inline bool need_Reg() {
        return mod & SizeModReg;
    }
    inline SizeBit size() { return DispSize(opcode); }
    inline uint32_t length() {
        switch (DispSize(opcode)) {
            case Size0:
                return 0;
            case Size8:
                return 1;
            case Size16:
                return 2;
            case Size32:
                return 4;
            case Size64:
                return 8;
        }
        return 0;
    }
    inline uint8_t *begin() { return (uint8_t *) (&opcode); }
    inline uint8_t *end() { return begin() + length(); }
    X64Instruct operator+(RegID id) {
        return {name, mod, reg_op_extern, opcode + (uint32_t) id, op1, op2, op3};
    }
};
static X64Instruct Instructs[] = {
    #include "instruct.inc"
};
X64Instruct GetInstruct(const char *name, int op1 = ParamNone, int op2 = ParamNone, int op3 = ParamNone) {
    for (auto &instr : Instructs) {
        if (strcmp(instr.name, name) == 0) {
            if ((instr.op1 & op1) == op1 && (instr.op2 & op2) == op2 && (instr.op3 & op3) == op3) {
                return instr;
            }
        }
    }
    return {};
}
enum Mode {
    Mode_Mem,
    Mode_Mem_Disp8,
    Mode_Mem_Disp32,
    Mode_Reg
};
enum Scale {
    Scale1,
    Scale2,
    Scale4,
    Scale8,
};

constexpr uint8_t RemoveREX(RegID id) {
    return (uint8_t(id) & 0b111);
}
constexpr uint8_t GetREX(RegID id) {
    return (uint8_t(id) >> 3);
}
constexpr uint8_t ModRM(Mode mod, RegID reg, RegID rm = RegID::ax) {
    return ((mod << 6) | (RemoveREX(reg) << 3) | (RemoveREX(rm)));
}
constexpr uint8_t SIB(RegID base, RegID index = RegID::SIB_Index_None, Scale scale = Scale1) {
    return ((scale << 6) | (RemoveREX(index) << 3) | (RemoveREX(base)));
}
constexpr uint8_t REX(bool SIB_base, bool SIB_index, bool ModRM_reg, bool W = true) {
    return 0b00100000 | (W << 3) | (ModRM_reg << 2) | (SIB_index << 1) | (SIB_base);
}
constexpr uint8_t REXPrefix(bool W, RegID reg = RegID::ax, RegID base = RegID::ax, RegID index = RegID::SIB_Index_None) {
    return 0b01000000 | (W << 3) | (GetREX(reg) << 2) | (GetREX(index) << 1) | GetREX(base);
}
constexpr uint8_t OperandOverridePrefix = 0x66;
constexpr uint8_t AddressOverridePrefix = 0x67;

int main() {
    std::vector<uint8_t> bytes;
    auto ins = GetInstruct("add", ParamReg | Size64, ParamMem | Size64);
    if (ins.need_REX_W()) {
        bytes.push_back(REXPrefix(ins.need_REX_W()));
    }
    if (ins.need_Reg()) {
        ins = ins + RegID::ax;
    }
    bytes.insert(bytes.end(), ins.begin(), ins.end());
    if (ins.need_ModRM()) {
        bytes.push_back(ModRM(Mode::Mode_Mem_Disp8, RegID::ax, RegID::ax));
    }
    //bytes.push_back(SIB(RegID::ax));
    bytes.push_back(8); // Disp
    for (auto &byte : bytes) {
        std::cout << (int) byte << " ";
    }
    std::cout << std::endl;
    return 0;
}
