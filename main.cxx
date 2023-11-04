// *************************************
// syp hash_map usage.
// Current implementation for numerics.
// Author: Juan Carlos Juarez.
// Contact: jc.juarezgarcia@outlook.com
// *************************************

#include <iostream>
#include "hash_map.hxx"

int main()
{
    syp::hash_map<char, unsigned int> map;

    for (char character = 'a'; character <= 'z'; ++character)
    {
        unsigned int position = (character - 'a') + 1;
        map.insert(character, position);
    }

    for (char character = 'a'; character <= 'z'; ++character)
    {
        std::cout << "* " << character << " <-> " << map.get(character).value() << std::endl;
    }

    std::cout << "* Map size is: " << map.size() << std::endl;

    map.remove('f');

    if (!map.get('f').has_value()) std::cout << "* Map does not contain key 'f'" << std::endl;

    map.clear();

    std::cout << "* Map size is: " << map.size() << std::endl;

    return 0;
}