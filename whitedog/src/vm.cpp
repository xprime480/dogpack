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
        if ((program_size + sizeof(int)) <= MAX_PROGRAM_SIZE)
        {
            memcpy((void *)&program[program_size], (void *)&val, sizeof(int));
            program_size += sizeof(int);
        }
        else
        {
            program_too_big();
        }
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

void VM::exec() const
{
    if (!valid_program)
    {
        cerr << "Cannot execute invalid program\n";
        return;
    }

    int stack[1024];
    unsigned int sp = 0;
    unsigned int pc = 0;

    while (pc < program_size)
    {
        OPCODE op = program[pc++];
        switch (op)
        {
        case PUSH:
        {
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
                cerr << "Cannot POP empty stack\n";
                return;
            }
            --sp;
        }
        break;

        case DUP:
        {
            if (0 == sp)
            {
                cerr << "Cannot DUP empty stack\n";
                return;
            }
            stack[sp] = stack[sp - 1];
            ++sp;
        }
        break;

        case ADD:
        {
            if (sp < 2)
            {
                cerr << "Too few items on stack to ADD\n";
                return;
            }

            stack[sp - 2] = stack[sp - 2] + stack[sp - 1];
            --sp;
        }
        break;

        case SUB:
        {
            if (sp < 2)
            {
                cerr << "Too few items on stack to SUB\n";
                return;
            }

            stack[sp - 2] = stack[sp - 2] - stack[sp - 1];
            --sp;
        }
            break;

        case MUL:
        {
            if (sp < 2)
            {
                cerr << "Too few items on stack to MUL\n";
                return;
            }

            stack[sp - 2] = stack[sp - 2] * stack[sp - 1];
            --sp;
        }
            break;

        case DIV:
        {
            if (sp < 2)
            {
                cerr << "Too few items on stack to DIV\n";
                return;
            }
            if (0 == stack[sp-1])
            {
                cerr << "Division by zero not allowed\n";
                return;
            }

            stack[sp - 2] = stack[sp - 2] / stack[sp - 1];
            --sp;
        }
            break;

        default:
            cerr << "Internal Error: Invalid OPCODE detected";
            break;
        }
    }

    if (0 == sp)
    {
        cerr << "Error: Program produced no value\n";
    }
    else
    {
        cout << "Program value is " << stack[sp-1] << "\n";
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

void VM::program_too_big()
{
    valid_program = false;
    cerr << "Maximum program size exceeded\n";
}
