#include "Utility/HelpTypedef.h"
#include "Utility/HelpPuz.h"
#include "Puzzle/Volume.h"
#include "Puzzle/Puzzle.h"
#include "Puzzle/PuzzleCreator.h"
#include "Puzzle/PuzAction.h"
#include "libigl_UI.h"
#include "libigl_Render.h"
#include "Puzzle/Piece.h"
#include <iostream>

// main function
int main(int argc, char *argv[])
{
    // argument list:
    // 1. mesh path
    // 2. piece number
    // 3. difficulty level
    // 4. seed number
    if (argc < 5)
    {
        cout << "Usage: " << argv[0] << " <mesh path> <piece number> <difficulty level> <seed number>" << endl;
        return -1;
    }

    // target puzzle
    Puzzle myPuzzle;

    // get file path and file name
    string inputFileName= argv[1];
    bool isFixLastPiece = false;

    // load the mesh
    myPuzzle.ReadPuzzleFile(inputFileName);

    myPuzzle.ReadSmoothPuzzleFile(inputFileName);

    myPuzzle.CheckPuzzleState(true, isFixLastPiece);

    // set the puzzle
    // extract puzzle parameters
    int pieceNum = atoi(argv[2]);
    int level = atoi(argv[3]);
    int seed = atoi(argv[4]);
    int variance = 0;

    srand(seed);

    PuzzleCreator myPuzzleCreator;
    myPuzzleCreator.InitPuzzleCreator(myPuzzle.volume);

    // ready for performance test
    myPuzzle = *myPuzzleCreator.CreateBuildablePuzzle(pieceNum, level, isFixLastPiece, variance);

    printf("Performance Test Ended Successfully\n");

    return 0;
}