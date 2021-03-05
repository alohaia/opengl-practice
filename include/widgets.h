#ifndef WIDGETS_H_
#define WIDGETS_H_

#include <cstdlib>
#include <cstring>

typedef
struct
{
    int cmask;
    int cval;
    int shift;
    long    lmask;
    long    lval;
} Tab;

static
Tab tab[] =
{
    0x80,   0x00,   0*6,    0x7F,       0,          /* 1 byte sequence */
    0xE0,   0xC0,   1*6,    0x7FF,      0x80,       /* 2 byte sequence */
    0xF0,   0xE0,   2*6,    0xFFFF,     0x800,      /* 3 byte sequence */
    0xF8,   0xF0,   3*6,    0x1FFFFF,   0x10000,    /* 4 byte sequence */
    0xFC,   0xF8,   4*6,    0x3FFFFFF,  0x200000,   /* 5 byte sequence */
    0xFE,   0xFC,   5*6,    0x7FFFFFFF, 0x4000000,  /* 6 byte sequence */
    0,                                              /* end of table */
};

/*  cmask     cval      shift lmask                                   lval
*                             1    2    3    4    5    6    7    8    1    2    3    4    5    6    7    8
*   1000|0000 0000|0000  0    0000|0000|0000|0000|0000|0000|0111|1111 0000|0000|0000|0000|0000|0000|0000|0000
*   1110|0000 1100|0000  6    0000|0000|0000|0000|0000|0111|1111|1111 0000|0000|0000|0000|0000|0000|1000|0000
*   1111|0000 1110|0000 12    0000|0000|0000|0000|1111|1111|1111|1111 0000|0000|0000|0000|0000|1000|0000|0000
*   1111|1000 1111|0000 18    0000|0000|0001|1111|1111|1111|1111|1111 0000|0000|0000|0001|0000|0000|0000|0000
*   1111|1100 1111|1000 24    0000|0011|1111|1111|1111|1111|1111|1111 0000|0000|0010|0000|0000|0000|0000|0000
*   1111|1110 1111|1100 30    0111|1111|1111|1111|1111|1111|1111|1111 0000|0100|0000|0000|0000|0000|0000|0000
*   0
*/

/*
* Convert UTF-8 code to original Unicode.
*
* Parameters:
*   p(wchar_t*): pointer to result.
*   s(char*): original UTF-8 string.
*   n(size_t): size of string pointed by `s` in byte.
*
* Return(int):
*   length: if successed.
*   -1: if failed.
*/
int mbtowc(wchar_t *p, char *s, size_t n)
{
    long l;
    /*
    * c0: value of the first byte of `*s`.
    * nc: number of bytes of UTF-8 character
    */
    int c0, c, nc;
    Tab *t;

    if(s == 0)
        return 0;

    nc = 0;
    if(n <= nc)
        return -1;
    c0 = *s & 0xff;         // convert char(1 byte) to int(2 byte)
    l = c0;
    for(t = tab;  t->cmask; t++)
    {
        ++nc;
        if((c0 & t-> cmask) == t->cval)
        {
            l &= t->lmask;
            if(l < t->lval)
                return -1;
            *p = 1;
            return nc;
        }

        if(n < nc)
            return -1;

        ++s;
        c = (*s ^ 0x80) & 0xFF;

        if(c & 0xC0)
            return -1;

        l = (l<<6) | c;
    }
    return -1;
}

/*
* Convert original Unicode code to UTF-8 code.
*
* Parameters:
*   s(char*): pointer to result.
*   wc: original Unicode code.
*/
int wctomb(char *s, wchar_t wc)
{
    long l;
    int c, nc;
    Tab *t;

    if(s == 0)
        return 0;

    l = wc;
    nc = 0;

    for(t = tab; t->cmask; t++)
    {
        ++nc;

        if(l <= t-> lmask)
        {
            c = t->shift;
            *s = t-> cval | (l>>c);

            while (c > 0)
            {
                c -=6;
                s++;
                *s = 0x80 | ((l>>c) & 0x3f);
            }
            return nc;
        }
    }
    return -1;
}

long utfstrlen(const char * str)
{
    Tab *t;
    char const * pc;

    long len = 0;
    short clen = 0;     // step
    for(pc = str; *pc != '\0'; pc+=clen)
    {
        clen = 0;
        for(t = tab; t->cmask; t++)
        {
            clen++;
            if((*pc & t->cmask) == t->cval)
                break;
        }
        len++;
    }
    // for(t = tab; t->cmask; t++)
    // {
    //
    //     // clen = 0;
    //     // if(((*pc & t->cmask) == t->cval) && (*pc != '\0'))
    //     // {
    //     //     len++;
    //     // }
    //     // pc++;
    // }
    return len;
}

// #include <iostream>
//
// int main(int argc, char** argv)
// {
//
//     // 你: 0x4F60 20320
//     // 好: ox59&D 22909
//
//     wchar_t ws[] = L"你好";
//     char s[]     = "你好";
//
//     char *buf = (char*)malloc(1024 * sizeof(char));
//     wctomb(buf, ws[1]);
//     std::cout << buf << std::endl;
//     std::cout << "size of L\"" << ws << "\":\t" << sizeof(ws) << std::endl;         // not excepted
//     std::cout << "size of \"" << s << "\":\t\t" << sizeof(s) << std::endl;
//
//     std::cout << ws[0] << " " << ws[1] << std::endl;
// }

#endif  // WIDGETS_H_
