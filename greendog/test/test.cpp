
#include <functional>
#include <iostream>
#include <string>

#include "vm.hpp"
#include "Runner.hpp"

using namespace std;

bool empty_program()
{
    VM vm;
    return EXPECT_RUN_OK(vm, "Empty program", [](bool) -> bool { return true; });
}

bool load_bad_register()
{
    VM vm;
    vm.load(32, 0);
    return EXPECT_ERROR(vm, "Load Bad Register");
}

bool load_bad_address()
{
    VM vm;
    vm.load(31, 8192);
    return EXPECT_ERROR(vm, "Load Bad Address");
}

bool store_bad_register()
{
    VM vm;
    vm.store(32, 0);
    return EXPECT_ERROR(vm, "Store Bad Register");
}

bool store_bad_address()
{
    VM vm;
    vm.store(31, 8192);
    return EXPECT_ERROR(vm, "Store Bad Address");
}

bool load_store_ok()
{
    VM vm;
    vm.load(31, 0);
    vm.store(31, 1);

    vm.set_heap(0, 42);
    return EXPECT_RUN_OK(vm, "Load Store OK", [&vm](bool verbose) -> bool { 
        int act = vm.get_heap(1);
        if ( verbose )
        {
            if (42 != act)
            {
                cerr << "Expected 42, got " << act << "\n";
            }
        }
        return 42 == act; 
    });
}

void load_suite(Runner &runner)
{
    runner(load_bad_register);
    runner(load_bad_address);
    runner(store_bad_register);
    runner(store_bad_address);
    runner(load_store_ok);
}

int main(void)
{
    Runner runner;

    runner(empty_program);

    load_suite(runner);

    return runner.report();
}
