# Implementing-Xoodoo-with-protection-against-single-fault-attacks
Code repository of the thesis Implementing Xoodoo with protection against single fault attacks on Cortex-M4

In this repository one can find the implementations and tests mentioned in the thesis.

*implementation_without_optimization contains a basic C version of an implementation of Xoodoo with protection against fault attacks. It does not contain any platform based optimizations.

*implementation_with_representation_of_optimization is an improved implementation that is a representation of how one should manage the registers and swapping on a Cortex-M4

*Tests:
  *The performance folder contains a comparison between the unoptimized C Xoodoo code without protection against fault attacks and the representation of the optimization. The goal of this comparison is to show the cost of the added steps, not accounting for the cost and time of swaps.
  *equivalent is a test that checks if the output of the new implemation of Xoodoo is equal to the output of Xoodoo without protection against fault attacks, when both receive the same input.
  * The unoptimized C Xoodoo code is based on the [XKCP](https://github.com/XKCP/XKCP/blob/master/lib/low/Xoodoo/Optimized/Xoodoo-optimized.c) implementation. Minor adjustments have been made to make it fit with the external functions of the new implementation.
