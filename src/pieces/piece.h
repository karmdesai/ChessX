#include <vector>

class Piece {
    private:
        int value;
        char name;
        char color;
        bool inStartingPosition;

        /* Moves are represented by a pair<char, int>, where: 
            - the character is a letter ranging from a through h.
            - the integer is a number ranging from 1 to 8. 
            
            Although the character ranges from a through h, we can 
            also treat it as an integer ranging from 97 to 104 (ASCII). */
        std::vector<std::pair<char, int>> allPossibleMoves;

    public:
        // constructor
        Piece(int value, char name, char color, bool inStartingPosition);

        int getValue();
        char getName();
        char getColor();

        // the Piece class is an abstract class
        void virtual getAllPossibleMoves() = 0;
};
