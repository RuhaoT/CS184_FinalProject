#include "Utility/HelpPuz.h"
#include "Puzzle/Volume.h"
#include "Puzzle/PieceCreator.h"
#include "Puzzle/Piece.h"
#include "Puzzle/PieceFace.h"
#include <nlohmann/json.hpp>
#include <Eigen/Eigen>
#include <fstream>
#include <vector>

using json = nlohmann::json;

class Piece;
class Voxel;
class Volume;

struct SeedPath;
struct BlockPath;

// seedPathCreationSequence

// Initialization
seedPathCreationSequence::seedPathCreationSequence(Volume *puzzleVolume, vector<Piece*> pieceList)
{
    this->puzzleVolume = puzzleVolume;
    this->pieceList = pieceList;
    isFinalResultValid = false;
}

// Destructor
seedPathCreationSequence::~seedPathCreationSequence()
{
    // Do nothing
}

// Add a seedPath to the sequence
void seedPathCreationSequence::addSeedPath(SeedPath seedPath)
{
    seedPathSequence.push_back(seedPath);
}

// Update the final result
void seedPathCreationSequence::updateFinalResult()
{
    // Check if the last seedPath is valid
    if (seedPathSequence.back().isValid)
    {
        isFinalResultValid = true;
    }
}

// Save the seedPath sequence to a file
void seedPathCreationSequence::saveSeedPathSequence(string filename)
{
    // if not valid, do not save
    if (!isFinalResultValid)
    {
        return;
    }

    json j;
    j["isFinalResultValid"] = isFinalResultValid;

    // Save creation sequence
    j["seedPathSequence"] = json::array();
    for (int i = 0; i < seedPathSequence.size(); i++)
    {
        j["seedPathSequence"].push_back(seedPathToJson(&(seedPathSequence[i])));
    }
    //printf("Saved seedPathSequence\n");

    ofstream file(filename, ios::app);

    // Save stable information
    j["puzzleVolume"] = volumeToJson(puzzleVolume);
    //printf("Saved puzzleVolume\n");
    j["pieceList"] = json::array();
    for (int i = 0; i < pieceList.size(); i++)
    {
        j["pieceList"].push_back(pieceToJson((pieceList[i])));
    }
    //printf("Saved stable information\n");
    printf("Saved seedPathSequence\n");

    // append to the previous json object
    file << j.dump(4);
    file.close();
}

// blockPathCreationSequence

// Initialization
blockPathCreationSequence::blockPathCreationSequence(Volume *puzzleVolume, vector<Piece*> pieceList, SeedPath *seedPath)
{
    this->puzzleVolume = puzzleVolume;
    this->pieceList = pieceList;
    this->seedPath = seedPath;
}

// Destructor
blockPathCreationSequence::~blockPathCreationSequence()
{
    // Do nothing
}

// Add a blockPath to the sequence
void blockPathCreationSequence::addBlockPath(BlockPath blockPath)
{
    blockPathSequence.push_back(blockPath);
}

// Save the blockPath sequence to a file
void blockPathCreationSequence::saveBlockPathSequence(string filename)
{
    json j;
    // Save creation sequence
    j["blockPathSequence"] = json::array();
    for (int i = 0; i < blockPathSequence.size(); i++)
    {
        j["blockPathSequence"].push_back(blockPathToJson(&(blockPathSequence[i])));
    }
    //printf("Saved blockPathSequence\n");

    ofstream file(filename, ios::app);

    // Save stable information
    j["puzzleVolume"] = volumeToJson(puzzleVolume);
    //printf("Saved puzzleVolume\n");
    j["pieceList"] = json::array();
    for (int i = 0; i < pieceList.size(); i++)
    {
        j["pieceList"].push_back(pieceToJson((pieceList[i])));
    }
    j["seedPath"] = seedPathToJson(seedPath);
    //printf("Saved stable information\n");
    printf("Saved blockPathSequence\n");

    // append to the previous json object
    file << j.dump(4);
    file.close();
}

// Helper functions
json eigenVector3iToJsonArray(vector<Vector3i> vec)
{
    json j = json::array();
    for (int i = 0; i < vec.size(); i++)
    {
        j.push_back({vec[i][0], vec[i][1], vec[i][2]});
    }
    return j;
}

json intVectorToJsonArray(vector<int> vec)
{
    json j = json::array();
    for (int i = 0; i < vec.size(); i++)
    {
        j.push_back(vec[i]);
    }
    return j;
}

json voxelToJson(Voxel *voxel)
{
    json j;
    j["center"] = {voxel->center[0], voxel->center[1], voxel->center[2]};
    j["size"] = {voxel->size[0], voxel->size[1], voxel->size[2]};
    j["pos"] = {voxel->pos[0], voxel->pos[1], voxel->pos[2]};
    j["state"] = voxel->state;
    j["piece"] = voxel->piece;
    for (int i = 0; i < 6; i++)
    {
        j["neiborPiece"].push_back(voxel->neiborPiece[i]);
    }
    for (int i = 0; i < 6; i++)
    {
        j["neiborVoxel"].push_back(voxel->neiborVoxel[i]);
    }
    j["reach"] = voxel->reach;
    j["color"] = voxel->color;
    j["dist"] = voxel->dist;
    j["visited"] = voxel->visited;
    j["type"] = voxel->type;
    j["marked"] = voxel->marked;
    return j;
}

