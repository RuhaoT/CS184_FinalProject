flamegraphPath=../../FlameGraph

$flamegraphPath/stackcollapse-perf.pl out_Heavy.perf > out_Heavy.folded

$flamegraphPath/flamegraph.pl out_Heavy.folded > flamegraph_Heavy.svg