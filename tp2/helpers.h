#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>

#define RED    "\033[1;31m"
#define GREEN  "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE   "\033[1;34m"
#define RESET  "\033[0m"

void ok();
void error();
char ** tokenize(char*, int*, char*);

//#define TRACE() printf("%s:%s:%d\n", __FILE__, __FUNCTION__, __LINE__)
#define TRACE()

#endif // HELPERS_H
