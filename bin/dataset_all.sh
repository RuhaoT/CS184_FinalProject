# iterate through all .vol files in the volume directory and its subdirectories
for file in $(find ../volume -name "*.vol"); do
    # Test Dataset
    meshName=$(basename $file .vol)
    pathToMesh=$file
    # how many path to generate
    pathNum=40
    for pieceNumber in 4 5 6 7 8; do
        for difficulty in 3 4 5 6 7; do
            ../bin/High-LevelPuzzle_data $pathToMesh $pieceNumber $difficulty $pathNum
        done
    done
    # rename the output file
    # rename seedpath.json to seedpath_$meshName.json
    mv seedpath.json ../data/seedpath_$meshName.json
    # rename blockpath.json to blockpath_$meshName.json
    mv blockpath.json ../data/blockpath_$meshName.json
done
