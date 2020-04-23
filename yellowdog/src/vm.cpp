#include <cstring>
#include <iostream>

#include "../include/vm.hpp"

using namespace std;

VM::VM()
    : program_size(0u), valid_program(true)
{
}

void VM::push(int val)
{
    if (maybe_add_op(PUSH))
    {
        maybe_add_arg(val);
    };
}

void VM::pop()
{
    maybe_add_op(POP);
}

void VM::dup()
{
    maybe_add_op(DUP);
}

void VM::swap()
{
    maybe_add_op(SWAP);
}

void VM::add()
{
    maybe_add_op(ADD);
}

void VM::sub()
{
    maybe_add_op(SUB);
}

void VM::mul()
{
    maybe_add_op(MUL);
}

void VM::div()
{
    maybe_add_op(DIV);
}

void VM::cmp()
{
    maybe_add_op(CMP);
}

void VM::jmp(const std::string &target)
{
    maybe_add_jmp(JMP, target);
}

void VM::jeq(const std::string &target)
{
    maybe_add_jmp(JEQ, target);
}

void VM::jne(const std::string &target)
{
    maybe_add_jmp(JNE, target);
}

void VM::jlt(const std::string &target)
{
    maybe_add_jmp(JLT, target);
}

void VM::jle(const std::string &target)
{
    maybe_add_jmp(JLE, target);
}

void VM::jgt(const std::string &target)
{
    maybe_add_jmp(JGT, target);
}

void VM::jge(const std::string &target)
{
    maybe_add_jmp(JGE, target);
}

void VM::label(const std::string &target)
{
    if (!valid_program)
    {
        // cerr << "Program already invalid\n";
        return;
    }

    if (labels.add_or_update(target, program_size) < 0)
    {
        // cerr << "becoming invalid because of bad return code from labels.add_or_update\n";
        valid_program = false;
    }
}

VM_exec_status VM::exec(bool verbose) const
{
    if (verbose)
    {
        cerr << "Starting program execution\n";
        labels.dump();
        cerr << "program_size = " << program_size << "\n";
    }

    if (!valid_program)
    {
        return VM_exec_status("Cannot execute invalid program");
    }

    static const unsigned int MAX_STACK_SIZE = 1024;
    static const unsigned int MAX_TICKS = 102400;

    int stack[MAX_STACK_SIZE];
    unsigned int sp = 0;
    unsigned int pc = 0;
    unsigned ticks = 0;

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

void VM::maybe_add_jmp(OPCODE op, string const &target)
{
    if (maybe_add_op(op))
    {
        int index = labels.add_or_update(target, -1);
        maybe_add_arg(index);
    }
}

bool VM::maybe_add_op(OPCODE op)
{
    if (valid_program)
    {
        if (program_size < MAX_PROGRAM_SIZE)
        {
            program[program_size++] = op;
        }
        else
        {
            program_too_big();
        }
    }

    return valid_program;
}

bool VM::maybe_add_arg(int arg)
{
    if ((program_size + sizeof(int)) <= MAX_PROGRAM_SIZE)
    {
        memcpy((void *)&program[program_size], (void *)&arg, sizeof(int));
        program_size += sizeof(int);
    }
    else
    {
        program_too_big();
    }

    return valid_program;
}

void VM::program_too_big()
{
    valid_program = false;
}

void VM::trace(unsigned int pc, int *stack, unsigned int sp) const
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

void VM::trace_jmp(string const &op, unsigned int pc) const
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
