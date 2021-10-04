#include <bits/stdc++.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <time.h>
#include <chrono>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>

using namespace std;

#define PORT	 8080 
#define MAXLINE 1024 


pthread_mutex_t lockt; 


int MAX_PACKETS;
vector <string> actualbuffer;
vector <int> bufferavail;
vector <int> currbufindex;


string decToBinary(int n) 
{ 
	string s = "";
	// Size of assumed integer is assumed to be 32 bits 
	for (int i = 31; i >= 0; i--) { 
		int k = n >> i; 
		if (k & 1) 
			s += '1'; 
		else
			s += '0'; 
	} 

	return s;
} 



void threadtask(int maxbuffersize, int packetgenrate)
{
	cout << endl << endl;
	cout << bufferavail[1] << "thread1 talking" << maxbuffersize << " " << packetgenrate << endl;
	cout << endl << endl;
	//return;
	int n = 0;
	int index = 0;

	int attempts = 0;
	while(n < MAX_PACKETS){
		attempts ++;
		//usleep(1000);
		if(attempts > 1000000)
			return;
		index = 0;
	//	cout << "threads mn : " << n << endl;
		while(bufferavail[index] != -1 && index < maxbuffersize)
		{
			index ++;
		}

		if(index != maxbuffersize)
		{
			//cout << creating 
			 pthread_mutex_lock(&lockt); 
			bufferavail[index] = 1;
			actualbuffer[index] = decToBinary(n);
			currbufindex[index] = n;
			n++;
			 pthread_mutex_unlock(&lockt); 
			usleep((1/packetgenrate) * 1000000);
		}

		for(int i = 0; i < maxbuffersize; i++)
		{
			//cout << actualbuffer[i] << " " << currbufindex[i] << "hmm" ;
		}
	}

	
	cout << "thread closing"<< endl;
	return;
}

char buffer[MAXLINE]; 

int stooi()
{
	int k = 0;
	for(int i = 0; i < 32; i++)
	{
		if(buffer[i] == 45)
			return -1;
		if(buffer[i]-48 <= 9 && buffer[i]-48 >=0 )
			k = k * 10 + (buffer[i] - 48);
		cout << buffer[i] << " " ;
	}


	//cout << k << "stooi returned this <-" << endl;
	return k;
}

