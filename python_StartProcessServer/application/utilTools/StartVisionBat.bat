D:
cd D:\aaaaaa\build-WorkerUI-Desktop_Qt_5_7_1_MSVC2015_64bit-Release\release
start /min "" "CoverGlassWorker.exe"

D:
cd D:\aaaaaa\onet_worker1\release
start /min "" "CoverGlassWorker.exe"

D:
cd D:\aaaaaa\build-TrulyMaster-Desktop_Qt_5_7_1_MSVC2015_64bit-Release\release
start /max "" "CoverGlassMaster.exe"

D:
cd D:\aaaaaa\utilTools\clientServer
python clientStartRemoteProcess.py