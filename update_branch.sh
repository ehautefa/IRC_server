#!/bin/bash
git checkout master
git pull
git checkout $1
git merge master

# USE : ./update_branch branch_name
# Pull master and merge master with your branch