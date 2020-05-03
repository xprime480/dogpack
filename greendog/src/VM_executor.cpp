#include "VM_executor.hpp"

#include <iostream>

using namespace std;

VM_executor::VM_executor(unsigned int const *program, unsigned int length, int * heap)
    : program(program), program_size(length), heap(heap)
{
    reset();
}

VM_exec_status VM_executor::exec(bool verbose)
{
    reset();

    while (status.empty() && pc < program_size)
    {
        if (verbose && (ticks % 1000) == 0)
        {
            cerr << ticks << " ticks\n";
        }
        if (++ticks > MAX_TICKS)
        {
            status = "Max Runtime Exceeded";
            break;
        }

       Instruction instr(program[pc++], verbose);
        
        if (verbose)
        {
            trace(instr);
        }

        switch (instr.op)
        {
        case LOAD:
            do_instructions(
                [this, &instr]() {
                    registers[instr.r1] = heap[instr.addr];
                }, "LOAD");
            break;

        case STORE:
            do_instructions(
                [this, &instr]() {
                    heap[instr.addr] = registers[instr.r1];
                },
                "STORE");
            break;

        default:
            status = "Internal Error: Invalid OPCODE detected";
            break;
        }
    }

    if (!status.empty())
    {
        return VM_exec_status(status);
    }

    return VM_exec_status(registers[0]);
}

void VM_executor::reset()
{
    pc = ticks = 0;
    status = "";
}

void VM_executor::do_instructions(std::function<void(void)> instr, const char *name)
{
    if (status.empty())
    {
        instr();
    }
}

void VM_executor::trace(Instruction const & instr) const
{
    cerr << "---------------------\n";

    cerr << "PC: " << pc << "\n";

    OPCODE op = instr.op;
    cerr << "op: " << (unsigned int)op << "\n";
    switch (op)
    {
    case LOAD:
        cerr << "LOAD r" << instr.r1 << " " << instr.addr << " (" << heap[instr.addr] <<")\n";
    break;

    case STORE:
        cerr << "STORE r" << instr.r1 << " " << instr.addr << " (" << registers[instr.r1] <<")\n";
        break;

    default:
        cerr << "unknown op code: " << op << "\n";
    }
}

VM_executor::Instruction::Instruction(unsigned int code, bool verbose)
{
    if ( verbose )
    {
        std::ios_base::fmtflags f( cerr.flags() );
        cerr << "decoding instruction " << hex << code << "\n";
        cerr.flags( f );
    }

    op = code >> 24;
    switch (op) {
        case LOAD:
        case STORE:
            decode_RA(code);
            break;

        default:
        op = 0;
        break;
    }
}

void VM_executor::Instruction::decode_RA(unsigned int code)
{
    r1 = (code >> 16) & 0xFF;
    addr = code & 0xFFFF;
}
