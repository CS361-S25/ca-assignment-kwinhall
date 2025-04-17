[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/StQAS8iq)

By Kendra Winhall 

This program implements a continuous cellular automaton on a toroidal grid, with multiple color options for users to choose from. Color options include grayscale, multicolor, red, orange, green, blue, purple, and pink. Users can switch between color options by pressing the relevant button before the animation begins, while the animation is running, or when the animation is stopped.

The pattern begins with a single cell in the middle of the grid, but turns into an elaborate pattern with both reflectional symmetry and rotational symmetry.

The update rule does the following:
- Take the average value for all neighbor cells in the Moore neighborhood (excluding the cell itself).
- If the average neighbor cell value is zero, set the cell's future value to be zero.
- Otherwise, add 0.75 to the average neighbor cell value and then set the fractional component to be the cell's future value.
- The update rule was inspired by an example from Stephen Wolfram's book A New Kind of Science, although I changed the constant and adapted the rule to work for 2D cellular automata: https://www.wolframscience.com/nks/p158--continuous-cellular-automata/
