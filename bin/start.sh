#!/bin/bash

# Need this wrapper since system default python is 2.6 while 2.7 is used in CMSSW

BOOK=$1
DATASET=$2
FILESET=$3

source /cvmfs/cms.cern.ch/cmsset_default.sh
source taskenv.sh

scram project CMSSW $CMSSW_RELEASE
tar xzf *.lib.tar.gz -C $CMSSW_RELEASE
tar xzf *.inc.tar.gz -C $CMSSW_RELEASE

mkdir catalog
tar xzf catalog.tar.gz -C catalog

cd $CMSSW_RELEASE
eval `scram runtime -sh`

cd -

python run.py --book=$BOOK --dataset=$DATASET --fileset=$FILESET analysis.py
