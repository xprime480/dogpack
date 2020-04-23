#include "VM_labels.hpp"

#include <iostream>

using namespace std;

VM_labels::VM_labels()
{
    labels.reserve(20);
}

size_t VM_labels::size() const
{
    return labels.size();
}

int VM_labels::find(std::string const &name) const
{
    int sz = (int)size();
    for (int i = 0; i < sz; ++i)
    {
        if (name == labels[i].first)
        {
            return i;
        }
    }

    return -1;
}

int VM_labels::new_label(std::string const &name, int location)
{
    //cerr << "new_label: " << name << " " << location << "\n";
    if (find(name) > 0)
    {
        //cerr << "\talready exists\n";
        return -1;
    }
    return add_or_update(name, location);
}

int VM_labels::add_or_update(std::string const &name, int location)
{
    // cerr << "add_or_update: " << name << " " << location << "\n";
    int index = find(name);
    if (index < 0)
    {
        index = size();
        labels.push_back(pair<string, int>(name, location));
        // cerr << "\tbrand new at index " << index << "\n";
        return index;
    }

    if (labels[index].second < 0 && location >= 0)
    {
        // cerr << "\tupdated\n";
        labels[index].second = location;
        return index;
    }
    else if (labels[index].second >= 0 && location >= 0)
    {
        // cerr << "\tduplicate entry; returning index -1\n";
        return -1;
    }

    return index;
}

int VM_labels::pc_at(int index) const
{
    if (index < (int)size())
    {
        return labels[index].second;
    }
    return -1;
}

std::string const &VM_labels::name_at(int index) const
{

    if (index >= 0 && index < (int)size())
    {
        return labels[index].first;
    }
    static const string empty("");
    return empty;
}

void VM_labels::dump() const
{
    cerr << "Labels [" << size() << "]\n";
    for (size_t i = 0; i < size(); ++i)
    {
        cerr << "\t" << labels[i].first << ": " << labels[i].second << "\n";
    }
}

