#!../../bin/linux-x86/myexample

## You may have to change myexample to something else
## everywhere it appears in this file

< envPaths_x86
#< envPaths

cd ${TOP}


## Register all support components
dbLoadDatabase "dbd/myexample.dbd"
myexample_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadTemplate "db/userHost.substitutions"
dbLoadRecords "db/dbHybT.db"
dbLoadRecords "db/dbHybLV.db"
dbLoadRecords "db/dbHybLVStat.db"
dbLoadRecords "db/dbFebTemp.db"
dbLoadRecords "db/dbHybSwitch.db"
dbLoadRecords "db/dbHybSync.db"
dbLoadRecords "db/dbDpmMap.db"
dbLoadRecords "db/dbFebLayer.db"
dbLoadRecords "db/dbSVT.db"

# syncbase records
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=0,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=0,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=0,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=0,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=0,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=0,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=1,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=1,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=1,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=1,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=1,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=1,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=2,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=2,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=2,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=2,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=2,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=2,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=3,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=3,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=3,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=3,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=3,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=3,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=4,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=4,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=4,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=4,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=4,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=0,HYB=4,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=0,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=0,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=0,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=0,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=0,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=0,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=1,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=1,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=1,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=1,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=1,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=1,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=2,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=2,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=2,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=2,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=2,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=2,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=3,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=3,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=3,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=3,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=3,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=3,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=4,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=4,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=4,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=4,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=4,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=1,HYB=4,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=0,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=0,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=0,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=0,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=0,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=0,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=1,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=1,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=1,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=1,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=1,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=1,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=2,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=2,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=2,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=2,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=2,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=2,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=3,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=3,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=3,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=3,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=3,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=3,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=4,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=4,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=4,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=4,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=4,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=2,HYB=4,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=0,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=0,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=0,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=0,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=0,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=0,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=1,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=1,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=1,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=1,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=1,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=1,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=2,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=2,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=2,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=2,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=2,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=2,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=3,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=3,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=3,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=3,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=3,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=3,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=4,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=4,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=4,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=4,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=4,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=3,HYB=4,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=0,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=0,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=0,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=0,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=0,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=0,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=1,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=1,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=1,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=1,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=1,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=1,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=2,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=2,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=2,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=2,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=2,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=2,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=3,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=3,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=3,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=3,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=3,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=3,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=4,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=4,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=4,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=4,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=4,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=4,HYB=4,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=0,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=0,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=0,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=0,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=0,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=0,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=1,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=1,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=1,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=1,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=1,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=1,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=2,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=2,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=2,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=2,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=2,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=2,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=3,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=3,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=3,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=3,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=3,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=3,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=4,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=4,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=4,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=4,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=4,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=5,HYB=4,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=0,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=0,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=0,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=0,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=0,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=0,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=1,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=1,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=1,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=1,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=1,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=1,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=2,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=2,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=2,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=2,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=2,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=2,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=3,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=3,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=3,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=3,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=3,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=3,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=4,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=4,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=4,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=4,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=4,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=6,HYB=4,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=0,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=0,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=0,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=0,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=0,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=0,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=1,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=1,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=1,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=1,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=1,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=1,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=2,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=2,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=2,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=2,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=2,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=2,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=3,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=3,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=3,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=3,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=3,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=3,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=4,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=4,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=4,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=4,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=4,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=7,HYB=4,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=0,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=0,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=0,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=0,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=0,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=0,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=1,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=1,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=1,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=1,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=1,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=1,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=2,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=2,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=2,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=2,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=2,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=2,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=3,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=3,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=3,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=3,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=3,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=3,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=4,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=4,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=4,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=4,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=4,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=8,HYB=4,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=0,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=0,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=0,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=0,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=0,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=0,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=1,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=1,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=1,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=1,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=1,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=1,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=2,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=2,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=2,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=2,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=2,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=2,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=3,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=3,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=3,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=3,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=3,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=3,APV=5")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=4,APV=0")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=4,APV=1")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=4,APV=2")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=4,APV=3")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=4,APV=4")
dbLoadRecords("db/dbHybSyncBase.db","FEB=9,HYB=4,APV=5")



## Set this to see messages from mySub
var mySubDebug 0

## Run this to trace the stages of iocInit
traceIocInit

cd ${TOP}/iocBoot/${IOC}
iocInit

## Start any sequence programs
#seq sncExample, "user=rootHost"
#seq sncExample, "user=trackerHost"



