#include <iostream>

#include "../include/vm.hpp"

using namespace std;

namespace
{
void EXPECT_ERROR(VM &vm, const char *msg)
{
    VM_exec_status status = vm.exec();
    if (status.is_status_ok())
    {
        cerr << "[FAIL] " << msg << ", expected Error, got" << status.get_program_value() << "\n";
    }
    else
    {
        cerr << "[PASS] " << msg << "\n";
    }
}

void EXPECT_VALUE(VM &vm, const char *msg, int value)
{
    VM_exec_status status = vm.exec();
    if (status.is_status_ok())
    {
        int program_value = status.get_program_value();
        if (value == program_value)
        {
            cerr << "[PASS] " << msg << "\n";
        }
        else
        {
            cerr << "[FAIL] " << msg << ", expected " << value << ", got" << program_value << "\n";
        }
    }
    else
    {
        cerr << "[FAIL] " << msg << ", expected value " << value << ", got error" << status.get_message() << "\n";
    }
}

void empty_program_test()
{
    VM vm;
    EXPECT_ERROR(vm, "Empty program");
}

void trivial_program_test()
{
    VM vm;
    vm.push(1);
    EXPECT_VALUE(vm, "Just Push", 1);
}

} // namespace

int main(void)
{
    empty_program_test();
    trivial_program_test();
}
