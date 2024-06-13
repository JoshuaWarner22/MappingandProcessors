/**********************************************************************/
/*                       Macros and TypeDefs                          */
/*              Authors: Davis Polito and Joshua Warner               */
/**********************************************************************/

#ifndef DEFS_HEADER
#define DEFS_HEADER

// Maximum number of parameters for a processor object
#define MAX_NUM_PARAMS 8

// Maximum number of processor sources for a mapping object
#define MAX_NUM_SOURCES 3

// Defines 'pTickFunction' to be the name of a new function pointer type
typedef float (*tTickFunc)(void*);

// Defines `_tParameter` to be the name of a new function pointer type
typedef void (*tSetter)(void*, float);

typedef enum {
    ModuleTypeMultiOscModule

} ModuleType;

#endif
