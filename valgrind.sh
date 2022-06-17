#!/bin/bash

EXEC=./ircserv # Put your executable's path here.
LOG=valgrind

# Compile and set executable rights
make && chmod +x $EXEC

valgrind 								\
	--tool=memcheck 					\
	--leak-check=full 					\
	--leak-resolution=high			 	\
	--track-origins=yes 				\
	--track-fds=yes						\
	--show-reachable=yes		 		\
	--log-file=${LOG}.log 				\
	$EXEC $@ 							\
;

# cat valgrind.log | ./suppressions.sh > minimale.supp
#--gen-suppressions=all \

# # Separate the logs from multiple processes into multiple log files.
# grep '==[0-9]*==' <valgrind.log --only-matching | sort --unique | tr -d = |
# 	while read -r pid; 
# 	do \
# 		pidlog=${LOG}.${pid}.log;
# 		echo $pidlog;
# 		grep "$pid" <${LOG}.log >$pidlog;
# 		grep -A1 "valgrind" $pidlog | grep $EXEC;
# 	done;

