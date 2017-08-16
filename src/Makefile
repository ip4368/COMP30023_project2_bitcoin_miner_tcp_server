# 
# Makefile for project 2
# Submission by Shing Sheung Daniel Ip 723508
#


## CC  = Compiler.
## CFLAGS = Compiler flags.
CC  = gcc -g
CFLAGS =    -Wall -Wextra -std=gnu99 -lpthread


## OBJ = Object files.
## SRC = Source files.
## EXE = Executable name.

SRC =		main.c \
			sha256.c \
			server.c \
			list.c \
			queue.c \
			signal_handler.c \
			request_handler.c \
			alloc.c \
			parser.c \
			logger.c \
			hashcash.c \
			work_daemon.c \
			work_worker.c \
			solution.c \
			ping_ctrl.c \
			pong_ctrl.c \
			okay_ctrl.c \
			erro_ctrl.c \
			soln_ctrl.c \
			work_ctrl.c \
			abrt_ctrl.c \
			unknown_ctrl.c

OBJ =		main.o \
			sha256.o \
			server.o \
			list.o \
			queue.o \
			signal_handler.o \
			request_handler.o \
			alloc.o \
			parser.o \
			logger.o \
			hashcash.o \
			work_daemon.o \
			work_worker.o \
			solution.o \
			ping_ctrl.o \
			pong_ctrl.o \
			okay_ctrl.o \
			erro_ctrl.o \
			soln_ctrl.o \
			work_ctrl.o \
			abrt_ctrl.o \
			unknown_ctrl.o

EXE =       server

## Top level target is executable.
$(EXE): $(OBJ)
		$(CC) $(CFLAGS) -o $(EXE) $(OBJ) -lm


## Clean: Remove object files and core dump files.
clean:
		/bin/rm $(OBJ) 

## Clobber: Performs Clean and removes executable file.

clobber: clean
		/bin/rm $(EXE) 

## Dependencies
sha256.o: 			sha256.h
server.o:			constants.h server.h signal_handler.h \
					alloc.h logger.h work_daemon.h
list.o:				list.h alloc.h
queue.o:			queue.h list.h
signal_handler.o:	signal_handler.h server.h logger.h
request_handler.o:	request_handler.h message.h parser.h \
					server.h logger.h controller.h work_daemon.h
alloc.o:			alloc.h
parser.o:			parser.h alloc.h message.h controller.h \
					request_handler.h logger.h
logger.o:			alloc.h constants.h logger.h
hashcash.o:			hashcash.h uint256.h
work_daemon.o:		constants.h alloc.h work_daemon.h work_worker.h logger.h
work_worker.o:		work_worker.h work_daemon.h solution.h hashcash.h \
					logger.h request_handler.h
solution.o:			solution.h hashcash.h sha256.h uint256.h
ping_ctrl.o:		request_handler.h controller.h logger.h ping_ctrl.h
pong_ctrl.o:		request_handler.h controller.h logger.h pong_ctrl.h
okay_ctrl.o:		request_handler.h controller.h logger.h okay_ctrl.h
erro_ctrl.o:		request_handler.h controller.h logger.h erro_ctrl.h
soln_ctrl.o:		request_handler.h alloc.h controller.h logger.h \
					soln_ctrl.h hashcash.h uint256.h sha256.h solution.h
work_ctrl.o:		request_handler.h alloc.h controller.h logger.h \
					work_ctrl.h work_daemon.h
abrt_ctrl.o:		request_handler.h controller.h logger.h abrt_ctrl.h \
					work_daemon.h
unknown_ctrl.o:		request_handler.h controller.h logger.h \
					unknown_ctrl.h erro_ctrl.h
