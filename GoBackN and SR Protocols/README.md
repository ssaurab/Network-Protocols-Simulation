The port number is hardcoded and thus need not be mentioned.
To run the source codes, first compile them using make file.
After that run the executables in pairs as follows : 

SR : ./srclient -d -s m2 -p 12345 -n 8 -L 512 -R 10 -N 10 -W 4 -B 100
     ./srserver -d -p 2312 -N 10 -n 8 -w 4 -B 100 -e 0.5

GBN : ./clien -d -s machine1 -p 12345 -l 512 -r 10 -n 10 -w 4 -b 6
	./server -d -p 000 -n 10 -e 0.1
Note : do not exculed any field in the command line input, except for -d 
flag which can be removed if debugging mode is not to be activated. Also the 
order of these parameters must be maintained same.

