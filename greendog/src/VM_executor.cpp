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

        case ADD:
            do_instructions(
                [this, &instr]() {
                    registers[instr.r3] = registers[instr.r1] + registers[instr.r2];
                },
                "ADD");
            break;

        case SUB:
            do_instructions(
                [this, &instr]() {
                    registers[instr.r3] = registers[instr.r1] - registers[instr.r2];
                },
                "SUB");
            break;

        case MUL:
            do_instructions(
                [this, &instr]() {
                    registers[instr.r3] = registers[instr.r1] * registers[instr.r2];
                },
                "MUL");
            break;

        case DIV:
            do_instructions(
                [this, &instr]() {
                    int divisor = registers[instr.r2];
                    if (divisor == 0)
                    {
                        status = "Division by Zero";
                    }
                    else
                    {
                        registers[instr.r3] = registers[instr.r1] / divisor;
                    }
                },
                "DIV");
            break;

        case CMP:
            do_instructions(
                [this, &instr]() {
                    const int l = registers[instr.r1];
                    const int r = registers[instr.r2];
                    if ( l < r )
                    {
                        registers[instr.r3] = -1;
                    }
                    else if ( l == r )
                    {
                        registers[instr.r3] = 0;
                    }
                    else
                    {
                        registers[instr.r3] = 1;
                    }
                },
                "CMP");
            break;

        case JMP:
            do_jump(
                instr,
                [this, &instr]() -> bool { return true; },
                "JMP");
            break;

        case JEQ:
            do_jump(
                instr,
                [this, &instr]() -> bool { return registers[instr.r1] == 0; },
                "JEQ");
            break;

        case JNE:
            do_jump(
                instr,
                [this, &instr]() -> bool { return registers[instr.r1] != 0; },
                "JNE");
            break;

        case JLT:
            do_jump(
                instr,
                [this, &instr]() -> bool { return registers[instr.r1] < 0; },
                "JLT");
            break;

        case JLE:
            do_jump(
                instr,
                [this, &instr]() -> bool { return registers[instr.r1] <= 0; },
                "JLE");
            break;

        case JGT:
            do_jump(
                instr,
                [this, &instr]() -> bool { return registers[instr.r1] > 0; },
                "JGT");
            break;

        case JGE:
            do_jump(
                instr,
                [this, &instr]() -> bool { return registers[instr.r1] >= 0; },
                "JGE");
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

void VM_executor::do_jump(Instruction const & instr, std::function<bool()> test, const char *name)
{
    do_instructions([this, &instr, test]() {
        if (instr.loc >= program_size)
        {
            status = "branch beyond end of program";
            return;
        }
        if ( test() )
        {
            pc = instr.loc;
        }
    }, name);
}

void VM_executor::trace(Instruction const & instr) const
{
    cerr << "---------------------\n";

    // by the time we get here PC has already been incremented from the
    // instruction location.
    cerr << "PC: " << (pc-1) << "\n";

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

    case ADD:
        cerr << "ADD r" << instr.r1 << " r" << instr.r2 << " r" << instr.r3 << " (" << registers[instr.r1]
        << " + " << registers[instr.r2] << ")\n"; 
        break;

    case SUB:
        cerr << "SUB r" << instr.r1 << " r" << instr.r2 << " r" << instr.r3 << " (" << registers[instr.r1]
        << " - " << registers[instr.r2] << ")\n"; 
        break;

    case MUL:
        cerr << "MUL r" << instr.r1 << " r" << instr.r2 << " r" << instr.r3 << " (" << registers[instr.r1]
        << " * " << registers[instr.r2] << ")\n"; 
        break;

    case DIV:
        cerr << "DIV r" << instr.r1 << " r" << instr.r2 << " r" << instr.r3 << " (" << registers[instr.r1]
        << " / " << registers[instr.r2] << ")\n"; 
        break;

    case CMP:
        cerr << "CMP r" << instr.r1 << " r" << instr.r2 << " r" << instr.r3 << " (" << registers[instr.r1]
        << " <=> " << registers[instr.r2] << ")\n"; 
        break;

    case JMP:
        cerr << "JMP " << instr.loc << "\n";
        break; 

    case JEQ:
        cerr << "JEQ r" << instr.r1 << " " << instr.loc  << " (" << registers[instr.r1] << ")\n";
        break; 

    case JNE:
        cerr << "JNE r" << instr.r1 << " " << instr.loc << " (" << registers[instr.r1] << ")\n";
        break; 

    case JLT:
        cerr << "JLT r" << instr.r1 << " " << instr.loc << " (" << registers[instr.r1] << ")\n";
        break; 

    case JLE:
        cerr << "JLE r" << instr.r1 << " " << instr.loc << " (" << registers[instr.r1] << ")\n";
        break; 

    case JGT:
        cerr << "JGT r" << instr.r1 << " " << instr.loc << " (" << registers[instr.r1] << ")\n";
        break; 

    case JGE:
        cerr << "JGE r" << instr.r1 << " " << instr.loc << " (" << registers[instr.r1] << ")\n";
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

        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case CMP:
            decode_RRR(code);
            break;

        case JMP:
            decode_L(code);
            break;

        case JEQ:
        case JNE:
        case JLT:
        case JLE:
        case JGT:
        case JGE:
            decode_RL(code);
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

void VM_executor::Instruction::decode_RRR(unsigned int code)
{
    r1 = (code >> 16) & 0xFF;
    r2 = (code >> 8) & 0xFF;
    r3 = code & 0xFF;
}

void VM_executor::Instruction::decode_L(unsigned int code)
{
    loc = code & 0xFFFF;
}

void VM_executor::Instruction::decode_RL(unsigned int code)
{
    r1 = (code >> 16) & 0xFF;
    loc = code & 0xFFFF;
}



