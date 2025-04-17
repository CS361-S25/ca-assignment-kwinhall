/*
Kendra Winhall 
Cellular Automata Assignment, CS 361
Adapted from starter code from Anya Vostinar

This program implements a continuous cellular automaton on a toroidal grid, with multiple color options.
*/

#include "emp/web/Animate.hpp"
#include "emp/web/web.hpp"
#include "emp/web/Button.hpp"

emp::web::Document doc{"target"};

class CAAnimator : public emp::web::Animate {
    bool isGrayscale = true;
    bool isMulticolor = false;

    // sets cell size and grid dimensions
    const int cellLength = 4; // cell length in pixels
    const int numRowBoxes = 130; 
    const int numColBoxes = 130; 
    const double gridHeight = numRowBoxes * cellLength; 
    const double gridWidth = numColBoxes * cellLength; 
    int hue = 0;

    // creates vectors to hold current and future cell states, ranging from 0.0 (white) to 1.0 (color)
    std::vector<std::vector<double>> cells;
    std::vector<std::vector<double>> futureCells;
            
    emp::web::Canvas canvas{gridWidth, gridHeight, "canvas"};

    public:

        CAAnimator() {
            // puts canvas and control buttons into the div
            doc << canvas;
            std::vector<emp::web::Button> buttons;
            buttons.push_back(GetToggleButton("Toggle"));
            buttons.push_back(GetStepButton("Step"));
            buttons.push_back(emp::web::Button(MakeGrayscale, "Grayscale"));
            buttons.push_back(emp::web::Button(MakeMulticolor, "Multicolor"));
            buttons.push_back(emp::web::Button(MakeRed, "Red"));
            buttons.push_back(emp::web::Button(MakeOrange, "Orange"));
            buttons.push_back(emp::web::Button(MakeGreen, "Green"));
            buttons.push_back(emp::web::Button(MakeBlue, "Blue"));
            buttons.push_back(emp::web::Button(MakePurple, "Purple"));
            buttons.push_back(emp::web::Button(MakePink, "Pink"));

            for (emp::web::Button button : buttons) {
                StyleButton(button.GetLabel(), button);
                doc << button;
            }

            // sets all cells to white (0.0) to start
            cells.resize(numColBoxes, std::vector<double>(numRowBoxes, 0.0));
            futureCells.resize(numColBoxes, std::vector<double>(numRowBoxes, 0.0));

            // sets the center cell to color (1.0)
            cells[numColBoxes/2][numRowBoxes/2] = 1.0;
        }

        // Creates buttons
        const std::function<void()> MakeGrayscale = [this]() {isGrayscale = true; isMulticolor = false;};
        const std::function<void()> MakeMulticolor = [this]() {isGrayscale = false; isMulticolor = true;};
        const std::function<void()> MakeRed = [this]() {isGrayscale = false; isMulticolor = false; hue = 0;};
        const std::function<void()> MakeOrange = [this]() {isGrayscale = false; isMulticolor = false; hue = 30;};
        const std::function<void()> MakeGreen = [this]() {isGrayscale = false; isMulticolor = false; hue = 120;};
        const std::function<void()> MakeBlue = [this]() {isGrayscale = false; isMulticolor = false; hue = 240;};
        const std::function<void()> MakePurple = [this]() {isGrayscale = false; isMulticolor = false; hue = 270;};
        const std::function<void()> MakePink = [this]() {isGrayscale = false; isMulticolor = false; hue = 300;};

        // Styles buttons
        void StyleButton(std::string buttonName, emp::web::Button button) {
            button.SetFontSize(12).SetCSS("margin", "4px 2px").SetCSS("border-radius", "8px").SetCSS("padding", "10px 10px");
            if (buttonName == "Red") {button.SetBackground("#ff0000").SetBorder("#ff0000").SetColor("white");}
            else if (buttonName == "Orange") {button.SetBackground("#ff8000").SetBorder("#ff8000");}
            else if (buttonName == "Green") {button.SetBackground("#00ff00").SetBorder("#00ff00");}
            else if (buttonName == "Blue") {button.SetBackground("#0000ff").SetBorder("#0000ff").SetColor("white");}
            else if (buttonName == "Purple") {button.SetBackground("#8000ff").SetBorder("#8000ff").SetColor("white");;}
            else if (buttonName == "Pink") {button.SetBackground("#ff00ff").SetBorder("#ff00ff");}
        }

        // Given a cell's coordinates, returns the average value of its neighbor cells in its toroidal Moore neighborhood
        double AvgNeighborValue(int x, int y) {
            double neighborValue = 0.0;
            // finds current cell's toroidal Moore neighborhood
            for (int a = -1; a < 2; a++) {
                for (int b = -1; b < 2; b++) {
                    int neighborX = emp::Mod(a+x, numColBoxes);
                    int neighborY = emp::Mod(b+y, numRowBoxes);
                    if (a != 0 or b != 0) { // excludes cells[x][y], only considers its neighbors
                        neighborValue += cells[neighborX][neighborY];
                    }
                }
            }
            return neighborValue/8; // cell has 8 neighbors
        }

        // Updates the cellular automaton for each frame
        void DoFrame() override {
            canvas.Clear();
            std::string color;
            // sets colors based on button click
            for (int x = 0; x < numColBoxes; x++) {
                for (int y = 0; y < numRowBoxes; y++) {
                    if (isMulticolor) {
                        color = emp::ColorHSV(300*(1 - cells[x][y]), 1, 1);
                    }
                    else if (isGrayscale) {
                        color = emp::ColorHSV(0, 0, 1 - cells[x][y]);
                    }
                    else {
                        color = emp::ColorHSV(hue, cells[x][y], 1); 
                    }
                    canvas.Rect(x*cellLength, y*cellLength, cellLength, cellLength, color, color);

                    double avgNeighbors =  AvgNeighborValue(x, y);
                    if (avgNeighbors == 0.0) { // if all neighbor cells are dead (white), the cell should stay dead
                        futureCells[x][y] = 0.0;
                    }
                    else {
                        // update rule inspired by https://www.wolframscience.com/nks/p158--continuous-cellular-automata/
                        double futureValue =  AvgNeighborValue(x, y) + .75;
                        futureValue -= (int)futureValue;
                        futureCells[x][y] = futureValue;
                    }
                }
            }
            cells = futureCells;
        }
};

CAAnimator animator;

int main() {
    animator.Step();
}