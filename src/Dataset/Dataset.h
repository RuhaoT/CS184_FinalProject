#include "PieceCreator.h"
#include "Utility/HelpPuz.h"

class processSet
{
    public:
    vector<DataState> stateList;

    processSet();
    ~processSet();
}

class DataVoxel
{
    public:
    Voxel *voxel;
    bool isKept;
    bool isSeed;
    bool isEmpty;

    DataVoxel();
    ~DataVoxel();

}

DataVoxel::DataVoxel(Voxel *voxel, bool isKept, bool isSeed, bool isEmpty)
{
    this->voxel = voxel;
    this->isKept = isKept;
    this->isSeed = isSeed;
    this->isEmpty = isEmpty;
}


class DataState
{
    public:
    int stateID;
    vector<DataVoxel> voxelList;

    DataState();
    ~DataState();
}