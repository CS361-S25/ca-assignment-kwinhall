By Kendra Winhall 

This program implements a continuous cellular automaton on a toroidal grid, with multiple color options for users to choose from. Users can switch between color options by pressing the relevant button before the animation begins, while the animation is running, or when the animation is stopped.

The pattern begins with a single cell in the middle of the grid, but turns into an elaborate pattern with both reflectional symmetry and rotational symmetry.

The update rule does the following:
- Take the average value for all neighbor cells in the Moore neighborhood (excluding the cell itself).
- If the average neighbor cell value is zero, set the cell's future value to be zero.
- Otherwise, add 0.75 to the average neighbor cell value and then set the fractional component to be the cell's future value.
- The update rule was inspired by an example from Stephen Wolfram's book A New Kind of Science, although I changed the constant and adapted the rule to work for 2D cellular automata: https://www.wolframscience.com/nks/p158--continuous-cellular-automata/
