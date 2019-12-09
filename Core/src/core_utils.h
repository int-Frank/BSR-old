#ifndef CORE_H
#define CORE_H

#include <memory>

#define BIT(x) (1 << x)

//--- Types ------------------------------------------------------------------

// 2 steps to initialise: 1) define a new NEW_COUNTER name (this is just used internally)
//                        2) Initialise the counter
//example:
// #undef NEW_COUNTER
// #define NEW_COUNTER MyCounter
// INIT_COUNTER(1)
// ..will start a new counter, beginning at 1
// int val[3] = {COUNTER, COUNTER, COUNTER} // = {1, 2, 3}
#define CONCAT2(a, b) a##b
#define CONCAT(a, b) CONCAT2(a, b)
#define NEW_COUNTER TEMP
#define INIT_COUNTER(val) namespace CONCAT(IMPL_COUNTER_, NEW_COUNTER) {int const COUNTER_BASE = __COUNTER__ - val + 1;}
#define COUNTER (__COUNTER__ - CONCAT(IMPL_COUNTER_, NEW_COUNTER)::COUNTER_BASE)

#endif