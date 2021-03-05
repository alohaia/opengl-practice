#include "include/widgets.h"
#include <iostream>

int main(int argc, char** argv)
{
    std::cout << utfstrlen("hello 世界！") << std::endl;
    std::cout << strlen("hello") << std::endl;
}
