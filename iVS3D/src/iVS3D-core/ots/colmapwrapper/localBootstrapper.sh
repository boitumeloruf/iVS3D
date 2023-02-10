#!/bin/sh

unset LD_LIBRARY_PATH

screen -dm -S lib3D_ots_colmap sh -c "touch <+workspace+>/_colmapRunning; python3 <+workspace+>/ColmapWorkerSkript/ColmapWorker.py <+workspace+>/colmap_worker_state.yaml <+workspace+>/colmap_work_queue.yaml --robust_mode 1 > <+workspace+>/ColmapWorker.log 2>&1; rm <+workspace+>/_colmapRunning;"

exit