json voxelVectorToJsonArray(vector<Voxel> vec)
{
    json j = json::array();
    for (int i = 0; i < vec.size(); i++)
    {
        j.push_back(voxelToJson(&(vec[i])));
    }
    return j;
}

json volumeToJson(Volume *volume)
{
    json j;
    j["volumeSize"] = {volume->volumeSize[0], volume->volumeSize[1], volume->volumeSize[2]};
    j["voxelSize"] = {volume->voxelSize[0], volume->voxelSize[1], volume->voxelSize[2]};
    j["volVoxelNum"] = volume->volVoxelNum;
    j["bbox"] = boundingBoxToJson((volume->bbox));
    j["outBBoxVoxel"] = voxelToJson((volume->outBBoxVoxel));
    for (int i = 0; i < volume->voxelGrid.size(); i++)
    {
        j["voxelGrid"].push_back(voxelToJson((volume->voxelGrid[i])));
    }
    
    return j;
}

json boundingBoxToJson(BoundingBox* bbox)
{
    json j;
    j["minPt"] = {bbox->minPt[0], bbox->minPt[1], bbox->minPt[2]};
    j["maxPt"] = {bbox->maxPt[0], bbox->maxPt[1], bbox->maxPt[2]};
    j["cenPt"] = {bbox->cenPt[0], bbox->cenPt[1], bbox->cenPt[2]};

    return j;
}

// convert a piece face to a json object
json pieceFaceToJson(PieceFace* pieceFace)
{
    json j;
    j["faceID"] = pieceFace->faceID;
    // iterate through the voxel list and convert each voxel to a json object
    for (int i = 0; i < pieceFace->surfaceVoxel.size(); i++)
    {
        j["surfaceVoxel"].push_back(voxelToJson((pieceFace->surfaceVoxel[i])));
    }
    for (int i = 0; i < pieceFace->contactVoxel.size(); i++)
    {
        j["contactVoxel"].push_back(voxelToJson((pieceFace->contactVoxel[i])));
    }
    j["contactedPiece"] = intVectorToJsonArray(pieceFace->contactedPiece);
    return j;
}

// convert a piece to a json object
json pieceToJson(Piece* piece)
{
    json j;
    j["isPicked"] = piece->isPicked;
    j["pieceID"] = piece->pieceID;
    for (int i = 0; i < piece->voxelList.size(); i++)
    {
        j["voxelList"].push_back(voxelToJson((piece->voxelList[i])));
    }
    j["pieceFace"] = pieceFaceToJson((piece->pieceFace));
    j["neiborPiece"] = intVectorToJsonArray(piece->neiborPiece);
    j["movableAxis"] = intVectorToJsonArray(piece->movableAxis);
    return j;
}

// convert a SeedPath to a json object
json seedPathToJson(SeedPath *seedPath)
{
    json j;
    j["pieceID"] = seedPath->pieceID;
    j["pieceAxisID"] = seedPath->pieceAxisID;
    j["isValid"] = seedPath->isValid;
    j["contaVoxels"] = eigenVector3iToJsonArray(seedPath->contaVoxels);
    j["seedVoxels"] = eigenVector3iToJsonArray(seedPath->seedVoxels);
    j["keptVoxels"] = eigenVector3iToJsonArray(seedPath->keptVoxels);
    j["seedPossib"] = seedPath->seedPossib;
    j["pathVoxels"] = eigenVector3iToJsonArray(seedPath->pathVoxels);
    j["moveAxisIDs"] = intVectorToJsonArray(seedPath->moveAxisIDs);
    j["seedMoveSteps"] = intVectorToJsonArray(seedPath->seedMoveSteps);
    j["moveStep"] = seedPath->moveStep;
    j["enableBackMotion"] = seedPath->enableBackMotion;
    return j;
}

// convert a BlockPath to a json object
json blockPathToJson(BlockPath *blockPath)
{
    json j;
    j["pieceID"] = blockPath->pieceID;
    j["pieceAxisID"] = blockPath->pieceAxisID;
    j["isValid"] = blockPath->isValid;
    j["blockVoxel"] = {blockPath->blockVoxel[0], blockPath->blockVoxel[1], blockPath->blockVoxel[2]};
    j["keptVoxels"] = eigenVector3iToJsonArray(blockPath->keptVoxels);
    j["blkPossib"] = blockPath->blkPossib;
    j["pathVoxels"] = eigenVector3iToJsonArray(blockPath->pathVoxels);
    return j;
}