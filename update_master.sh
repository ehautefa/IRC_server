#!/bin/bash
git add .
git commit -m $1
git push
git checkout master
git merge $2
git checkout elise

# USE : ./update_master commit_name branch_name
# Commit you work and merge it with master