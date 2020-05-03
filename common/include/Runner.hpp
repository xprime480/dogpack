#if !defined(RUNNER_HPP)
#define RUNNER_HPP 1

#include <functional>
#include <string>

#include "VM_exec_status.hpp"

class Runner
{
public:
    Runner();

    void operator()(std::function<bool()> fn);

    int report() const;

private:
    size_t count;
    size_t passed;
};

extern bool expect_error_helper(std::string const & label, bool verbose, VM_exec_status status);

template <typename VM>
bool EXPECT_ERROR(VM &vm, std::string const &label, bool verbose = false)
{
    VM_exec_status status = vm.exec(verbose);
    return expect_error_helper(label, verbose, status);
}

extern bool expect_value_helper(std::string const &label, int value, bool verbose, VM_exec_status status);

template <typename VM>
bool EXPECT_VALUE(VM &vm, std::string const &label, int value, bool verbose = false)
{
    VM_exec_status status = vm.exec(verbose);
    return expect_value_helper(label, value, verbose, status);
}

extern bool expect_run_ok_helper(std::string const &label, std::function<bool(bool)> fn, bool verbose, VM_exec_status status);

template <typename VM>
bool EXPECT_RUN_OK(VM &vm, std::string const &label, std::function<bool(bool)> fn, bool verbose = false)
{
    VM_exec_status status = vm.exec(verbose);
    return expect_run_ok_helper(label, fn, verbose, status);
}



#endif
