#include "vm.hpp"

#include <iostream>

#include "VM_executor.hpp"

using namespace std;

VM::VM()
    : program_size(0u), valid_program(true)
{
}

void VM::load(unsigned int reg, unsigned int addr)
{
    maybe_add_op_RA(LOAD, reg, addr);
}

void VM::store(unsigned int reg, unsigned int addr)
{
    maybe_add_op_RA(STORE, reg, addr);
}

void VM::add(unsigned int r1, unsigned int r2, unsigned int r3)
{
    maybe_add_op_RRR(ADD, r1, r2, r3);
}

void VM::sub(unsigned int r1, unsigned int r2, unsigned int r3)
{
    maybe_add_op_RRR(SUB, r1, r2, r3);
}

void VM::mul(unsigned int r1, unsigned int r2, unsigned int r3)
{
    maybe_add_op_RRR(MUL, r1, r2, r3);
}

void VM::div(unsigned int r1, unsigned int r2, unsigned int r3)
{
    maybe_add_op_RRR(DIV, r1, r2, r3);
}

void VM::cmp(unsigned int r1, unsigned int r2, unsigned int r3)
{
    maybe_add_op_RRR(CMP, r1, r2, r3);
}

VM_exec_status VM::exec(bool verbose)
{
    if (!valid_program)
    {
        return VM_exec_status("Cannot execute invalid program");
    }

    if (verbose)
    {
        cerr << "Starting program execution\n";
//        labels.dump();
        cerr << "program_size = " << program_size << "\n";
    }

    VM_executor executor(program, program_size, heap);
    VM_exec_status rv = executor.exec(verbose);
    if ( verbose ) 
    {
        dump_heap(0, 4);
    }
    return rv;
}

void VM::set_heap(unsigned int addr, int value)
{
    if (addr < MAX_HEAP_SIZE)
    {
        heap[addr] = value;
    }
}

int VM::get_heap(unsigned int addr) const
{
    if (addr < MAX_HEAP_SIZE)
    {
        return heap[addr];
    }

    return 0xdeadbeef;
}

void VM::dump_heap(unsigned int from, unsigned int to) const
{
    cerr << "Heap (" << &heap[0] << "):\n";
    while ( from <= to ) 
    {
        cerr << "\t" << from << ":\t" << get_heap(from) << "\n";
        ++from;
    }
}

bool VM::check_program_size()
{
    if (valid_program)
    {
        if (program_size >= MAX_PROGRAM_SIZE)
        {
            valid_program = false;
        }
    }
    return valid_program;
}

bool VM::check_register(unsigned int reg)
{
    if (valid_program)
    {
        if (reg >= MAX_REGISTERS)
        {
            valid_program = false;
        }
    }
    return valid_program;
}

bool VM::check_address(unsigned int addr)
{
    if (valid_program)
    {
        if (addr >= MAX_HEAP_SIZE)
        {
            valid_program = false;
        }
    }
    return valid_program;
}

void VM::maybe_add_op_RA(OPCODE op, unsigned int reg, unsigned int addr)
{
    if (valid_program)
    {
        if (check_program_size() && check_register(reg) && check_address(addr))
        {
            unsigned int instr = (((unsigned int)op) << 24) | (reg << 16) | addr;
            program[program_size++] = instr;
        }
    }
}

void VM::maybe_add_op_RRR(OPCODE op, unsigned int r1, unsigned int r2, unsigned int r3)
{
    if (valid_program)
    {
        if (check_program_size() && check_register(r1) && check_register(r2) && check_register(r3))
        {
            unsigned int instr = (((unsigned int)op) << 24) | 
                                  (r1 << 16) | 
                                  (r2 << 8) |
                                  r3;
            program[program_size++] = instr;
        }
    }
}