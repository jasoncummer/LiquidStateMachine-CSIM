#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "csimerror.h"

csimError TheCsimError;

void csimError::add(const char* fmt, ...)
{
  va_list ap;
  int nChars = strlen(message);
  hasMsg = 1;
  while (1) {
    /* Try to print in the allocated space. */
    va_start(ap, fmt);
#ifdef _GNU_SOURCE
    int n = vsnprintf (message+nChars, lMessage-nChars-1, fmt, ap);
#else
    int n = vsprintf (message+nChars, fmt, ap);
#endif
    va_end(ap);
    /* If that worked, return else alloc more memory */
    if (n > -1 && n < lMessage-nChars-1 ) {
      hasMsg = 1;
      return;
    } else {
      lMessage *= 2;
      message = (char *)realloc(message,lMessage*sizeof(char));
    }
  }
}
