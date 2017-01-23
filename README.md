# SysExplorer


Description:
------------

Is a multi-thread web server implementation in C programming language. The server <br>
provides a page  for each computer’s directory in any web browser. The server has <br>
a dynamic functionality, so it can create or kill threads if it is needed. A thread <br>
is responsible for making new connections to the server and a new thread is created <br>
to handle each new connection. Each new thread equals with a socket (file descriptor) <br>
for the connection with the browser. This new thread reads the http request, sends <br>
the http response, terminates the connection and then this thread will be detached. <br>   


Performance Improvement:
------------------------

<b>1.</b> Thread Poll Implementation

is a collection of workers threads organized in a queue that efficiently execute <br>
asynchronous callbacks on behalf of the server. It is primarily used to reduce <br>
the server resources, providing management of the worker threads. If a thread is <br>
done and the number of inactive threads is less than N=10, insert this thread in <br>
queue and set it inactive with the help of a condition variable, otherwise kill <br>
this thread.

<b>2.</b> Cache LRU (Least Recently Used)

is a temporary storage mechanism for created HTML pages to avoid re-creation of <br>
own pages in the future. Each stored object in cache has four fields:


<table>
<tr>
  <td>
    <b>path:</b>
  </td>
  <td>
    directory of stored HTML page
  </td>
</tr>
<tr>
  <td>
    <b>htmltext:</b>
  </td>
  <td>
    HTML page content
  </td>
</tr>
<tr>
  <td>
    <b>htmllen:</b>
  </td>
  <td>
    bytes of HTML page 
  </td>
</tr>
<tr>
  <td>
    <b>version:</b>
  </td>
  <td>
    if directory is changed we must create the HTML page again
  </td>
</tr>
</table>


maximum cache size in this project = 100


Execute:
--------

$ make clean <br>
$ make <br>
$ ./sysexplorer <br>

Open localhost:11880 page and explore or search your computer's directories

