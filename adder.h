/**********************************************************************/
/*                            Adder header                            */
/*               Authors: Davis Polito and Joshua Warner              */
/**********************************************************************/

#include "processor.h"

#ifndef ADDER_HEADER
#define ADDER_HEADER

// An `Add` object is an object which stores two addends of type float.
// Defines a struct Add object, and aliases it with `tAdd`.
typedef struct Add
{
    float a; // addend 1
    float b; // addend 2
    float out;
} tAdder;

// Tick function for a `tAdd` object.  Returns the sum of adder's two
// addends.
void tAdder_tick (tAdder* adder);

// Setter function for a `tAdd` object.  Sets the adder's first addend to
// the value of `a`.
void tAdder_setA (tAdder* adder, float a);

// Setter function for a `tAdd` object.  Sets the adder's second addend to
// the value of `b`.
void tAdder_setB (tAdder* adder, float b);

void tAddInitializer(tProcessor* processor, tAdder* adder, int ID);

#endif