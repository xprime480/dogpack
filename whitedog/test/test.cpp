#include <iostream>

#include "../include/vm.hpp"

namespace
{
void empty_program_test()
{
    std::cout << "\n\tWe expect an error message if we execute an empty program.\n";

    VM vm;
    vm.exec();
}
} // namespace

int main(void)
{
    empty_program_test();
}