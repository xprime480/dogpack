
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

        if (verbose)
        {
            trace(pc, stack, sp);
        }

        OPCODE op = program[pc++];
        switch (op)
        {
        case PUSH:
            do_instructions(
                [this]() {
                    int val;
                    memcpy((void *)&val, (void *)&program[pc], sizeof(int));
                    pc += sizeof(int);
                    stack[sp++] = val;
                },
                0, 1, "PUSH");
            break;

        case POP:
            do_instructions(
                [this]() {
                    --sp;
                },
                1, 0, "POP");
            break;

        case DUP:
            do_instructions(
                [this]() {
                    stack[sp] = stack[sp - 1];
                    ++sp;
                },
                1, 1, "DUP");
            is_stack_available("DUP");
            break;

        case DUPN:
            do_instructions(
                [this]() {
                    int target;
                    memcpy((void *)&target, (void *)&program[pc], sizeof(int));
                    pc += sizeof(int);
                    if (target <= 0 || target > (int)sp)
                    {
                        status = "DUPN index out of range";
                        return;
                    }
                    stack[sp] = stack[sp - target];
                    ++sp;
                },
                1, 1, "DUPN");
            is_stack_available("DUPN");
            break;

        case DROPN:
            do_instructions(
                [this]() {
                    int target;
                    memcpy((void *)&target, (void *)&program[pc], sizeof(int));
                    pc += sizeof(int);
                    if (target <= 0 || target > (int)sp)
                    {
                        status = "DROPN index out of range";
                        return;
                    }
                    for (unsigned int i = sp-target ; i < sp ; ++i)
                    {
                        stack[i] = stack[i+1];
                    }
                    --sp;
                },
                1, 0, "DROPN");
            break;

        case SWAP:
            do_instructions(
                [this]() {
                    std::swap(stack[sp - 2], stack[sp - 1]);
                },
                2, 0, "SWAP");
            break;

        case ADD:
            do_instructions(
                [this]() {
                    stack[sp - 2] = stack[sp - 2] + stack[sp - 1];
                    --sp;
                },
                2, 0, "ADD");
            break;

        case SUB:
            do_instructions(
                [this]() {
                    stack[sp - 2] = stack[sp - 2] - stack[sp - 1];
                    --sp;
                },
                2, 0, "SUB");
            break;

        case MUL:
            do_instructions(
                [this]() {
                    stack[sp - 2] = stack[sp - 2] * stack[sp - 1];
                    --sp;
                },
                2, 0, "MUL");
            break;

        case DIV:
            do_instructions(
                [this]() {
                    if (0 == stack[sp - 1])
                    {
                        status = "Division by zero not allowed";
                        return;
                    }

                    stack[sp - 2] = stack[sp - 2] / stack[sp - 1];
                    --sp;
                },
                2, 0, "DIV");
            break;

        case CMP:
            do_instructions(
                [this]() {
                    int lhs = stack[sp - 2];
                    int rhs = stack[sp - 1];

                    stack[sp - 2] = (lhs < rhs) ? -1 : ((lhs > rhs) ? +1 : 0);
                    --sp;
                },
                2, 0, "CMP");
            break;

        case JMP:
            do_jump(
                [this]() -> bool {
                    return true;
                },
                0, "JMP");
            break;

        case JEQ:
            do_jump(
                [this]() -> bool {
                    return stack[--sp] == 0;
                },
                1, "JEQ");
            break;

        case JNE:
            do_jump(
                [this]() -> bool {
                    return stack[--sp] != 0;
                },
                1, "JNE");
            break;

        case JLT:
            do_jump(
                [this]() -> bool {
                    return stack[--sp] < 0;
                },
                1, "JLT");
            break;

        case JLE:
            do_jump(
                [this]() -> bool {
                    return stack[--sp] <= 0;
                },
                1, "JLE");
            break;

        case JGT:
            do_jump(
                [this]() -> bool {
                    return stack[--sp] > 0;
                },
                1, "JGT");
            break;

        case JGE:
            do_jump(
                [this]() -> bool {
                    return stack[--sp] >= 0;
                },
                1, "JGE");
            break;

        default:
            status = "Internal Error: Invalid OPCODE detected";
            break;
        }
    }

    if (status.empty() && 0 == sp)
    {
        status = "Program produced no value";
    }
    if (!status.empty())
    {
        return VM_exec_status(status);
    }

    return VM_exec_status(int(stack[sp - 1]));
}

void VM_executor::reset()
{
    pc = sp = ticks = 0;
    status = "";
}

void VM_executor::is_stack_available(const char *name)
{
    if (MAX_STACK_SIZE == sp)
    {
        status = "Stack overflow on ";
        status += name;
    }
}

void VM_executor::is_arg_available(size_t count, const char *name)
{
    if (sp < count)
    {
        status = "Not enough arguments on stack for ";
        status += name;
    }
}

int VM_executor::get_jump_target()
{
    int index;
    memcpy((void *)&index, (void *)&program[pc], sizeof(int));
    int target = labels.pc_at(index);
    if (target < 0)
    {
        status = "Label was never defined";
    }
    return target;
}

void VM_executor::do_instructions(std::function<void(void)> instr, size_t argcount, size_t stackneeded, const char *name)
{
    if (argcount > 0 && status.empty())
    {
        is_arg_available(argcount, name);
    }
    if (stackneeded > 0 && status.empty())
    {
        is_stack_available(name);
    }
    if (status.empty())
    {
        instr();
    }
}

void VM_executor::do_jump(std::function<bool(void)> check, size_t argcount, const char *name)
{
    do_instructions(
        [this, check]() {
            int target = get_jump_target();
            if (target < 0)
            {
                return;
            }

            bool jumping = check();
            if (jumping)
            {
                pc = (unsigned int)target;
            }
            else
            {
                pc += sizeof(int);
            }
        },
        argcount, 0, name);
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

    case DUPN:
    {
        int val;
        memcpy((void *)&val, (void *)&program[pc], sizeof(int));
        cerr << "DUPN " << val << "\n";
        break;
    }

    case DROPN:
    {
        int val;
        memcpy((void *)&val, (void *)&program[pc], sizeof(int));
        cerr << "DROPN " << val << "\n";
        break;
    }

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
        int target = labels.pc_at(index);
        if (target < 0)
        {
            cerr << " (never defined)";
        }
    }

    cerr << "\n";
}
