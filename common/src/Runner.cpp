#include "Runner.hpp"

#include <iostream>

using namespace std;

Runner::Runner()
    : count(0u), passed(0u)
{
}

void Runner::operator()(function<bool()> fn)
{
    ++count;
    if (fn())
    {
        ++passed;
    }
}

int Runner::report() const
{
    cout << "\n"
         << passed << " passed out of " << count << " tests\n";
    return passed == count ? 0 : 1;
}

bool expect_error_helper(string const &label, bool verbose, VM_exec_status status)
{
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

bool expect_value_helper(string const &label, int value, bool verbose, VM_exec_status status)
{
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

bool expect_run_ok_helper(std::string const &label, std::function<bool(bool)> fn, bool verbose, VM_exec_status status)
{
    if (status.is_status_ok())
    {
        if (fn(verbose))
        {
            cerr << "[PASS] " << label << "\n";
            return true;
        }
        else
        {
            cerr << "[FAIL] " << label << ", failed additional validations\n";
            return false;
        }
    }
    else
    {
        cerr << "[FAIL] " << label << ", expected normal termination, got error: " << status.get_message() << "\n";
        return false;
    }
}


