[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/StQAS8iq)

By Kendra Winhall 

This program implements a continuous cellular automaton on a toroidal grid, with multiple color options.

The update rule does the following:
- Take the average value for all neighbor cells in the Moore neighborhood (excluding the cell itself)
- If the average value is zero, the cell's future value will be zero
- Otherwise, add 0.75 to the average neighbor cell value and then remove the integer component. This will be the cell's future value.
- The update rule was inspired by https://www.wolframscience.com/nks/p158--continuous-cellular-automata/
