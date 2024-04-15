# Test Profiling
pathToMesh=../volume/tbl_2_cubes_compare/Cube_4x4x4_E1.vol
pieceNumber=4
difficulty=4
seed=50
perfPath=/usr/lib/linux-tools/5.15.0-102-generic/perf

# Test 1
echo "Test 1"
$perfPath record -g ./High-LevelPuzzle_perf $pathToMesh $pieceNumber $difficulty $seed
$perfPath script -i perf.data > out.perf