## ALSAMI WEB SERVER
THIS IS THE REPOSITROY FOR THE ALSAMI WEB SERVER. THIS IS UNDER THE EXPAT (MIT) LISCENCE.
Note: this project is independent from the .als frameworks.
*for further reading, visit `docs/`*

HTTP server attached at
`serverlite/`
TCP server attached at
`rev/`

This is a light server that runs well.

**alsws** refers to a family of web servers, The basic Network server is located at `nsv/`, the HTTP server is located at `serverlite/`, and the M3 (HTTP) Server implementation is at `ws/M3P`

NSV/WS has a built in auth system.

Explination of ALSWS Servers:
* ws/basenet: Sami's First webserver. Does not compile but it's a pretty cool thing to have included in the repo
* ws/m3 The second revision of ALSWS. this revision compiles sucsesfully and compiles sucsesfully


The Port numbers are **MEANT** to be changed. Before compling, please change the port number to whatever port you desire. 

There is a simple TCP Client bundled with alsws. However, it is highly reccomended to compile the tcpclient from source

the HTTP server requres a port number and a file to serve. if you dont insert those args, the program will terminate and ask you to provide than at relaunch.

**THIS COMES WITH NO WARRINTY WHATSOEVER ATTACHED**
