#include <iostream>

#include "../include/vm.hpp"

using namespace std;

namespace
{
bool EXPECT_ERROR(VM &vm, const char *msg)
{
    VM_exec_status status = vm.exec();
    if (status.is_status_ok())
    {
        cerr << "[FAIL] " << msg << ", expected Error, got" << status.get_program_value() << "\n";
        return false;
    }
    else
    {
        cerr << "[PASS] " << msg << "\n";
        return true;
    }
}

bool EXPECT_VALUE(VM &vm, const char *msg, int value)
{
    VM_exec_status status = vm.exec();
    if (status.is_status_ok())
    {
        int program_value = status.get_program_value();
        if (value == program_value)
        {
            cerr << "[PASS] " << msg << "\n";
            return true;
        }
        else
        {
            cerr << "[FAIL] " << msg << ", expected " << value << ", got" << program_value << "\n";
            return false;
        }
    }
    else
    {
        cerr << "[FAIL] " << msg << ", expected value " << value << ", got error" << status.get_message() << "\n";
        return false;
    }
}

bool empty_program()
{
    VM vm;
    return EXPECT_ERROR(vm, "Empty program");
}

bool push()
{
    VM vm;
    vm.push(1);
    return EXPECT_VALUE(vm, "Just Push", 1);
}

bool pop_to_empty()
{
    VM vm;
    vm.push(1);
    vm.pop();
    return EXPECT_ERROR(vm, "Pop to Empty");
}

bool pop_from_empty()
{
    VM vm;
    vm.pop();
    return EXPECT_ERROR(vm, "Pop from Empty");
}

bool pop()
{
    VM vm;
    vm.push(1);
    vm.push(2);
    vm.pop();
    return EXPECT_VALUE(vm, "Pop", 1);
}

bool dup_from_empty()
{
    VM vm;
    vm.dup();
    return EXPECT_ERROR(vm, "Dup from Empty");
}

bool dup()
{
    VM vm;
    vm.push(1);
    vm.dup();
    vm.add();
    return EXPECT_VALUE(vm, "Dup", 2);
}

bool add_too_few()
{
    VM vm;
    vm.push(1);
    vm.add();
    return EXPECT_ERROR(vm, "Add Too Few");
}

bool add()
{
    VM vm;
    vm.push(1);
    vm.push(33);
    vm.add();
    return EXPECT_VALUE(vm, "Add", 34);
}

class Runner
{
    public:
    Runner()
    : count(0u), passed(0u)
    {
    }

    void operator()(bool (*fn)())
    {
        ++count;
        if ( fn() )
        {
            ++passed;
        }
    }

    int report() const
    {
        cout << "\n" << passed << " passed out of " << count << " tests\n";
        return passed == count ? 0 : 1;
    }

    private:
    size_t count;
    size_t passed;
};

} // namespace

int main(void)
{
    Runner runner;

    runner(empty_program);
    runner(push);
    runner(pop_to_empty);
    runner(pop_from_empty);
    runner(pop);
    runner(dup_from_empty);
    runner(dup);
    runner(add_too_few);
    runner(add);

    return runner.report();
}
