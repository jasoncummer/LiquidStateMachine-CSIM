#ifndef __CSIMERROR_H__
#define __CSIMERROR_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

//! Class for managing formated error messages
class csimError {
 public:
  csimError(void) {
    lMessage=128;
    message=(char *)malloc(lMessage*sizeof(char));
    // set the message to the NULL string
    *message = 0;
    // add some starting part
    clear();
  }

  ~csimError() { 
    if (message) free(message);
  }

  //! Add new formated message (formating is like for printf)
  void add(const char *fmt, ...);

  //! Clear all messages
  void clear(void) { 
    message[0]=0;
    add("\n\nCSIM-Error:\n");
    hasMsg = 0;
  }

  //! Get current message
  char *msg(void) { return message; }

  //! Returns TRUE if message is not empty
  bool hasErrorMsg(void) { return hasMsg; }
 private:
  char *message;
  int lMessage;
  bool hasMsg;
};

extern csimError TheCsimError;

#endif
