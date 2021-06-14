## alsWS - The Alsami Web Server
*a scalable, multithreaded family of POSIX webservers for most purposes*
![AlsWS Banner](assets/alsws2.png)

THIS IS THE REPOSITROY FOR THE ALSAMI WEB SERVER. THIS IS UNDER THE EXPAT (MIT) LISCENCE.
Note: this project is independent from the .als frameworks.
*for further reading, visit `docs/`*

`ws/` Is where all the HTTP servers are stored
`tcp/` Is where the TCP Servers are stored
`bin/` Is where the binaries will be stored
`archived/` Is where unstable servers that might not have even compiled successfully go to rest

**alsws** refers to a family of web servers, The basic Network server is located at `nsv/`, the HTTP server is located at `serverlite/`, and the M3 (HTTP) Server implementation is at `ws/M3P`

NSV/WS has a built in auth system.

Explination of ALSWS Servers:
* ws/m3 - The second revision of ALSWS. this revision compiles sucsesfully but is not extremely configurable and does not have any safety guards
* ws/PXX - The mutlthreaded versions of AlsWS webservers.
* ws/SLXX - The former flagship webservers, they run well, are efficient, but not extremely configurable and has numerous vunerabilities
* nsv/ - a simple terminal alsws tcp server, but with an auth system
* tcp/ - A basic TCP server that does not do much, but can be used as a base for TCP applications (without any mention of alsws)

Configurable ALSWS Web Servers load the configuration before initalizing anything except the logging safety guards.
In the config.h file, you can enable checking to see if (for example) 

For Non-configurable versions of AlsWS: 
The Port numbers are **MEANT** to be changed. Before compling, please change the port number to whatever port you desire. 

There is a simple TCP Client bundled with alsws. However, it is highly reccomended to compile the tcp client from source.

the HTTP server requres a port number and a file to serve. If you don't insert those args, the program will terminate and ask you to provide them at relaunch.
