#include "Utility/HelpPuz.h"
#include "Puzzle/Volume.h"
#include "Puzzle/PieceCreator.h"
#include "Puzzle/Piece.h"
#include "Puzzle/PieceFace.h"
#include <nlohmann/json.hpp>
#include <Eigen/Eigen>
#include <vector>

using json = nlohmann::json;

struct SeedPath;
struct BlockPath;

class Piece;
class Voxel;
class Volume;

class seedPathCreationSequence
{
    public:
    vector<SeedPath> seedPathSequence;
    bool isFinalResultValid;

    //stable information
    //these information are stable and will not change during the process of creating seedPath
    Volume *puzzleVolume;
    vector<Piece*> pieceList;

    seedPathCreationSequence(Volume *puzzleVolume, vector<Piece*> pieceList);
    ~seedPathCreationSequence();

    void addSeedPath(SeedPath seedPath);
    void updateFinalResult();

    void saveSeedPathSequence(string filename);
};

class blockPathCreationSequence
{
    public:
    vector<BlockPath> blockPathSequence;

    //stable information
    //these information are stable and will not change during the process of creating blockPath
    Volume *puzzleVolume;
    vector<Piece*> pieceList;
    SeedPath *seedPath;

    blockPathCreationSequence(Volume *puzzleVolume, vector<Piece*> pieceList, SeedPath *seedPath);
    ~blockPathCreationSequence();

    void addBlockPath(BlockPath blockPath);

    void saveBlockPathSequence(string filename);
};

// helper functions
json blockPathToJson(BlockPath *blockPath);

// convert a SeedPath to a json object
json seedPathToJson(SeedPath *seedPath);

// convert a eigen vector3i vector to a json array
json eigenVector3iToJsonArray(vector<Vector3i> vec);

// convert a int vector to a json array
json intVectorToJsonArray(vector<int> vec);

// convert a voxel to a json object
json voxelToJson(Voxel *voxel);

// convert a voxel vector to a json array
json voxelVectorToJsonArray(vector<Voxel> vec);

// convert a bounding box to a json object
json boundingBoxToJson(BoundingBox* bbox);

// convert a volume to a json object
json volumeToJson(Volume *volume);

// convert a piece face to a json object
json pieceFaceToJson(PieceFace* pieceFace);

// convert a piece to a json object
json pieceToJson(Piece* piece);