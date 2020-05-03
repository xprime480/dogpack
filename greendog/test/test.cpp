
#include <functional>
#include <iostream>
#include <string>

#include "vm.hpp"
#include "VM_exec_status.hpp"

using namespace std;

namespace
{
class Runner
{
public:
    Runner()
        : count(0u), passed(0u)
    {
    }

    void operator()(std::function<bool()> fn)
    {
        ++count;
        if (fn())
        {
            ++passed;
        }
    }

    int report() const
    {
        cout << "\n"
             << passed << " passed out of " << count << " tests\n";
        return passed == count ? 0 : 1;
    }

private:
    size_t count;
    size_t passed;
};

bool EXPECT_ERROR(VM &vm, string const &label, bool verbose = false)
{
    VM_exec_status status = vm.exec(verbose);
    if (status.is_status_ok())
    {
        cerr << "[FAIL] " << label << ", expected Error, got " << status.get_program_value() << "\n";
        return false;
    }
    else
    {
        cerr << "[PASS] " << label << "\n";
        if (verbose)
        {
            cerr << "\tactual error message: " << status.get_message() << "\n";
        }
        return true;
    }
}

bool EXPECT_VALUE(VM &vm, string const &label, int value, bool verbose = false)
{
    VM_exec_status status = vm.exec(verbose);
    if (status.is_status_ok())
    {
        int program_value = status.get_program_value();
        if (value == program_value)
        {
            cerr << "[PASS] " << label << "\n";
            return true;
        }
        else
        {
            cerr << "[FAIL] " << label << ", expected " << value << ", got " << program_value << "\n";
            return false;
        }
    }
    else
    {
        cerr << "[FAIL] " << label << ", expected value " << value << ", got error: " << status.get_message() << "\n";
        return false;
    }
}

bool empty_program()
{
    VM vm;
    return EXPECT_ERROR(vm, "Empty program");
}
}

int main(void)
{
    Runner runner;

    runner(empty_program);


    return runner.report();
}
