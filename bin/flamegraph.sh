flamegraphPath=../../FlameGraph

$flamegraphPath/stackcollapse-perf.pl out.perf > out.folded

$flamegraphPath/flamegraph.pl out.folded > flamegraph.svg