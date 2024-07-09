/**********************************************************************/
/*                       Macros and TypeDefs                          */
/*              Authors: Davis Polito and Joshua Warner               */
/**********************************************************************/

#ifndef DEFS_HEADER
#define DEFS_HEADER

// Maximum number of parameters for a processor object
#define MAX_NUM_PARAMS 16

// Maximum number of processor sources for a mapping object
#define MAX_NUM_SOURCES 3

// Defines 'pTickFunction' to be the name of a new function pointer type
typedef void (*tTickFuncReturningVoid)(void*); // used for processors (don't return audio because that will be stored in the modules' output array

typedef float (*tTickFuncReturningFloat)(void*); //used inside modules to wrap internal tick functions as function pointers

// Defines `_tParameter` to be the name of a new function pointer type
typedef void (*tSetter)(void*, float);

typedef enum {
    ModuleTypeMultiOscModule,
    ModuleTypeLFOModule,
    ModuleTypeOscModule

} ModuleType;

#endif
