/**********************************************************************/
/*                 Add object definitions and functions               */
/*               Authors: Davis Polito and Joshua Warner              */
/**********************************************************************/

#include <assert.h>
#include "adder.h"



// Tick function for a _tAdd object.  Returns the sum of adder's two
// fields.
float tAdder_tick (tAdder* adder)
{
    return adder->a + adder->b;
}

// Setter function for the `a` parameter of a _tAdd object
void tAdder_setA (tAdder* adder, float a)
{
    adder->a = a;
}

// Setter function for the `b` parameter of a _tAdd object
void tAdder_setB (tAdder* adder, float b)
{
    adder->b = b;
}

// Initializes the `processor` object to encapsulate the _tAdd adder 
// object and its related setter functions, parameters, and ID
void tAddInitializer(tProcessor* processor, tAdder* adder, int ID) {

    // Checks that arguments are valid
    assert (processor != NULL);
    assert (adder != NULL);

    processor->processorUniqueID = ID;
    processor->object = adder;
    processor->tick = (tTickFunc) &tAdder_tick;
    processor->numSetterFunctions = 2;
    processor->setterFunctions[0] = (tSetter) &tAdder_setA;
    processor->setterFunctions[1] = (tSetter) &tAdder_setB;

}