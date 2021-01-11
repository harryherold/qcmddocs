#include "note_tree.hpp"
#include <iostream>
#include <memory>
#include <string>

int
main()
{
    auto string_tree = NoteTree<std::string>::create(std::string("Hans"));
    string_tree->appendChild(std::string("Bert"));
    string_tree->appendChild(std::string("Tom"));

    for (int i = 0; i < string_tree->childCount(); i++)
    {
        std::cout << string_tree->child(i)->value() << "\n";
    }

    return 0;
}