
#include "VM_executor.hpp"

#include <cstring>
#include <iostream>

using namespace std;

VM_executor::VM_executor(OPCODE const *program, unsigned int length, VM_labels const &labels)
    : program(program), program_size(length), labels(labels)
{
    reset();
}

VM_exec_status VM_executor::exec(bool verbose)
{
    reset();

    while (pc < program_size)
    {
        if (verbose && (ticks % 1000) == 0)
        {
            cerr << ticks << " ticks\n";
        }
        if (++ticks > MAX_TICKS)
        {
            return VM_exec_status("Max Runtime Exceeded");
        }

        if (verbose)
        {
            trace(pc, stack, sp);
        }

        OPCODE op = program[pc++];
        switch (op)
        {
        case PUSH:
        {
            if (MAX_STACK_SIZE == sp)
            {
                return VM_exec_status("Stack overflow on PUSH");
            }

            int val;
            memcpy((void *)&val, (void *)&program[pc], sizeof(int));
            pc += sizeof(int);
            stack[sp++] = val;
        }
        break;

        case POP:
        {
            if (0 == sp)
            {
                return VM_exec_status("Cannot POP empty stack");
            }
            --sp;
        }
        break;

        case DUP:
        {
            if (MAX_STACK_SIZE == sp)
            {
                return VM_exec_status("Stack overflow on DUP");
            }
            if (0 == sp)
            {
                return VM_exec_status("Cannot DUP empty stack");
            }
            stack[sp] = stack[sp - 1];
            ++sp;
        }
        break;

        case SWAP:
        {
            if (sp < 2)
            {
                return VM_exec_status("Too few items on stack to SWAP");
            }

            std::swap(stack[sp - 2], stack[sp - 1]);
        }
        break;

        case ADD:
        {
            if (sp < 2)
            {
                return VM_exec_status("Too few items on stack to ADD");
            }

            stack[sp - 2] = stack[sp - 2] + stack[sp - 1];
            --sp;
        }
        break;

        case SUB:
        {
            if (sp < 2)
            {
                return VM_exec_status("Too few items on stack to SUB");
            }

            stack[sp - 2] = stack[sp - 2] - stack[sp - 1];
            --sp;
        }
        break;

        case MUL:
        {
            if (sp < 2)
            {
                return VM_exec_status("Too few items on stack to MUL");
            }

            stack[sp - 2] = stack[sp - 2] * stack[sp - 1];
            --sp;
        }
        break;

        case DIV:
        {
            if (sp < 2)
            {
                return VM_exec_status("Too few items on stack to DIV");
            }
            if (0 == stack[sp - 1])
            {
                return VM_exec_status("Division by zero not allowed");
            }

            stack[sp - 2] = stack[sp - 2] / stack[sp - 1];
            --sp;
        }
        break;

        case CMP:
        {
            if (sp < 2)
            {
                return VM_exec_status("Too few items on stack to CMP");
            }

            int lhs = stack[sp - 2];
            int rhs = stack[sp - 1];

            stack[sp - 2] = (lhs < rhs) ? -1 : ((lhs > rhs) ? +1 : 0);
            --sp;
        }
        break;

        case JMP:
        {
            int index;
            memcpy((void *)&index, (void *)&program[pc], sizeof(int));
            int next_pc = labels.pc_at(index);
            if (next_pc < 0)
            {
                return VM_exec_status("Label was never defined");
            }

            pc = (unsigned int)next_pc;
        }
        break;

        case JEQ:
        {
            if (sp < 1)
            {
                return VM_exec_status("Too few items on stack to JEQ");
            }
            int test = stack[--sp];

            int index;
            memcpy((void *)&index, (void *)&program[pc], sizeof(int));
            int next_pc = labels.pc_at(index);
            if (next_pc < 0)
            {
                return VM_exec_status("Label was never defined");
            }

            if (test == 0)
            {
                pc = (unsigned int)next_pc;
            }
            else
            {
                pc += sizeof(int);
            }
        }
        break;

        default:
            return VM_exec_status("Internal Error: Invalid OPCODE detected");
        }
    }

    if (0 == sp)
    {
        return VM_exec_status("Program produced no value");
    }

    return VM_exec_status(int(stack[sp - 1]));
}

void VM_executor::reset()
{
    pc = sp = ticks = 0;
}

void VM_executor::trace(unsigned int pc, int *stack, unsigned int sp) const
{
    cerr << "---------------------\n";

    cerr << "STACK[" << sp << "]: ";
    for (unsigned int i = 3; i > 0 && sp > 0; --i)
    {
        cerr << stack[--sp] << " ";
    }
    cerr << "\n";

    cerr << "PC: " << pc << "\n";

    OPCODE op = program[pc++];
    cerr << "op: " << (unsigned int)op << "\n";
    switch (op)
    {
    case PUSH:
    {
        int val;
        memcpy((void *)&val, (void *)&program[pc], sizeof(int));
        cerr << "PUSH " << val << "\n";
    }
    break;

    case POP:
        cerr << "POP\n";
        break;

    case DUP:
        cerr << "DUP\n";
        break;

    case ADD:
        cerr << "ADD\n";
        break;

    case SUB:
        cerr << "SUB\n";
        break;

    case MUL:
        cerr << "MUL\n";
        break;

    case DIV:
        cerr << "DIV\n";
        break;

    case CMP:
        cerr << "CMP\n";
        break;

    case JMP:
        cerr << "trace jmp\n";
        trace_jmp("JMP", pc);
        break;

    case JEQ:
        trace_jmp("JEQ", pc);
        break;

    case JNE:
        trace_jmp("JNE", pc);
        break;

    case JLT:
        trace_jmp("JLT", pc);
        break;

    case JLE:
        trace_jmp("JLE", pc);
        break;

    case JGT:
        trace_jmp("JGT", pc);
        break;

    case JGE:
        trace_jmp("JGE", pc);
        break;

    default:
        cerr << "unknown op code: " << op << "\n";
    }
}

void VM_executor::trace_jmp(string const &op, unsigned int pc) const
{
    int index;
    memcpy((void *)&index, (void *)&program[pc], sizeof(int));
    cerr << "Jump Index = " << index << "\n";

    string const &name = labels.name_at(index);
    if (name.empty())
    {
        cerr << op << " ???";
    }
    else
    {
        cerr << op << " " << name;
        int next_pc = labels.pc_at(index);
        if (next_pc < 0)
        {
            cerr << " (never defined)";
        }
    }

    cerr << "\n";
}
