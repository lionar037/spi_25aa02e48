#!/bin/bash


./bin/st25vf010_app_aarch64 

./scripts/cmd_winbond.sh  read

python3 scripts/python_compare/compare.py 

