#ifndef _STRING_SUPPORT_H_
#define _STRING_SUPPORT_H_

#if defined(__cplusplus)
extern "C" {
#endif

int sup_itoa(int val, char* buf, unsigned int radix);
int sup_uitoa(unsigned int val, char* buf, unsigned int radix);

#if defined(__cplusplus)
}
#endif

#endif