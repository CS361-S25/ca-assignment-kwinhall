/**
 * Kendra Winhall
 * Cellular Automata Assignment, CS 361
 * Adapted from starter code from Anya Vostinar
 * 
 * Implements a continuous cellular automaton on a toroidal grid, with multiple color options for users to choose from.
 * Run with ./compile-run.sh and view web interface at http://localhost:8000/ (Chrome or Safari recommended).
 */


#include "emp/web/Animate.hpp"
#include "emp/web/Button.hpp"
#include "emp/web/web.hpp"
#include <string>


emp::web::Document doc{"target"};


/**
 * Manages the animation of a continuous cellular automaton.
 */
class CAAnimator : public emp::web::Animate {

    // sets cell size and grid dimensions
    const int cellLength = 3; // cell length in pixels
    const int numRowBoxes = 170; 
    const int numColBoxes = 170; 
    const double gridHeight = numRowBoxes * cellLength; 
    const double gridWidth = numColBoxes * cellLength; 
    emp::web::Canvas canvas{gridWidth, gridHeight, "canvas"};

    // determines colors of cellular automaton, which the user changes with buttons on the web interface
    bool isGrayscale = true; // default is grayscale
    bool isRedToPurple = false;
    bool isRedToYellow = false;
    bool isPurpleToCyan = false;
    int hue = 0;

    // these store the cell states in the current frame and the future frame, ranging from 0.0 ("dead") to 1.0 ("alive")
    std::vector<std::vector<double>> cells;
    std::vector<std::vector<double>> futureCells;

    public:

        /**
         * Creates web interface and initializes cell values.
         */
        CAAnimator() {
            // adds canvas and buttons to web interface
            doc << "<div style \"display: flex\">";
            doc << "<div>" << canvas.SetCSS("margin", "8px").SetCSS("float", "left") << "</div>";
            MakeButtons();
            doc << "</div>";

            // sets all cells to "dead" (0.0) to start, except the center cell is fully "alive" (1.0)
            cells.resize(numColBoxes, std::vector<double>(numRowBoxes, 0.0));
            futureCells.resize(numColBoxes, std::vector<double>(numRowBoxes, 0.0));
            cells[numColBoxes/2][numRowBoxes/2] = 1.0;
            futureCells[numColBoxes/2][numRowBoxes/2] = 1.0;
        }


        /**
         * Creates toggle, step, and cellular automaton color buttons for the web interface.
         */
        void MakeButtons() {
            std::vector<emp::web::Button> buttons;
            buttons.push_back(GetToggleButton("Toggle"));
            buttons.push_back(GetStepButton("Step"));

            // for each color button, specifies flags and hue and re-draws cellular automaton with the new color
            buttons.push_back(emp::web::Button([this]() {isGrayscale = true; isRedToPurple = false; isRedToYellow = false; isPurpleToCyan = false; UpdateCA(false);}, "Grayscale"));
            buttons.push_back(emp::web::Button([this]() {isGrayscale = false; isRedToPurple = true; isRedToYellow = false; isPurpleToCyan = false; UpdateCA(false);}, "Red to Purple"));
            buttons.push_back(emp::web::Button([this]() {isGrayscale = false; isRedToPurple = false; isRedToYellow = true; isPurpleToCyan = false; UpdateCA(false);}, "Red to Yellow"));
            buttons.push_back(emp::web::Button([this]() {isGrayscale = false; isRedToPurple = false; isRedToYellow = false; isPurpleToCyan = true; UpdateCA(false);}, "Purple to Cyan"));
            buttons.push_back(emp::web::Button([this]() {isGrayscale = false; isRedToPurple = false; isRedToYellow = false; isPurpleToCyan = false; hue = 0; UpdateCA(false);}, "Red"));
            buttons.push_back(emp::web::Button([this]() {isGrayscale = false; isRedToPurple = false; isRedToYellow = false; isPurpleToCyan = false; hue = 30; UpdateCA(false);}, "Orange"));
            buttons.push_back(emp::web::Button([this]() {isGrayscale = false; isRedToPurple = false; isRedToYellow = false; isPurpleToCyan = false; hue = 120; UpdateCA(false);}, "Green"));
            buttons.push_back(emp::web::Button([this]() {isGrayscale = false; isRedToPurple = false; isRedToYellow = false; isPurpleToCyan = false; hue = 240; UpdateCA(false);}, "Blue"));
            buttons.push_back(emp::web::Button([this]() {isGrayscale = false; isRedToPurple = false; isRedToYellow = false; isPurpleToCyan = false; hue = 270; UpdateCA(false);}, "Purple"));
            buttons.push_back(emp::web::Button([this]() {isGrayscale = false; isRedToPurple = false; isRedToYellow = false; isPurpleToCyan = false; hue = 300; UpdateCA(false);}, "Pink"));

            // styles all buttons and adds them to the web interface
            doc << "<div>";
            for (emp::web::Button button : buttons) {
                StyleButton(button);
                doc << button;
                doc << "<br>";
            }
            doc << "</div>";
        }


