#pragma once

// Persisted parameters of the last generated airfoil, kept as globals for
// compatibility with the original design (they are set by each generator
// dialog when "Generate" is pressed). Not read anywhere else internally,
// but kept available in case other parts of the app need the last-used
// values.
extern int   M, P4, SS;               // NACA 4
extern int   L, P5, Q, XX;            // NACA 5
extern float N1, N2;                  // CST
extern float A1U, A1L, A2U, A2L;      // CST
extern float A3U, A3L, A4U, A4L;      // CST
extern float AoA, Mach, Re;           // Solution
