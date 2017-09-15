#!/bin/bash

source ~/dlang/dmd-2.076.0/activate
dub test --root=`dirname $0` --compiler=dmd
