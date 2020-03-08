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

```make clean```   

```make```   

```./sysexplorer```   

Open ```localhost:11880``` page and explore and search your computer's directories


## License

License under the MIT License (MIT)

Copyright © 2016 PLH 301 (TUC) - [Dimitris Kargatzis](https://www.linkedin.com/in/dimitris-kargatzis-1385a2101/)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 

IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
