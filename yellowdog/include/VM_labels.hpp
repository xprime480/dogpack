#if ! defined(VM_LABELS_HPP)
#define VM_LABELS_HPP 1

#include <string>
#include <vector>

class VM_labels
{
public:
    VM_labels();

    size_t size() const;
    int find(std::string const& name) const;
    int new_label(std::string const & name, int location);
    int add_or_update(std::string const & name, int location);
    int pc_at(int index) const;
    std::string const & name_at(int index) const;

    void dump() const;

private:
    std::vector<std::pair<std::string, int>> labels;
};

#endif