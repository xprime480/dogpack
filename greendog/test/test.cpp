
#include <functional>
#include <iostream>
#include <string>

#include "vm.hpp"
#include "Runner.hpp"

using namespace std;

bool empty_program()
{
    VM vm;
    return EXPECT_ERROR(vm, "Empty program");
}

int main(void)
{
    Runner runner;

    runner(empty_program);


    return runner.report();
}
