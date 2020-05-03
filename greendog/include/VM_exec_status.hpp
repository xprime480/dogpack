#if ! defined(VM_EXEC_STATUS_HPP)
#define VM_EXEC_STATUS_HPP 1

#include <string>

class VM_exec_status
{
public:
    explicit VM_exec_status(int value);
    explicit VM_exec_status(std::string const & msg);

    bool is_status_ok() const;
    int get_program_value() const;
    const std::string &get_message() const;

private:
    const bool is_ok;
    const int value;
    const std::string msg;
};

#endif