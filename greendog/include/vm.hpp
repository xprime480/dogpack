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
    void maybe_add_op_RA(OPCODE op, unsigned int reg, unsigned int addr);
    void maybe_add_op_RRR(OPCODE op, unsigned int r1, unsigned int r2, unsigned int r3);
};

#endif
