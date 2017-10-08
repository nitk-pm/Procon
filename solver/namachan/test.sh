#!/bin/bash

source ~/dlang/dmd-2.076.0/activate
dub test --root=`dirname $0`/solver --compiler=dmd
dub test --root=`dirname $0`/mergetool --compiler=dmd
