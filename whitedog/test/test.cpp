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

void trivial_program_test()
{
    std::cout << "\n\tWe expect to see 1 for an answer if the program is just PUSH 1.\n";

    VM vm;
    vm.push(1);
    vm.exec();
}

} // namespace

int main(void)
{
    empty_program_test();
    trivial_program_test();
}