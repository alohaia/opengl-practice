#include <stdio.h>

int main(int argc, char** argv)
{
    // get controlling terminal name
    printf("%s", ctermid(NULL));
}
