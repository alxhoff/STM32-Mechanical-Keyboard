#ifndef __BUFFER_RESOLVER_H__
#define __BUFFER_RESOLVER_H__ 

#include <stdint.h>

int8_t buffer_resolverKeyboardToUSB(void);
int8_t buffer_resolverKeyboardToCLI(void);
int8_t buffer_resolverKeyboardToMacro(void);
int8_t buffer_resolverInit(void);

#endif // __BUFFER_RESOLVER_H__
