///////////////////////////////////////////////////////////////
//
// PuzzleCreator.h
//
//   3D Puzzle Creator Class
//
// by Peng Song and Rulin Chen
//
// 27/Aug/2021
//
//
///////////////////////////////////////////////////////////////

#ifndef _PUZZLE_CREATOR_H
#define _PUZZLE_CREATOR_H

#include "PieceCreator.h"
#include "PuzConfig.h"
#include "PuzCreator_Iter.h"

#include "RandomForest/RandomForest.h"

class Piece;
class Puzzle;
class PuzCreator_Iter;

class PuzzleCreator
{
private:
    Volume *volume;                    // Input volume for constructing puzzles

public:
    PuzzleCreator();
	~PuzzleCreator();
    void InitPuzzleCreator(Volume *_volume);

	/// Create Interlocking, Buildable and High-level Puzzle
    Puzzle* CreateBuildablePuzzle(int pieceNum, int keyLevel, bool isFixLastPiece, float variance);
    vector< vector<int> > CreateBuildablePuzzle_creator(int pieceNum, int keyLevel, bool isFixLastPiece, float variance, float timeLimit, int k, bool & isNeedModification);
    void PartitionVolume_Subdiv(Puzzle *puzzle, int pieceNum, int keyLevel, float variance, RandomForest randomForest);
    bool ConstructPiece(Puzzle *puzzle, int pieceNum, int keyLevel, int maxSubdivTimes, vector<int> pieceVoxelNums, RandomForest randomForest);

    /// Construct Puzzle Pieces by Subdivision
    bool SubdivideKey(Puzzle *puzzle, Piece *origPiece, Piece *remvPiece, Piece *restPiece, int remvAvgVoxelNum, vector<seedPathCreationSequence> &seedPathSequenceArray, RandomForest randomForest);
    bool SubdividePiece(Puzzle *puzzle, Piece *origPiece, Piece *remvPiece, Piece *restPiece, int remvAvgVoxelNum);
	bool CheckPieceSubdivision(Puzzle *puzzle, int pieceNum, int keyLevel);
	void ComputePieceReachability(Puzzle *puzzle, int pieceID);
	void BuildRemovePiece(Puzzle *puzzle, Piece *origPiece, Piece *remvPiece, Piece *restPiece, vector<Vector3i> remvVoxels);
	void DeleteCurrPiece(Puzzle *puzzle, Piece *origPiece, Piece *remvPiece, Piece *restPiece, vector<Vector3i> remvVoxels);

};

#endif
