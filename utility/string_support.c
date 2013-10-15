#include "string_support.h"

int sup_itoa(int val, char* buf, unsigned int radix)
{  
    char* p;
    unsigned int a; //every digit
    int len;
    char* b;        //start of the digit char
    char temp;
    unsigned int u;
    
    p = buf;
    
    if (val < 0)
    {
        *p++ = '-';
        val = 0 - val;
    }
    
    u = (unsigned int)val;
    b = p;
    
    do
    {
        a = u % radix;
        u /= radix;       
        *p++ = a + '0';   
    } while (u > 0);
    
    len = (int)(p - buf); 
    *p-- = 0;
    
    //swap
    do
    {
        temp = *p;
        *p = *b;
        *b = temp;
        --p;
        ++b;
    } while (b < p);
    
    return len;
}

int sup_uitoa(unsigned int val, char* buf, unsigned int radix)
{
    char* p;
    unsigned int a; //every digit
    int len;
    char* b;        //start of the digit char
    char temp;
    
    p = buf;
    b = p;
    
    do
    {
        a = val % radix;
        val /= radix;       
        *p++ = a + '0';   
    } while (val > 0);
    
    len = (int)(p - buf); 
    *p-- = 0;
    
    //swap
    do
    {
        temp = *p;
        *p = *b;
        *b = temp;
        --p;
        ++b;
    } while (b < p);
    
    return len;
}