// Driver code 
int main(int argc, char const *argv[]) { 

	bool debugflag = false;
	int packetlength;
	int packetgenrate;
	int maxbuffersize;
	int windowsize;

	if(argv[1][0] == '-' && argv[1][1] == 'd')
	{
		cout << "ywah" << endl;
		debugflag = true;
		packetlength = atoi(argv[7]);
		packetgenrate = atoi(argv[9]);
		maxbuffersize = atoi(argv[15]);		
		MAX_PACKETS = atoi(argv[11]);
		windowsize = atoi(argv[13]);
	}
	else
	{
		//debugflag = true;
		packetlength = atoi(argv[6]);
		packetgenrate = atoi(argv[8]);
		maxbuffersize = atoi(argv[14]);		
		MAX_PACKETS = atoi(argv[10]);
		windowsize = atoi(argv[12]);
	}

	actualbuffer.resize(maxbuffersize);
	bufferavail.resize(maxbuffersize,-1);
	currbufindex.resize(maxbuffersize, -1);

	thread t1(threadtask, maxbuffersize,packetgenrate);


	//t1.join();
	//return 0; 


	//return 0;
	int sockfd; 
	char *hello = "Hello from client"; 
	struct sockaddr_in servaddr; 

	time_t timenow;
	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	
	//int MAX_PACKETS = 100;

	vector <string> buf;

	for(int i = 0; i < MAX_PACKETS; i++)
	{
		string s = decToBinary(i);
		buf.push_back(s);
		//cout << s << endl;
		//usleep(200000);
	}



	//int i = 0;

	//int currwin = 0;
	//int windowsize = 3;

	vector <int> ack(MAX_PACKETS, 0);

	// while(currwin < MAX_PACKETS){
	// 	int n;
	// 	socklen_t len; 
		
	// 	sendto(sockfd, (const char *)&buf[i][0], 32, 
	// 		MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
	// 			sizeof(servaddr)); 
	// 	printf("Hello message sent.\n"); 
			
	// 	n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
	// 				MSG_WAITALL, (struct sockaddr *) &servaddr, 
	// 				&len); 
	// 	buffer[n] = '\0'; 

	// 	int k = stoi(buffer);

	// 	cout << k << endl;
	// 	ack[k] = 1;
	// 	printf("Server : %s\n", buffer); 
	// 	i++;
	// }


	int lastack = -1;
	float a[MAX_PACKETS] = {0.00};
	float timeout = 100;
	float rtt[MAX_PACKETS] = {0.00};
	int sentcount[MAX_PACKETS] = {0};

	socklen_t len;

	auto start = chrono::steady_clock::now();


	while(lastack < MAX_PACKETS-1)
	{

		for(int i = 0; i < MAX_PACKETS; i++)
		{
			cout << rtt[i] << " rttvals" << " " << sentcount[i] << " sentcount" << endl;
			if(sentcount[i] > 5) 
			{		
				int totsent = 0;
				for(int i = 0; i < MAX_PACKETS; i++)
				{
					totsent += sentcount[i];
				}

				int avgrtt = 0;
				for(int i = 0; i < lastack; i++)
				{
					avgrtt += rtt[i];
				}

				avgrtt = avgrtt/lastack;

				t1.join();
				cout << "packetgenrate is" << packetgenrate << "and packet length is : " << packetlength << endl;
				cout << "Total transmissions : " << totsent << " ;number of packets acknowledged : " << lastack+1 << endl; 
				cout << "Average RTT for all acknowledged packets is " << avgrtt << "ms" << endl;
				return 0;
			}
		}

		cout << "going on" << endl;
		int g = windowsize;
		int w = g;
		while(g--)
		{
			//usleep(1000*1000);
			
			cout << "going on1, last ack at " << lastack  << endl;
			if(lastack-g+windowsize < MAX_PACKETS)
			{
				// int e = maxbuffersize;
				// while(e==maxbuffersize)
				// {
				// 	usleep(1000*1000);
				// 	cout << "finding " << lastack-g+windowsize;
				// 	//pthread_mutex_lock(&lockt);
				// 	for(int i = 0; i < maxbuffersize; i++)
				// 	{
				// 		if(currbufindex[i] == lastack-g+windowsize)
				// 		{
				// 			e = i;
				// 			break;
				// 		}
				// 	}

				// 	for(int i = 0; i < maxbuffersize; i++)
				// 	{
				// 		cout << actualbuffer[i] << " " << currbufindex[i] << "hmm" ;
				// 	}
				// 	//pthread_mutex_unlock(&lockt);
				// }

			sendto(sockfd, (const char *)&buf[lastack-g+windowsize][0], 32, 
			MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
			sizeof(servaddr));

			sentcount[lastack - g + windowsize]++;
			cout << "sentcount is " << sentcount[lastack-g+windowsize] << endl;

			//timenow = time(NULL);
			//timeval tv;
			//gettimeofday(&tv,0);
			//cout << tv << endl;

			//cout << timenow << endl;
			//usleep(2*1000);


			auto end = chrono::steady_clock::now();


	// cout << "Elapsed time in milliseconds : " 
	// 	<< chrono::duration_cast<chrono::microseconds>(end - start).count()
	// 	<< " us" << endl;

			a[lastack+windowsize-g] = chrono::duration_cast<chrono::milliseconds>(end - start).count();
			usleep(1000);
			cout << "testing the array" << a[lastack+windowsize-g] << endl;}
		}

		// for(int i = 0; i < MAX_PACKETS; i++)
		// {
		// 	//usleep(100*1000);
		// //	cout << "going on2" << endl;
		// 	cout << a[i]<< " " ;
		// }

		g = w;
		int lastloopack = lastack;
		while(g--)
		{
			//usleep(1000*1000);
			cout << "going on3" << endl;
			memset(buffer, '\0', 32);
			int nim = recvfrom(sockfd, (char *)buffer, 32, 
					MSG_DONTWAIT, (struct sockaddr *) &servaddr, 
					&len);
			cout << nim<<"recvd this"<<endl;
			int nom = stooi();

			if(nom != lastack){
				if(nim != -1){
					/*convert the string into integer*/
					int r = lastack;
					lastack = nom;
					cout << "lastack moved to " << nom << endl;
					while(++r<= lastack)
					{
						pthread_mutex_lock(&lockt);
						for(int i = 0; i < maxbuffersize; i++)
						{
							if(currbufindex[i] == r)
							{
								currbufindex[i] = -1;
								bufferavail[i] = -1;
							}
						}
						pthread_mutex_unlock(&lockt);
					}

					if(debugflag == true)
					{
						cout << "Seq No "<<nom  << "; time generated : "
						<< a[nom] << "; rtt : " << rtt[nom]<< " ; number of attempts"<< sentcount[nom] << endl;
						;
					}
				}

				if(nim != -1 && rtt[lastack] == 0.00)
				{
					auto end = chrono::steady_clock::now();
					rtt[lastack] = chrono::duration_cast<chrono::milliseconds>(end - start).count() - a[lastack];
					cout << "Rtt updated to :" << rtt[lastack] << endl;
					//usleep(3000*1000);
				}

				if(lastack > 9)
					{
						int newtimeout = 0;
						int y = lastack;
						while(y)
						{
							newtimeout += rtt[y];
							y--;
						}
						newtimeout = newtimeout/lastack;
						timeout = 2 * newtimeout;
					}
			//sleep(timeout/windowsize); //why?
			}
		}

			auto end = chrono::steady_clock::now();
			float tt = (float)(chrono::duration_cast<chrono::milliseconds>(end - start).count());
		
		while(tt- a[lastack+1]<= timeout && lastack != MAX_PACKETS-1)
		{  
			
			usleep(1000);
			//cout << end - a[lastloopack+windowsize-1] << " " ;
			//cout << "going on4" << endl;
			memset(buffer, '\0', 32);
			int nim = recvfrom(sockfd, (char *)buffer, 32, 
					MSG_DONTWAIT, (struct sockaddr *) &servaddr, 
					&len);
			int nom = stooi();

			if(nom != lastack){
				if(nim != -1)
				{	
					//int nom = stooi();
					lastack = nom;

					int r = lastack;
					lastack = nom;
					cout << "lastack moved to " << nom << endl;
					while(++r <= lastack)
					{
						pthread_mutex_lock(&lockt);
						for(int i = 0; i < maxbuffersize; i++)
						{
							if(currbufindex[i] == r)
							{
								currbufindex[i] = -1;
								bufferavail[i] = -1;
							}
						}
						pthread_mutex_unlock(&lockt);
					}

					if(debugflag == true)
					{
						cout << "Seq No "<<nom  << "; time generated : "
						<< a[nom] << "; rtt : " << rtt[nom]<< " ; number of attempts"<< sentcount[nom] << endl;
						;
					}


					if(nim != -1 && rtt[lastack] == 0.00)
					{
					auto end = chrono::steady_clock::now();
					rtt[lastack] = chrono::duration_cast<chrono::milliseconds>(end - start).count() - a[lastack];
					cout << "Rtt updated to (4):" << rtt[lastack] << endl;
					//usleep(3000*1000);
					}


						cout << "lastack moved to " << nom << endl;
						if(lastack > 9)
						{
							int newtimeout = 0;
							int y = lastack;
							while(y)
							{
								newtimeout += rtt[y];
								y--;
							}
							newtimeout = newtimeout/lastack;
							timeout = 2 * newtimeout;
						}

					}
				}
			

			int k = 0;
			// for(int i = 0; i < 5; i++)
			// {
			// 	//k = k * 10 + (buffer[i] - 30);
			// 	cout << buffer[i] << " " ;
			// }

			auto end = chrono::steady_clock::now();
			tt = (float)(chrono::duration_cast<chrono::milliseconds>(end - start).count());
			//cout << timeout << " "<<chrono::duration_cast<chrono::milliseconds>(end - start).count() << " " << a[lastack+1] <<" "<< chrono::duration_cast<chrono::milliseconds>(end - start).count()- a[lastack+1] <<endl;
		}

		usleep(1000*1000);
	}
	


	for(int i = 0; i < MAX_PACKETS; i++)
	{
		cout << ack[i] << " " ;
	}
	// string hi = decToBinary(6);
	// for(int i = 0; i < 32; i++)
	// {
	// 	cout << hi[i]<< endl;
	// }

	//close(sockfd); 


	int totsent = 0;
	for(int i = 0; i <= lastack; i++)
	{
		totsent += sentcount[i];
	}

	cout << "Total transmissions : " << totsent << " ;number of packets acknowledged : " << lastack+1 << endl; 
	
	t1.join();
	return 0; 
} 
