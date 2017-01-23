# SysExplorer


Description:
------------

Is a multi-thread web server implementation in C programming language. The server provides a page  for each computer’s directory in any web browser. The server has a dynamic functionality, so it can create or kill threads if it is needed. A thread is responsible for making new connections to the server and a new thread is created to handle each new connection. Each new thread equals with a socket (file descriptor) for the connection with the browser. This new thread reads the http request, sends the http response, terminates the connection and then this thread will be detached.   

