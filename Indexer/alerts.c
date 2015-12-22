#include "alerts.h"
#include <stdio.h>

#define KNRM "\x1B[0m"
#define KRED "\x1B[41m"
#define KGRN "\x1B[42m"
#define KYEL "\x1B[43m"

/**
 * Allows the user to print a colored warning message.
 * @param msg - Message that the user would like to print.
 */
void warning(char * msg)
{
	printf("%sWARNING: %s%s\n", KYEL, msg, KNRM);
	return;  
}

/**
 * Allows the user to print a colored error message.
 * @param msg - Message that the user would like to print.
 */
void error(char * msg)
{
	printf("%sERROR: %s%s\n", KRED, msg, KNRM);
	return;
}

/**
 * Allows the user to print a colored success message.
 * @param msg - Message that the user would like to print.
 */
void success(char * msg)
{
	printf("%sSUCCESS: %s%s\n", KGRN, msg, KNRM);
	return;
}

/**
 * Prints generic colored warning message to the screen.
 */
void genwarn()
{
	printf("%s WARNING: One or more things might not be correct. Misbehavior may occur. %s\n", KYEL, KNRM);
	return;
}

/**
 * Prints generic colored error message to the screen.
 */
void generr()
{
	printf("%s ERROR: Error has occurred during run time. %s\n", KRED, KNRM);
	return;
}

/**
 * Prints generic colored success message to the screen.
 */
void gensucc()
{
	printf("%s SUCCESS: Run was successful.%s\n", KGRN, KNRM);
	return;
}
