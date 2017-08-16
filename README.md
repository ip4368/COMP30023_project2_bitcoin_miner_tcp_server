# COMP30023 Computer Systems 2017 --- Project 2
Project 2 for COMP30023 Computer Systems 2017 in The University of Melbourne (unimelb) --- Bitcoin Miner TCP server

---

# DISCLAIMER
This is a solution for an assignment, but students from upcoming years should _NOT_ copy any code from this solution even the question is the same. You can read the code, understand how I implement the solution, and you are more than welcome to email me if you need help on understanding my solution.

---

# Compile and Execution
This program only works on Linux and macOS, as they use Linux and macOS specific system call. (potentially more unix systems, but not tested)

## Compile
Dependencies:
 * a gcc compiler
 * make program
```bash
git clone https://github.com/ip4368/COMP30023_project2_bitcoin_miner_tcp_server.git
cd COMP30023_project2_bitcoin_miner_tcp_server/src

make
```

## Execute
After compilation, you can specify the port number, the example below uses port 5000
```bash
./server 5000
```

---

# Details
This is a nearly full mark + bonus marks solution except I made a small mistake on one respond message.

This TCP server uses an architecture that is similar to Apache webserver for request handling. Which is quite different from what Nginx did. I am going discuss a bit on the advantages and disadvantages between this 2 different methods.

## Command handling
This server support makes it very easy to add more command (header). Developer only needs to develop new controller, and add the controller into [`controller.h`](src/controller.h), you can look into [`controller.h`](src/controller.h) to understand how to add more controller. Besides, you can look into [`message.h`](src/message.h) which defines the interface that the controller should do, e.g. what are the functions declaration of the controller should include.

## WORK message handling (special command)
This server handle WORK message in a single daemon way, jobs will be queued up and the daemon will try to fetch job when ever it can. The efficient implementation is done by using semaphore, you can take a look how I implement it. Please be noted that the semaphore that available in Linux is not available on macOS, so you might see some macro for compiler to know which platform it is being compiled on and choose the correct semaphore implementation. The daemon is started before the server start serving.

The [`queue.h`](src/queue.h) library, which based on the [`queue.h`](src/list.h) library I implented before, is not thread safe. Therefore, when I implement the daemon, I need to do some works to make sure I don't mess up the queue. I could potentially update the [`list.h`](src/list.h) library later on to make it thread safe. I will update the README once it is done.

## Log library
This server use a mono logging library to unify the logging action. The logging library is thread safe, so no matter which thead try to use the log, it will not mess up the logging action. Although it is thread safe, but I would not say that it is a very good implementation, because if a thread is trying to log, other thread basically need to wait that thread to finish the log until they can log action. This cause a little bit more logging time and performance may degrade if there are long queue for logging. A better implementation will be using a queue to store all the incoming log, so the thread that trying to log things into file can go back to do their work instead of blocking. It will have a similar concept with the WORK message handling daemon.

## Server bootstrap process
The order of bootstrapping the server is as follow
 1. Register the log file to the log library (logger should be a singleton)
 2. Register the Interupt Signal Handler, it is for gracefully shutdown the server if there is a interupt signal received (ctrl-c)
 3. Initialise the server instance
 4. Register the server instance to the Interupt Signal Handler, so the handler can actually stop the serve
 5. Register the handler function for a connection to the server
 6. server start serving, in the serve function, before it actually start listening, the WORK message daemon is spawn on a new thread.

## Connection and Request handling
The order of handling connection and request are as follow
 1. Main thread listen the connection, using `accept` function (this is a system call)
 2. The main thread spin up a new thread with the handler that is registered
 3. The client thread try to `read` (also system call) from client
 4. If the `read` return
     * 0, it means the connection is closed by the client, clean up and stop trying to read from client
     * &lt;0, it means there is error, log it into the log file and continue `read` (I am not sure if this indicate connection drop or what, probably better just to assume disconnection)
     * &gt;0, it means successfully read some bytes, parse the Header, and use the corresponding parser function to parse the Body.
 5. The client thread call the corresponding message handler
 6. The message handler process request and possibly respond (this happen in the controller handling function)
 7. goes back to step 3
 
 ## Comparison between this connection handling method vs Nginx handling method
  1. This method will have more instant respond if you have enough core count in the system, and a lot of threads are blocking
  2. Nginx method (which uses poll/select, epoll on Linux, kqueue on FreeBSD, OpenBSD, NetBSD, and macOS, they are similar but system optimised system call) will has better scalability, as less thread is spawn, the system will not need to do context switch often (which affected by scheduling algorithm as well). Nginx only spawn n threads if there are n logical threads in maximum for handling request, but all connections are handled by 1 single thread using the poll/select-like functions.
  Generally I think Nginx method should be more efficient (You can search online for Apache vs Nginx comparison in detail).
  
---

I hope that you, if you are student who wondering how to implement a tcp server, can understand a little bit more, and have an idea on how to write your own TCP server. Again, you are more than welcome to email me if you need help on understanding my solution.
