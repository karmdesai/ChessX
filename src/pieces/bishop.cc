#include "bishop.h"

Bishop::Bishop(char name, char color)
    : Piece(3, name, color) {}

void Bishop::getAllPossibleMoves(std::pair<char, int> currentPosition) {
    // clear the vector of previous possible moves
    this->allPossibleMoves.clear();

    // Bishop can move diagonally in all directions.
    int x = currentPosition.first;
    int y = currentPosition.second;
    
    // move diagonally left and down
    while (x > 'a' && y > 1) {
        x -= 1;
        y -= 1;

        std::pair<char, int> newMove = std::make_pair(x, y);
        this->allPossibleMoves.push_back(newMove);
    }

    x = currentPosition.first;
    y = currentPosition.second;

    // move diagonally left and up
    while (x > 'a' && y < 8) {
        x -= 1;
        y += 1;

        std::pair<char, int> newMove = std::make_pair(x, y);
        this->allPossibleMoves.push_back(newMove);
    }

    x = currentPosition.first;
    y = currentPosition.second;

    // move diagonally right and up
    while (x < 'h' && y < 8) {
        x += 1;
        y += 1;

        std::pair<char, int> newMove = std::make_pair(x, y);
        this->allPossibleMoves.push_back(newMove);
    }

    x = currentPosition.first;
    y = currentPosition.second;

    // move diagonally right and down
    while (x < 'h' && y > 1) {
        x += 1;
        y -= 1;

        std::pair<char, int> newMove = std::make_pair(x, y);
        this->allPossibleMoves.push_back(newMove);
    }
}
