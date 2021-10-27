import socket 
import os
import sys
import time

serverPort = int(sys.argv[1])

serverSocket=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serverSocket.bind(('',serverPort))

serverSocket.listen(1)
serverSocket.settimeout(10)
###Do I really need this?###


print("Server setup successful")
###Is the socket setup also complete by here?###

#this is the global variable that carries the string value of the server root
The_root_dir = os.getcwd()+'/FTP_SERVER'
#this is the global variable that carries the string value of pwd
curr_dir = The_root_dir

#print(type(curr_dir)), testing..ignore


def NLIST(*dirpath):

    if dirpath[0] != "null":

        try:
            os.chdir(os.path.abspath(os.path.realpath(os.path.join(curr_dir, dirpath[0]))))
            os.chdir(curr_dir)
            print("showing contents of :")
            return os.path.abspath(os.path.realpath(os.path.join(curr_dir, dirpath[0])))
        except:
            return "invalid directory name"

    elif dirpath[0] == "null":
    	print("showing contents of :")
        return curr_dir

    else :
        #the code never reaches here, testing
        print('testing, ignore!')
        return "invalid directory name"

def CWD(*dirpath):

    global curr_dir
    if dirpath[0] != "null":

    	if direc == ".." and curr_dir == The_root_dir:
    		connectionSocket.send("trying to get out of root, illegal")
    		return "Illegal, going outside root"

        try_flag = 0
        try:
            os.chdir(os.path.abspath(os.path.realpath(os.path.join(curr_dir, dirpath[0]))))
            try_flag = 1    
        except:
            try_flag = -1
            print("the client asked to change into an unexisting directory")
            connectionSocket.send("Illegal CWD Operation".encode())
            return "Illegal CWD Operation"
        
        if try_flag == 1 :
            curr_dir = os.path.abspath(os.path.realpath(os.path.join(curr_dir, dirpath[0])))
        direc_str = curr_dir.split('/')[-1]
        connectionSocket.send(direc_str.encode())
        print("chaning to "),
        print(direc_str),
        print("successful")
        return curr_dir.split('/')[-1]

    elif dirpath[0] == "null":

        #changing the current working directory of the client back to the original root directory of 
        #the server
        curr_dir = The_root_dir
        print(os.path.abspath(os.path.realpath('.')))
        os.chdir(os.path.abspath(os.path.realpath(curr_dir)))
        connectionSocket.send("FTP_SERVER".encode())
        print("Changing back to root folder successful")
        return "FTP_SERVER"

    else :
        #the code never reaches here, testing
        return dirpath

def RETR(filename,*dirname):
    global curr_dir

    flag_dir = 0
    flag_file = 0
    if dirname[0] != "null":
    	try:
            os.chdir(os.path.abspath(os.path.realpath(os.path.join(curr_dir, dirname[0]))))
            old_direc = curr_dir
            curr_dir = os.path.abspath(os.path.realpath(os.path.join(old_direc, dirname[0])))
            dir_path = os.path.abspath(os.path.realpath(os.path.join(old_direc, dirname[0])))
            flag_dir = 1
        except:
            flag_dir = 0
            connectionSocket.send("Invalid Directory parameter given".encode())
            print('Invalid Directory parameter')
            return "Invalid Directory parameter"

  

        if not os.path.isfile(os.path.join(dir_path, filename)):
        	print("Invalid Filename was given")
        	connectionSocket.send("Invalid Filename was given".encode())
        	return "Invalid Filename was given"
        else :
        	finalpathoffile = os.path.join(dir_path, filename)
        	flag_file = 1

    if dirname[0] == "null" :
    	flag_dir = 1
        if os.path.isfile(os.path.join(curr_dir, filename)):
        	flag_file = 1
        	finalpathoffile = os.path.join(curr_dir, filename)
        else :
            connectionSocket.send("Invalid Filename was given".encode())
            return "Invalid Filename was given"
    
    if flag_file == 1 and flag_dir == 1:


        connectionSocket.send("OKtoSend".encode())
        filep = open(finalpathoffile, 'r')
        time.sleep(2)
        print("client's OK ack - recieved")
        while 1:

            s1 = filep.read(4095)
            s1 += '$'
            #print(s1)
            if len(s1) < 4096: 
                connectionSocket.send(s1.encode())
                #print("sending last packet")
                return "success"
            connectionSocket.send(s1.encode())