        /**
        * Styles a given button for the web interface.
        * @param button The button (which must be initialized previously) to style.
        */
        void StyleButton(emp::web::Button button) {
            std::string buttonName = button.GetLabel();
            // sets standard style for all buttons
            button.SetFontSize(14).SetCSS("margin", "3px 3px").SetCSS("padding", "8px 8px").SetCSS("border-style", "solid");

            // sets button backgrounds and borders
            if (buttonName == "Start"){button.SetCSS("margin-top", "8px").SetBackground("#b3b3b3").SetBorder("#b3b3b3").SetColor("#000000");}
            else if (buttonName == "Step"){button.SetBackground("#b3b3b3").SetBorder("#b3b3b3").SetColor("#000000");}
            else if (buttonName == "Grayscale"){button.SetCSS("background-image", "linear-gradient(to right, #000000, #cccccc)").SetCSS("border-image", "linear-gradient(to right, #000000, #cccccc) 1").SetColor("white");}
            else if (buttonName == "Red to Purple"){button.SetCSS("background-image", "linear-gradient(to right in hsl longer hue, #ff0000, #8000ff)").SetCSS("border-image", "linear-gradient(to right in hsl longer hue, #ff0000, #8000ff) 1");}
            else if (buttonName == "Red to Yellow"){button.SetCSS("background-image", "linear-gradient(to right, #ff0000, #ffff00)").SetCSS("border-image", "linear-gradient(to right, #ff0000, #ffff00) 1");}
            else if (buttonName == "Purple to Cyan"){button.SetCSS("background-image", "linear-gradient(to right, #8000ff, #00ffd5)").SetCSS("border-image", "linear-gradient(to right, #8000ff, #00ffd5) 1").SetColor("white");}
            else if (buttonName == "Red") {button.SetCSS("background-image", "linear-gradient(to right, #ff0000, #ffb3b3)").SetCSS("border-image", "linear-gradient(to right, #ff0000, #ffb3b3) 1");}
            else if (buttonName == "Orange") {button.SetCSS("background-image", "linear-gradient(to right, #ff8000, #ffd9b3)").SetCSS("border-image", "linear-gradient(to right, #ff8000, #ffd9b3) 1");}
            else if (buttonName == "Green") {button.SetCSS("background-image", "linear-gradient(to right, #00ff00, #b3ffb3)").SetCSS("border-image", "linear-gradient(to right, #00ff00, #b3ffb3) 1");}
            else if (buttonName == "Blue") {button.SetCSS("background-image", "linear-gradient(to right, #0000ff, #b3b3ff)").SetCSS("border-image", "linear-gradient(to right, #0000ff, #b3b3ff) 1").SetColor("white");}
            else if (buttonName == "Purple") {button.SetCSS("background-image", "linear-gradient(to right, #8000ff, #d9b3ff)").SetCSS("border-image", "linear-gradient(to right, #8000ff, #d9b3ff) 1").SetColor("white");}
            else if (buttonName == "Pink") {button.SetCSS("background-image", "linear-gradient(to right, #ff00ff, #ffb3ff)").SetCSS("border-image", "linear-gradient(to right, #ff00ff, #ffb3ff) 1");}
        }


