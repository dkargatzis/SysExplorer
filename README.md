# SysExplorer


Description:
------------

Is a multi-thread web server implementation in C programming language. The server 
provides a page  for each computer’s directory in any web browser. The server has
a dynamic functionality, so it can create or kill threads if it is needed. A thread
is responsible for making new connections to the server and a new thread is created
to handle each new connection. Each new thread equals with a socket (file descriptor)
for the connection with the browser. This new thread reads the http request, sends 
the http response, terminates the connection and then this thread will be detached.   


Performance Improvement:
------------------------

1. Thread Poll Implementation

is a collection of workers threads organized in a queue that efficiently execute 
asynchronous callbacks on behalf of the server. It is primarily used to reduce 
the server resources, providing management of the worker threads. If a thread is
done and the number of inactive threads is less than N=10, insert this thread in
queue and set it inactive with the help of a condition variable, otherwise kill
this thread.

2. Cache LRU (Least Recently Used)

is a temporary storage mechanism for created HTML pages to avoid re-creation of
own pages in the future. Each stored object in cache has four fields:

<b>path:</b>          drectory of stored HTML page
<b>htmltext:</b>      HTML page content
<b>htmllen:</b>       bytes of HTML page
<b>version</b>        if directory is changed we must create the HTML page again

maximum cache size in this project = 100


Execute:
--------

$ make clean
$ make
$ ./sysexplorer

Open localhost:11880 page and explore or search your computer's directories

