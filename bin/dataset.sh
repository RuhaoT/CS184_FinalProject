# Test Dataset
meshName=Airplane_14x6x17
pathToMesh=../volume/fig_14_fabrication/Airplane/Airplane_14x6x17.vol
pieceNumber=4
difficulty=4
seed=50

../bin/High-LevelPuzzle_perf $pathToMesh $pieceNumber $difficulty $seed

# rename the output file
# rename seedpath.json to seedpath_$meshName.json
mv seedpath.json seedpath_$meshName.json
# rename blockpath.json to blockpath_$meshName.json
mv blockpath.json blockpath_$meshName.json