        /**
         * Returns the average value of a given cell's neighbors in its toroidal Moore neighborhood.
         * @param x Cell's x coordinate.
         * @param y Cell's y coordinate.
         * @return The average value of the cell's neighbors in its toroidal Moore neighborhood.
         */
        double AvgNeighborValue(int x, int y) {
            double neighborValueSum = 0.0;
            // finds current cell's toroidal Moore neighborhood
            for (int a = -1; a < 2; a++) {
                for (int b = -1; b < 2; b++) {
                    int neighborX = emp::Mod(a+x, numColBoxes);
                    int neighborY = emp::Mod(b+y, numRowBoxes);
                    if (a != 0 or b != 0) { // excludes the given cell, only considers its neighbors
                        neighborValueSum += cells[neighborX][neighborY];
                    }
                }
            }
            return neighborValueSum/8; // divide by 8 because the cell has 8 neighbors
        }


        /**
         * Updates the cellular automaton.
         * 
         * Draws all cells, and, if updateCellValues is true, sets future value for each cell.
         * @param updateCellValues True if cellular automaton is advancing through time (animation is running or the step button is pressed).
         *                         False if the user chose a different color scheme (cell values remain unchanged but visual display is updated).
         */
        void UpdateCA(bool updateCellValues = false) {
            canvas.Clear(); // prevents out of memory error
            if (updateCellValues) { // advances cell values by one update
                cells = futureCells;
            }
            // draws every cell
            for (int x = 0; x < numColBoxes; x++) {
                for (int y = 0; y < numRowBoxes; y++) {
                    DrawCell(x, y);
                    if (updateCellValues) { // if true, computes the value of the cell for the next frame
                        MakeFutureCell(x, y);
                    }
                }
            }
        }


        /**
         * Draws a given cell on the web interface based on the cell's value and the user's most recently pressed color button.
         * @param x Cell's x coordinate.
         * @param y Cell's y coordinate.
         */
        void DrawCell(int x, int y) {
            std::string color;
            // sets cell color based on button flags/hues and cell value
            if (isGrayscale) {color = emp::ColorHSV(0, 0, 1 - cells[x][y]);}
            else if (isRedToPurple) {color = emp::ColorHSV(270*(1 - cells[x][y]), 1, 1);}
            else if (isRedToYellow) {color = emp::ColorHSV(60*(1 - cells[x][y]), 1, 1);}
            else if (isPurpleToCyan) {color = emp::ColorHSV((100*cells[x][y]) + 170, 1, 1);}
            else {color = emp::ColorHSV(hue, cells[x][y], 1);}

            // draws cell
            canvas.Rect(x*cellLength, y*cellLength, cellLength, cellLength, color, color);
        }


        /**
         * Computes a given cell's future value based on the value of cells in its Moore neighborhood.
         * @param x Cell's x coordinate.
         * @param y Cell's y coordinate.
         */
        void MakeFutureCell(int x, int y) {
            double avgNeighborVal =  AvgNeighborValue(x, y);
            // if all neighbor cells have a value of 0, the cell should also have a value of 0
            if (avgNeighborVal == 0.0) {
                futureCells[x][y] = 0.0;
            }
            // otherwise, implements update rule inspired by https://www.wolframscience.com/nks/p158--continuous-cellular-automata/
            else {
                double futureCellVal =  avgNeighborVal + .75;
                futureCellVal -= (int)futureCellVal;
                futureCells[x][y] = futureCellVal;
            }
        }

        
        /**
         * Updates the cellular automaton for each frame while animation is running or when step button is pressed.
         */
        void DoFrame() override {
            UpdateCA(true);
        }
};

CAAnimator animator;

int main() {
    animator.Step();
}