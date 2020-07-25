//
// Created by Alex on 7/26/2020.
//

#ifndef X64ASSEMBLER_TINY_ASM_H
#define X64ASSEMBLER_TINY_ASM_H

#define EMIT_ASSERT(cond, msg) if (!(cond)) std::cout << msg;

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
enum SizeBit {
    Size0,
    Size8 = 1,
    Size16 = 1 << 1,
    Size32 = 1 << 2,
    Size64 = 1 << 3
};
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
    ParamST0 = (1 << 4),
    ParamSTi = (1 << 5) | ParamST0,
    ParamImm = (1 << 6),
    ParamRel = (1 << 7),
    ParamReg = (1 << 8),
    ParamMem = (1 << 9),
    ParamMM = (1 << 10),
    ParamXMM = (1 << 11),
    ParamForce = (1 << 12),
    ParamRegA = (1 << 13)  | ParamForce | ParamReg,
    ParamRegAL = (ParamRegA | Size8),
    ParamRegAX = (ParamRegA | Size16),
    ParamRegEAX = (ParamRegA | Size32),
    ParamRegRAX = (ParamRegA | Size64),
    ParamRegC = (1 << 14) | ParamForce | ParamReg,
    ParamRegCL = ((ParamRegC) | Size8),
    ParamValue1 = (1 << 16) | ParamImm | ParamForce,
    ParamValue3 = (1 << 17) | ParamImm | ParamForce,
    ParamValuen = (1 << 18) | ParamImm | ParamForce,
    ParamMMMem = ((ParamMM) | ParamMem),
    ParamXMMMem = ((ParamXMM) | ParamMem),
    ParamImm8 = ((ParamImm) | Size8),
    ParamImm16 = ((ParamImm) | Size16),
    ParamImm32 = ((ParamImm) | Size32),
    ParamReg8 = ((ParamReg) | Size8),
    ParamReg16 = ((ParamReg) | Size16),
    ParamReg32 = ((ParamReg) | Size32),
    ParamReg64 = ((ParamReg) | Size64),
    ParamMem8 = ((ParamMem) | Size8),
    ParamMem16 = ((ParamMem) | Size16),
    ParamMem32 = ((ParamMem) | Size32),
    ParamMem64 = ((ParamMem) | Size64),
    ParamRel8 = ((ParamRel) | Size8),
    ParamRel16 = ((ParamRel) | Size16),
    ParamRel32 = ((ParamRel) | Size32),
    ParamRM = ((ParamReg) | ParamMem),
    ParamRM8 = ((ParamReg8) | ParamMem8),
    ParamRM16 = ((ParamReg16) | ParamMem16),
    ParamRM32 = ((ParamReg32) | ParamMem32),
    ParamRM64 = ((ParamReg64) | ParamMem64),
};
enum class RegID {
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
    ModRM_RM_Null = r5,
    al = r0,
    cl = r1,
    dl = r2,
    bl = r3,
    ah = r4,
    ch = r5,
    dh = r6,
    bh = r7,
    xmm0 = r0,
    xmm1 = r1,
    xmm2 = r2,
    xmm3 = r3,
    xmm4 = r4,
    xmm5 = r5,
    xmm6 = r6,
    xmm7 = r7,
    mm0 = r0,
    mm1 = r1,
    mm2 = r2,
    mm3 = r3,
    mm4 = r4,
    mm5 = r5,
    mm6 = r6,
    mm7 = r7,
    st0 = r0,
    st1 = r1,
    st2 = r2,
    st3 = r3,
    st4 = r4,
    st5 = r5,
    st6 = r6,
    st7 = r7,
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
constexpr SizeBit DispSize(uint64_t disp) {
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
constexpr uint32_t SizeBitByte(SizeBit size) {
    switch(size) {
        case Size0: return 0;
        case Size8: return 1;
        case Size16: return 2;
        case Size32: return 4;
        case Size64: return 8;
    }
    return 0;
}
constexpr uint32_t DispByte(uint64_t disp) {
    if (!disp)
        return 0;
    if (disp < 0x100)
        return 1;
    if (disp < 0x10000)
        return 2;
    if (disp < 0x100000000)
        return 4;
    return 8;
}
constexpr uint32_t ValidByte(uint8_t bytes) {
    return (1 << (bytes * 8));
}
constexpr uint32_t CountByte(uint64_t value) {
    if (!value)
        return 0;
    if (value < 0x100)
        return 1;
    if (value < 0x10000)
        return 2;
    if (value < 0x1000000)
        return 3;
    if (value < 0x100000000)
        return 4;
    return 0;
}

constexpr SizeBit ParamSize(int type) {
    return (SizeBit) (type & 0b1111);
}

struct STReg {
    RegID reg;
    STReg() = default;
    STReg(RegID reg) : reg(reg) {}
    inline InstrParamType type() const {
        if (reg == RegID::st0) {
            return (InstrParamType) (ParamST0);
        }
        return (InstrParamType) (ParamSTi);
    }
};
struct MMReg {
    RegID reg;
    MMReg() = default;
    MMReg(RegID reg) : reg(reg) {}
    inline InstrParamType type() const { return ParamMM; }
};
struct XMMReg {
    RegID reg;
    XMMReg() = default;
    XMMReg(RegID reg) : reg(reg) {}
    inline InstrParamType type() const { return ParamXMM; }

};
struct Register {
    SizeBit size;
    RegID reg;
    inline InstrParamType type() const {
        if (reg == RegID::ax) {
            return (InstrParamType) (ParamRegA | size);
        }
        if (reg == RegID::cx) {
            return (InstrParamType) (ParamRegC | size);
        }
        return (InstrParamType) (ParamReg | size);
    }
    Register() = default;
    constexpr Register(SizeBit size, RegID reg) : size(size), reg(reg) {}
    inline bool empty() const { return size == Size0; }
    constexpr static Register al() { return {SizeBit::Size8, RegID::ax}; }
    constexpr static Register bl() { return {SizeBit::Size8, RegID::bx}; }
    constexpr static Register cl() { return {SizeBit::Size8, RegID::cx}; }
    constexpr static Register dl() { return {SizeBit::Size8, RegID::dx}; }
    constexpr static Register ah() { return {SizeBit::Size8, RegID::ah}; }
    constexpr static Register bh() { return {SizeBit::Size8, RegID::bh}; }
    constexpr static Register ch() { return {SizeBit::Size8, RegID::ch}; }
    constexpr static Register dh() { return {SizeBit::Size8, RegID::dh}; }
    constexpr static Register eax() { return {SizeBit::Size32, RegID::ax}; }
    constexpr static Register ebx() { return {SizeBit::Size32, RegID::bx}; }
    constexpr static Register ecx() { return {SizeBit::Size32, RegID::cx}; }
    constexpr static Register edx() { return {SizeBit::Size32, RegID::dx}; }
    constexpr static Register esp() { return {SizeBit::Size32, RegID::sp}; }
    constexpr static Register ebp() { return {SizeBit::Size32, RegID::bp}; }
    constexpr static Register rax() { return {SizeBit::Size64, RegID::ax}; }
    constexpr static Register rbx() { return {SizeBit::Size64, RegID::bx}; }
    constexpr static Register rcx() { return {SizeBit::Size64, RegID::cx}; }
    constexpr static Register rdx() { return {SizeBit::Size64, RegID::dx}; }
    constexpr static Register rbp() { return {SizeBit::Size64, RegID::bp}; }
    constexpr static Register rsp() { return {SizeBit::Size64, RegID::sp}; }
    constexpr static Register rdi() { return {SizeBit::Size64, RegID::di}; }
    constexpr static Register rsi() { return {SizeBit::Size64, RegID::si}; }
};
struct Address {
    SizeBit size;
    Register base;
    Register index;
    Scale scale;
    int32_t disp;
    Address(SizeBit size, Register base, Register index = {Size0, RegID::SIB_Index_None}, Scale scale = Scale1, int32_t disp = 0) :
            size(size), base(base), index(index), scale(scale), disp(disp) {
        if (disp < 0) {
            this->disp = ValidByte(DispByte(-disp)) + disp;
        }
    }
    inline InstrParamType type() const {
        return (InstrParamType) (ParamMem | size);
    }
    inline Mode get_mode() const {
        auto byte = DispByte(disp);
        if (byte == 0) {
            return Mode::Mode_Mem;
        } else if (byte == 1) {
            return Mode::Mode_Mem_Disp8;
        } else {
            return Mode::Mode_Mem_Disp32;
        }
    }
    inline bool need_SIB() const {
        return !(index.empty() && base.reg != RegID::ModRM_RM_EnableSIB);
    }
    inline bool need_disp() const { return DispSize(disp) >= Size8; }
    inline uint8_t *disp_begin() const { return (uint8_t *) (&disp); }
    inline uint8_t *disp_end() const { return disp_begin() + SizeBitByte(DispSize(disp)); }
    inline bool is_valid() {
        if (index.size != Size0) {
            return index.size == base.size;
        } else {
            return base.size != Size0;
        }
    }
    static Address Byte(int32_t disp, Register base, Register index = {Size0, RegID::SIB_Index_None}, Scale scale = Scale1) {
        return {Size8, base, index, scale, disp};
    }
    static Address Word(int32_t disp, Register base, Register index = {Size0, RegID::SIB_Index_None}, Scale scale = Scale1) {
        return {Size16, base, index, scale, disp};
    }
    static Address DWord(int32_t disp, Register base, Register index = {Size0, RegID::SIB_Index_None}, Scale scale = Scale1) {
        return {Size32, base, index, scale, disp};
    }
    static Address QWord(int32_t disp, Register base, Register index = {Size0, RegID::SIB_Index_None}, Scale scale = Scale1) {
        return {Size64, base, index, scale, disp};
    }
};
struct Immdiate {
    SizeBit size;
    int32_t immdiate;
    Immdiate(int32_t immdiate, SizeBit size = Size0) {
        this->size = size;
        this->immdiate = immdiate;
        if (size == Size0) {
            this->size = DispSize(immdiate);
        }
        if (immdiate < 0) {
            this->immdiate = (ValidByte(SizeBitByte(this->size)) + immdiate);
        }
    }
    inline InstrParamType type() const {
        return (InstrParamType) (ParamImm | size);
    }
    inline uint32_t length() const {
        return SizeBitByte(size);
    }
    inline uint8_t *begin() const { return (uint8_t *) (&immdiate); }
    inline uint8_t *end() const { return begin() + length(); }
    static inline Immdiate Byte(int32_t imm) { return {imm, Size8}; }
    static inline Immdiate Word(int32_t imm) { return {imm, Size16}; }
    static inline Immdiate DWord(int32_t imm) { return {imm, Size32}; }
};
struct RelOffset {
    SizeBit size;
    int32_t offset;
    RelOffset(int32_t offset, SizeBit size = Size0) : offset(offset) {
        this->size = size;
        this->offset = offset;
        if (size == Size0) {
            this->size = DispSize(offset);
        }
        if (offset < 0) {
            this->offset = (ValidByte(SizeBitByte(this->size)) + offset);
        }
    }
    inline InstrParamType type() const {
        return (InstrParamType) (ParamRel | size);
    }
    inline uint32_t length() const {
        return SizeBitByte(size);
    }
    inline uint8_t *begin() const { return (uint8_t *) (&offset); }
    inline uint8_t *end() const { return begin() + length(); }

};

struct X64Instruct {
    const char *name;
    int mod;
    uint8_t reg_op_extern;
    uint32_t opcode = 0;
    InstrParamType op1 = ParamNone;
    InstrParamType op2 = ParamNone;
    InstrParamType op3 = ParamNone;
    inline bool empty() { return !opcode; }
    inline bool need_Override() { return (mod & SizeModOvrd); }
    inline bool need_REX_W() { return (mod & SizeModREX); }
    inline bool need_ModRM() { return (op1 & ParamReg) || (op2 & ParamReg) || (op3 & ParamReg); }
    inline bool need_Reg() { return mod & SizeModReg; }
    inline bool need_Flt() { return mod & SizeModFlt; }
    inline uint32_t length() { return CountByte(opcode); }
    inline uint8_t *begin() { return (uint8_t *) (&opcode); }
    inline uint8_t *end() { return begin() + length(); }
    X64Instruct operator+(RegID id) {
        return {name, mod, reg_op_extern, opcode + (uint32_t) id, op1, op2, op3};
    }
};
static X64Instruct Instructs[] = {
    #include "instructions.h"
};
inline bool IsMatch(int op, int your) {
    if ((op & ParamMem) && (ParamSize(op) == Size0) && (your & ParamMem)) {
        return true;
    }
    if ((your & (ParamForce | ParamReg)) == (ParamForce | ParamReg)) {
        return ((op & ParamReg) == ParamReg) && (ParamSize(your) == ParamSize(op));
    } else {
        return ((op & your) == your) && (ParamSize(your) == ParamSize(op));
    }
}
X64Instruct GetInstruct(const char *name, InstrParamType op1 = ParamNone, InstrParamType op2 = ParamNone, InstrParamType op3 = ParamNone) {
    for (auto &instr : Instructs) {
        if (strcmp(instr.name, name) == 0) {
            if (IsMatch(instr.op1, op1) && IsMatch(instr.op2, op2) && IsMatch(instr.op3, op3)) {
                return instr;
            }
        }
    }
    return {};
}

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
constexpr uint8_t NeedREXPrefix(bool W, RegID reg = RegID::ax, RegID base = RegID::ax, RegID index = RegID::SIB_Index_None) {
    return W | GetREX(reg) | GetREX(index) | GetREX(base);
}
constexpr uint8_t OperandOverridePrefix = 0x66;
constexpr uint8_t AddressOverridePrefix = 0x67;

template<class Container>
void EmitInst(Container &bytes, X64Instruct ins, STReg &r) {
    if (ins.need_Flt()) {
        ins.opcode = ins.opcode + ((uint32_t) (r.reg) << 7);
        bytes.insert(bytes.end(), ins.begin(), ins.end());
    }
}
template<class Container>
void EmitInst(Container &bytes, X64Instruct ins, STReg &r, STReg &m) {
    if (ins.need_Flt()) {
        if (ins.op1 == ParamST0) {
            ins.opcode = ins.opcode + ((uint32_t) (m.reg) << 8);
        } else {
            ins.opcode = ins.opcode + ((uint32_t) (r.reg) << 8);
        }
        bytes.insert(bytes.end(), ins.begin(), ins.end());
    }
}
template<class Container, class R>
void EmitInst(Container &bytes, X64Instruct ins, R &r, R &m) {
    bytes.insert(bytes.end(), ins.begin(), ins.end());
    bytes.push_back(ModRM(Mode_Reg, r.reg, m.reg));
}
template<class Container, class R>
void EmitInst(Container &bytes, X64Instruct ins, R &r, Address &m) {
    EMIT_ASSERT(m.is_valid(), "Invalid Addressing!");
    if (NeedREXPrefix(ins.need_REX_W(), r.reg, m.base.reg, m.index.reg)) {
        bytes.push_back(REXPrefix(ins.need_REX_W(), r.reg, m.base.reg, m.index.reg));
    }
    bytes.insert(bytes.end(), ins.begin(), ins.end());
    if (m.need_SIB()) {
        bytes.push_back(ModRM(m.get_mode(), r.reg, RegID::ModRM_RM_EnableSIB));
        bytes.push_back(SIB(m.base.reg, m.index.reg, m.scale));
    } else {
        bytes.push_back(ModRM(m.get_mode(), r.reg, m.base.reg));
    }
    if (m.need_disp()) {
        bytes.insert(bytes.end(), m.disp_begin(), m.disp_end());
    }
}
template<class Container, class R>
void EmitInst(Container &bytes, X64Instruct ins, Address &m, R &r) {
    EmitInst(bytes, ins, r, m);
}

template<class Container>
void EmitInst(Container &bytes, X64Instruct ins) {
    if (ins.need_REX_W()) {
        bytes.push_back(REXPrefix(ins.need_REX_W()));
    }
    bytes.insert(bytes.end(), ins.begin(), ins.end());
}
template<class Container>
void EmitInst(Container &bytes, X64Instruct ins, const Immdiate &m) {
    if (ins.need_REX_W()) {
        bytes.push_back(REXPrefix(ins.need_REX_W()));
    }
    bytes.insert(bytes.end(), ins.begin(), ins.end());
    bytes.insert(bytes.end(), m.begin(), m.end());
}
template<class Container>
void EmitInst(Container &bytes, X64Instruct ins, const RelOffset &rel) {
    if (ins.need_REX_W()) {
        bytes.push_back(REXPrefix(ins.need_REX_W()));
    }
    bytes.insert(bytes.end(), ins.begin(), ins.end());
    bytes.insert(bytes.end(), rel.begin(), rel.end());
}
template<class Container>
void EmitInst(Container &bytes, X64Instruct ins, const Register &r) {
    if (ins.need_REX_W()) {
        bytes.push_back(REXPrefix(ins.need_REX_W()));
    }
    if (ins.need_Reg()) {
        ins = ins + r.reg;
        bytes.insert(bytes.end(), ins.begin(), ins.end());
    } else if (ins.need_ModRM()) {
        bytes.insert(bytes.end(), ins.begin(), ins.end());
        bytes.push_back(ModRM(Mode_Reg, RegID::r2, r.reg));
    }

}
template<class Container>
void EmitInst(Container &bytes, X64Instruct ins, const Address &r) {
    if (ins.need_REX_W()) {
        bytes.push_back(REXPrefix(ins.need_REX_W()));
    }
    if (ins.need_Reg()) {
        EMIT_ASSERT(false, "not handle the reg");
    } else if (ins.need_ModRM()) {
        bytes.insert(bytes.end(), ins.begin(), ins.end());
        if (r.need_SIB()) {
            bytes.push_back(ModRM(r.get_mode(), RegID::ax, RegID::ModRM_RM_EnableSIB));
            bytes.push_back(SIB(r.base.reg, r.index.reg, r.scale));
        } else {
            bytes.push_back(ModRM(r.get_mode(), r.base.reg, RegID::ModRM_RM_Null));
        }
        if (r.need_disp()) {
            bytes.insert(bytes.end(), r.disp_begin(), r.disp_end());
        }
    }
}
template<class Container>
void EmitInst(Container &bytes, X64Instruct ins, Register &r, Immdiate &m) {
    if (ins.need_REX_W()) {
        bytes.push_back(REXPrefix(ins.need_REX_W()));
    }
    if (ins.need_Reg()) {
        ins = ins + r.reg;
        bytes.insert(bytes.end(), ins.begin(), ins.end());
    } else if (ins.need_ModRM()) {
        bytes.insert(bytes.end(), ins.begin(), ins.end());
        bytes.push_back(ModRM(Mode_Reg, r.reg));
    }
    bytes.insert(bytes.end(), m.begin(), m.end());
}
template<class Container>
void EmitInst(Container &bytes, X64Instruct ins, Register &r, Register &m) {
    if (ins.need_Reg()) {
        if (ins.op1 & ParamForce) {
            ins = ins + r.reg;
        } else {
            ins = ins + m.reg;
        }
        bytes.insert(bytes.end(), ins.begin(), ins.end());
    } else if (ins.need_ModRM()) {
        EMIT_ASSERT((r.reg != m.reg), "same register error");
        if (NeedREXPrefix(ins.need_REX_W(), r.reg)) {
            bytes.push_back(REXPrefix(ins.need_REX_W(), r.reg, m.reg));
        }
        bytes.insert(bytes.end(), ins.begin(), ins.end());
        bytes.push_back(ModRM(Mode_Reg, r.reg, m.reg)); // Reg模式不需要Enable SIB
    }
}
template<class Container>
void EmitInst(Container &bytes, X64Instruct ins, Register &r, Address &m) {
    if (ins.need_Reg()) {
        ins = ins + r.reg;
        bytes.insert(bytes.end(), ins.begin(), ins.end());
        std::cout << "r-a-addressing" << std::endl;
    } else if (ins.need_ModRM()) {
        EMIT_ASSERT(m.is_valid(), "Invalid Addressing!");
        if ((m.base.size == Size32) || (m.index.size == Size32)) {
            // 32位转为64位
            bytes.push_back(AddressOverridePrefix);
        }
        if (NeedREXPrefix(ins.need_REX_W(), r.reg, m.base.reg, m.index.reg)) {
            bytes.push_back(REXPrefix(ins.need_REX_W(), r.reg, m.base.reg, m.index.reg));
        }
        bytes.insert(bytes.end(), ins.begin(), ins.end());
        if (m.need_SIB()) {
            bytes.push_back(ModRM(m.get_mode(), r.reg, RegID::ModRM_RM_EnableSIB));
            bytes.push_back(SIB(m.base.reg, m.index.reg, m.scale));
        } else {
            bytes.push_back(ModRM(m.get_mode(), r.reg, m.base.reg));
        }
        if (m.need_disp()) {
            bytes.insert(bytes.end(), m.disp_begin(), m.disp_end());
        }
    }
}
template<class Container>
void EmitInst(Container &bytes, X64Instruct ins, Register &r, Register &m, Immdiate &imm) {
    EmitInst(bytes, ins, r, m);
    bytes.insert(bytes.end(), imm.begin(), imm.end());
}
template<class Container>
void EmitInst(Container &bytes, X64Instruct ins, Address &m, Register &r) {
    EmitInst(bytes, ins, r, m);
}

template<class T, class ...K>
bool Emit(T &bytes, const char *name, K... op) {
    auto ins = GetInstruct(name, (op.type())...);
    if (ins.empty()) {
        return false;
    }
    if (ins.need_Override()) {
        bytes.push_back(OperandOverridePrefix);
    }
    EmitInst(bytes, ins, op...);
    return true;
}

#endif //X64ASSEMBLER_TINY_ASM_H
