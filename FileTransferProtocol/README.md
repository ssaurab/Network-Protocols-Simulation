

Run the .py files as :
	
	python client.py 5001
	python server.py 5001

The IP addresses are hardcoded and can be changed from the
client and server files.

IP address = 127.0.0.1(localhost), 10.2.0.15(internal IP)
IP of foreign = 192.168.0.103 when the transfer was done.
MAC address of the present device : 80:56:f2:34:8c:bf
MAC address of the foreign device : 08:00:27:0b:60:98

Note :

The client checks is a file is present in its current directory 
before sending a STOR request. This implementation in slightly 
different from the one asked in assignment as the directory on 
the server side is changed whenever a wrong 'put' request is made
otherwise. 

Two folders named FTP_SERVER AND FTP_CLIENT act as the root folders
for client and servers respectively as given in the sample session of the
assignment. All the files must be inside these folders and these names are 
hardcoded in the codes written. 

No special character addition ($,/) has been done. The file transfer can occur
using the intact data of the files, this is achieved by sending constant length
packets and terminating when a packet goes under this size.

The script file included has the sample run recorded when ran on terminal.
The transfer of files has been confirmed by looking directly into the 
directory as well.

The .pcapng files can be opened on wireshark, they show the captures of a old
transmission with different set of file structures. The wireshark capture has
the explaination for all the packets sent to and fro under the TCP protocol.

Also note that I have included the FTP-SERVER, FTP-CLIENT folders for reference

