#if !defined(VM_HPP)
#define VM_HPP 1

#include "VM_exec_status.hpp"
#include "VM_defs.hpp"

class VM
{
public:
    VM();

    void load(unsigned int reg, unsigned int addr);
    void store(unsigned int reg, unsigned int addr);

    void add(unsigned int r1, unsigned int r2, unsigned int r3);
    void sub(unsigned int r1, unsigned int r2, unsigned int r3);
    void mul(unsigned int r1, unsigned int r2, unsigned int r3);
    void div(unsigned int r1, unsigned int r2, unsigned int r3);
    void cmp(unsigned int r1, unsigned int r2, unsigned int r3);

    void jmp(unsigned int loc);
    void jeq(unsigned int reg, unsigned int loc);
    void jne(unsigned int reg, unsigned int loc);
    void jlt(unsigned int reg, unsigned int loc);
    void jle(unsigned int reg, unsigned int loc);
    void jgt(unsigned int reg, unsigned int loc);
    void jge(unsigned int reg, unsigned int loc);


    VM_exec_status exec(bool verbose = false);

    void set_heap(unsigned int addr, int value);
    int get_heap(unsigned int addr) const;
    void dump_heap(unsigned int from, unsigned int to) const;

private:
    unsigned int program_size;
    bool valid_program;
    unsigned int program[MAX_PROGRAM_SIZE];
    int heap[MAX_HEAP_SIZE];

    bool check_program_size();
    bool check_register(unsigned int reg);
    bool check_address(unsigned int addr);
    bool check_location(unsigned int loc);

    void maybe_add_op_RA(OPCODE op, unsigned int reg, unsigned int addr);
    void maybe_add_op_RRR(OPCODE op, unsigned int r1, unsigned int r2, unsigned int r3);
    void maybe_add_op_L(OPCODE op, unsigned int loc);
    void maybe_add_op_RL(OPCODE op, unsigned int reg, unsigned int loc);
};

#endif
