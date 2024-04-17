# Test Profiling
pathToMesh=../volume/fig_13_results_voxelized/Spider/Spider_12x13x6.vol
pieceNumber=4
difficulty=3
seed=50
perfPath=/usr/lib/linux-tools/5.15.0-102-generic/perf

# Test 2
echo "Test Heavy"
$perfPath record -g ./High-LevelPuzzle_perf $pathToMesh $pieceNumber $difficulty $seed
$perfPath script -i perf.data > out_Heavy.perf