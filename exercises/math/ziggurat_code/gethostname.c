#include <stdlib.h>
#include <stdio.h>

/* dummy to provide missing function when linking to testu01 */
int gethostname(char *name, size_t namelen)
{
	return -1;
}

