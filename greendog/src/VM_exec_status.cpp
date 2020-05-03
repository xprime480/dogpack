#include "../include/VM_exec_status.hpp"

using namespace std;

VM_exec_status::VM_exec_status(int value)
    : is_ok(true), value(value), msg("Execution OK")
{
}

VM_exec_status::VM_exec_status(const string & msg)
    : is_ok(false), value(-1), msg(msg)
{
}

bool VM_exec_status::is_status_ok() const
{
    return is_ok;
}

int VM_exec_status::get_program_value() const
{
    return value;
}

const string &VM_exec_status::get_message() const
{
    return msg;
}
