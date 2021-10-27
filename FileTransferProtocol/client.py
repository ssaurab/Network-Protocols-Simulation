import socket as sock
import os
import sys


serverport = int(sys.argv[1])
addr = ("127.0.0.1", serverport)

clientsock=sock.socket(sock.AF_INET, sock.SOCK_STREAM)
clientsock.connect(addr)
clientsock.settimeout(5)

the_root_dir=os.getcwd()+'/FTP_CLIENT'
os.chdir(os.path.abspath(the_root_dir))
curr_dir = the_root_dir

def network_interface(message, type):

    if type == "ls" :
        clientsock.send(message)
        server_response = clientsock.recv(4096).decode()
        print("The response from the server is :")
        print(server_response)
    if type == "cd" :
        clientsock.send(message)
        server_response = clientsock.recv(4096)

        print(server_response.decode())

    if type == "quit" :
        clientsock.send(message)
        server_response= clientsock.recv(4096)
        print(server_response.decode())
        clientsock.close()
    if type == "pwd" :
        clientsock.send(message)
        server_response= clientsock.recv(4096)
        print("The current working directory of the server is :")
        print(server_response.decode())


def Client_direc_switch(*dirpath):
    global the_root_dir
    global curr_dir
    ### can I change this ?? ###
    if len(dirpath)==1:
        try:
            os.chdir(os.path.abspath(os.path.join(curr_dir, dirpath[0])))
            curr_dir = os.path.abspath(os.path.join(curr_dir,dirpath[0]))
            
            return curr_dir.split('/')[-1]

        except:
            print("Illegal directory change Operation")
            print("Such directory does not exist in the present directory")
            return "failed"

    ### LCD with no input is below, need some changes ###
    else: 
        
        curr_dir  = the_root_dir
        os.chdir(os.path.abspath(curr_dir))
        return the_root_dir.split('/')[-1]



while(1):

    userin = raw_input('Your command here:')
    stream = userin.split()

    if(stream[0] == 'ls'):

        if len(stream) == 1:
            message = "NLST"
        else:
            message = "NLST "+stream[1]
        print(message)
        message = message.encode()
        network_interface(message, "ls")    

    if(stream[0]=='cd'):
        if len(stream)==1:
            message = "CWD"
        else:
            message = "CWD "+stream[1]
        print(message)
        message = message.encode()
        network_interface(message, 'cd')

    if(stream[0]=='lcd'):

        if len(stream)==1:
            temp = Client_direc_switch()
            print('The new client directory switched to : FTP_CLIENT')

        else:
            temp = Client_direc_switch(stream[1])
            if temp != "failed":
                print('The new directory is :')
                print(stream[1])

    if(stream[0] == 'pwd'):
        
        message='PWD'.encode()
        network_interface(message, 'pwd')

    if(stream[0] == 'quit'):
        
        print("Exiting the connection, closing the sockets ....")
        message='QUIT'.encode()
        network_interface(message, 'quit')
        break

    if(stream[0]=='get'):

        message = 'RETR'
        
        for i in range(1,len(stream)):
            message=message+' '+stream[i]
        
        message = message.encode()
        
        clientsock.send(message)
        status = clientsock.recv(4096).decode()
        print("server ack recieved : ")
        print(status)
        main_str = ''
        if status == "OKtoSend":

            while 1 :
                str = clientsock.recv(4096).decode()

                str = str[:-1]
                main_str += str

                if len(str) < 4096 :
                    fileo= open(stream[1], 'w+')
                    fileo.write(main_str)
                    print("created such a file and write is successful")
                    fileo.close()
                    break


    if(stream[0]=='put'):
        
        fileaddress='./'+stream[1]
        flag = 0
        if os.path.isfile(os.path.abspath(os.path.realpath(os.path.join(curr_dir,fileaddress)))) :
            flag = 1
        else : 
            print("such a file does not exist on the current client directory")


        if flag == 1:
            message='STOR '
        
            for i in range(1,len(stream)):
                message = message+' '+stream[i]


            message = message.encode()
            clientsock.send(message)
            status = clientsock.recv(4096)
            status = status.decode()

            if status=="OKtoSend" and flag == 1:

                print("OK ack recived from server")
                filep = open(fileaddress,'r')
                while 1:

                    s1 = filep.read(4095)
                    s1 += '$'
                    #print(s1)
                    clientsock.send(s1.encode())
                    if len(s1) < 4096: 
                        print("file transmission sucessful")
                        break

            else:
                print(status)

clientsock.close()
