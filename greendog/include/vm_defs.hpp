#if !defined(VM_DEFS_HPP)
#define VM_DEFS_HPP 1

const constexpr unsigned int MAX_TICKS = 102400;
const constexpr unsigned int MAX_PROGRAM_SIZE = 1024u;
const constexpr unsigned int MAX_REGISTERS    = 32u;
const constexpr unsigned int MAX_HEAP_SIZE    = 8192u;

using OPCODE = unsigned char;

const constexpr OPCODE LOAD = 1;
const constexpr OPCODE STORE = 2;

const constexpr OPCODE ADD = 6;
const constexpr OPCODE SUB = 7;
const constexpr OPCODE MUL = 8;
const constexpr OPCODE DIV = 9;
const constexpr OPCODE CMP = 10;
const constexpr OPCODE JMP = 11;
const constexpr OPCODE JEQ = 12;
const constexpr OPCODE JNE = 13;
const constexpr OPCODE JLT = 14;
const constexpr OPCODE JLE = 15;
const constexpr OPCODE JGT = 16;
const constexpr OPCODE JGE = 17;

#endif