def STOR(filename,*dirname):

    global curr_dir
    flag_dir = 0

    if dirname[0] == "null" :

            if not os.path.isfile(os.path.join(curr_dir, filename)):
                flag_dir = 1
                #connectionSocket.send("OK".encode())
            else :
                flag_dir = -1

    if dirname[0] != "null" :
        try:
            os.chdir(os.path.abspath(os.path.realpath(os.path.join(curr_dir, dirname[0]))))
            old_direc = curr_dir
            curr_dir = os.path.abspath(os.path.realpath(os.path.join(old_direc, dirname[0])))
            dir_path = os.path.abspath(os.path.realpath(os.path.join(old_direc, dirname[0])))
            if not os.path.isfile(os.path.join(dir_path, filename)):
                flag_dir = 1
            else :
                flag_dir = -1

        except:
            print('Invalid Directory parameter')
            connectionSocket.send("Invalid Directory parameter".encode())
            return "Invalid Directory parameter"
    if flag_dir == -1:
        connectionSocket.send("file already exists or invalid directory".encode())
    if flag_dir == 1:
        connectionSocket.send("OKtoSend".encode())

        main_str = ''
        print("<Ok to send> ack sent")
        while 1 :
            str = connectionSocket.recv(4096).decode()

            str = str[:-1]
            main_str += str

            #print(len(str))
            if len(str) < 4096 :
               # print(main_str)
                fileo= open(filename, 'w+')
                fileo.write(main_str)
                print("file creation and file write successful")
                fileo.close()
               # successmessage = clientSocket.recv(4096)
                break





connectionSocket,clientAddress= serverSocket.accept()
os.chdir(os.path.abspath(The_root_dir))


while 1:

    sentence = connectionSocket.recv(4096)
    #print("sent from client:",sentence)
    lst = sentence.decode().split()
    #print(lst)
   
    print("Client message : ")
    for i in lst :
    	print(i),
    print('')

    if len(lst) == 0:
        break

    if(lst[0]=="NLST"):
        
        if len(lst)==1:
            direc = "null"
        
        else:
            direc=lst[1]

        path_status = NLIST(direc)
        print(path_status)

        if path_status != "invalid directory name":
            string = ' '
            cache = os.listdir(path_status)
            
            if len(cache) != 0:

                string += cache[0]
                cache.pop(0)
                for item in cache:
                    string+=', '
                    string=string+item

            print("The contents of the requested directory are :")
            print(string)
            connectionSocket.send(string.encode())
        
        else:
            string = "invalid directory name"
            connectionSocket.send(string.encode())
    


    if(lst[0]=="CWD"):

        if len(lst)==1:
            direc = "null"
        
        else:
            direc = lst[1]

        if direc == "..":
            print("Invalid cwd opn")


        status = CWD(direc)

    if(lst[0]=="RETR"):
        if len(lst)==2:
            status = RETR(lst[1], "null")
        else:
            status  = RETR(lst[1],lst[2])

        if status == "success" :
            print("transmission success")


    if(lst[0]=="STOR"):


        if len(lst)==2:
            temp=STOR(lst[1], "null")
        else:
            temp=STOR(lst[1],lst[2])

       
    if(lst[0]=="PWD"):

        if len(lst) != 1:
            print("200 OK - However No attribute needed for pwd command")
        current_directory_lastname=curr_dir.split('/')[-1]
        print("The current working directory is :")
        print(current_directory_lastname)
        connectionSocket.send(current_directory_lastname.encode())


    if(lst[0]=="QUIT"):

        if len(lst) != 1:
            print("200 OK- However No attribute needed for quit command")
        print('200 OK - Close request obtained from the client')
        connectionSocket.send("Termination acknowledged from server".encode())
        serverSocket.close()
        break
 
    print('')
    print('')

print("Connection terminated successfully")

