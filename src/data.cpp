#include "Utility/HelpTypedef.h"
#include "Utility/HelpPuz.h"
#include "Puzzle/Volume.h"
#include "Dataset/Dataset.h"
#include "Puzzle/Puzzle.h"
#include "Puzzle/PuzzleCreator.h"
#include "Puzzle/PuzAction.h"
#include "libigl_UI.h"
#include "libigl_Render.h"
#include "Puzzle/Piece.h"
#include <iostream>
#include <fstream>



#include <nlohmann/json.hpp>

using json = nlohmann::json;

extern int generatedSeedPathNum;

// main function
int main(int argc, char *argv[])
{
    // argument list:
    // 1. mesh path
    // 2. piece number
    // 3. difficulty level
    // 4. path number target
    if (argc < 5)
    {
        cout << "Usage: " << argv[0] << " <mesh path> <piece number> <difficulty level> <path number target>" << endl;
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
    int pathNum = atoi(argv[4]);
    int variance = 0;

    // init puzzle creator
    PuzzleCreator myPuzzleCreator;
    myPuzzleCreator.InitPuzzleCreator(myPuzzle.volume);

    Puzzle *puzzle = NULL;
    int trialNum = 0;

    clock_t beginTime = clock();
    clock_t endTime;
    float elapsed;

    while ( true )
    {
        // choose a random seed
        int seed = rand();
        srand(seed);

        trialNum++;

        if( puzzle != NULL )
        {
            delete puzzle;
            puzzle = NULL;
        }

        puzzle = new Puzzle();
        puzzle->SetVolume( myPuzzle.volume );

        myPuzzleCreator.PartitionVolume_Subdiv(puzzle, pieceNum, level, variance);

        endTime = clock();
        elapsed = ((float) (endTime - beginTime)) / (1.0f*CLOCKS_PER_SEC);

        if (elapsed > 60)
            break;

        if (generatedSeedPathNum >= pathNum)
        {
            printf("Generated path number reached the target\n");
            printf("Generated path number: %d\n", generatedSeedPathNum);
            //printf("Exiting...\n");
            break;
        }
    }

    return 0;
}