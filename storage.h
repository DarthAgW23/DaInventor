#ifndef STORAGE_H
#define STORAGE_H

#include "store.h"

/* Write the store in the Appendix A format. Prints OK SAVE or ERR FILE_OPEN. */
void cmd_save(const Store *s, const char *filename);

/* Open the file first; if that fails print ERR FILE_OPEN and change nothing.
   Otherwise free the old data, read the new data, print OK LOAD.
   Returns 1 on success (so main can close all carts), 0 on failure. */
int cmd_load(Store *s, const char *filename);

#endif