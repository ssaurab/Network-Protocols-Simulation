// Server side implementation of UDP client-server model 
#include <bits/stdc++.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <cstdlib>

using namespace std;

#define PORT	 8080 
#define MAXLINE 1024 


int binaryToDecimal(int n) 
{ 
	int num = n; 
	int dec_value = 0; 
	
	// Initializing base value to 1, i.e 2^0 
	int base = 1; 
	
	int temp = num; 
	while (temp) 
	{ 
		int last_digit = temp % 10; 
		temp = temp/10; 
		
		dec_value += last_digit*base; 
		
		base = base*2; 
	} 
	
	return dec_value; 
} 

int MAX_PACKETS;
int seqbits;
int windowsize = 4;
float rate;
int buffersize;
// Driver code 
int main(int argc, char const *argv[])
{ 
	bool debugflag = false;
	if(argv[1][0] == '-' && argv[1][1] == 'd')
	{
		cout << "ywah" << endl;
		debugflag = true;
		MAX_PACKETS = atoi(argv[5]);
		seqbits = atoi(argv[7]);
		buffersize = atoi(argv[11]);
		rate = atof(argv[13]);
	}
	else
	{
		//debugflag = true;.
		MAX_PACKETS = atoi(argv[4]);
		seqbits = atoi(argv[6]);
		buffersize = atoi(argv[10]);
		rate = atof(argv[12]);
	}



	int sockfd; 
	char buffer[MAXLINE]; 
	char *hello = "Hello from server"; 
	struct sockaddr_in servaddr, cliaddr; 
	
	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	//int MAX_PACKETS = 100;
	int gotit[MAX_PACKETS] = {0};
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(PORT); 
	
	// Bind the socket with the server address 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, 
			sizeof(servaddr)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	//int windowstart = 0;
	//int windowsize = 4;
	while(true){
		int n; 
		socklen_t len;
		n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
					MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
					&len); 
		buffer[n] = '\0';

		int decval = 0;

		cout << buffer << endl;

		string s = buffer;

		cout << s << endl;

		for(int i = 0; i < 32; i++)
		{
			//cout << s[i]<< endl;
			if(s[i] == '\0') break;
			else if(s[i] == '1')
				decval = decval * 2 + 1;
			else 
				decval = decval * 2;
		}

		cout << decval << endl;
		string p;
		p = to_string(decval);
		printf("Client : %s\n", buffer); 
		//&& decval < windowsize + windowstart && decval >= windowstart
		if(((float)(rand()%10)/10.00000000) > rate)
		{
			cout << "got the packet" <<decval <<endl;
			gotit[decval] = 1;

		// 	int x = 1;
		// 	int u = -1;
		// 	while(x == 1 && u < MAX_PACKETS)
		// 	{
		// 		u++;
		// 		x = gotit[u];
		// 	}
		// //	if(gotit[0] == 0) u = -1;
		// 	u--;
		// 	if(u < decval) gotit[decval] = 0;
			p = to_string(decval);
			//cout << (float)(rand() % 10) / 10 << endl;

			sendto(sockfd, (const char *)&p[0], p.size(), 
			MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
				len);

			bool allsent = false;
			for(int i = 0; i < MAX_PACKETS; i++)
			{
				if(gotit[i] == 0)
				{
					allsent = false;
					break;
				}
				else if(i == MAX_PACKETS-1)
				{
					allsent = true;
				}
			}

			if(allsent) 
			{
				return 0;
			}
			cout << "sent" << p << endl;

			// if(windowstart = decval - 1)
			// {
			// 	windowstart ++;
			// }
		} 
		printf("Hello message sent.\n"); 
	}
	
	return 0; 
} 
