#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <chrono>

using namespace std;

vector <int> w;
vector <int> backoff;
vector <int> buffer_entries;
vector <int> currtransmissions;
vector <int> buffers;
vector <pair<float, float> >timeofgen;
int main(int argc, char const *argv[]){

	int N = atoi(argv[2]);

	int W = atoi(argv[4]); //what does default mean here?

	float packetgenrate = atof(argv[6]);

	int MAX_PACKETS = atoi(argv[8]);
	
	int simtime = 0;

	w.resize(N, W);
	backoff.resize(N,0);
	buffer_entries.resize(N,0);
	timeofgen.resize(N, make_pair(0,0));
	currtransmissions.resize(N,0);
	buffers.resize(N,0);


	for(int i = 0; i < N; i++)
	{
		backoff[i] = rand() % w[i];
	}

	int packetssent = 0;
	float total_waittime_cumulative = 0.00;

//also terminate if maximum retransimssions for any packet exceeds 10
	auto start = chrono::steady_clock::now();
	while(packetssent <= MAX_PACKETS )
	{

		//usleep(2000 * 1000);
		int transmissionsnow = 0;
		int firstuser = -1;

		cout << endl<< endl<< endl<< endl;
		cout << "at time t : " << simtime << endl;


		cout << "The Buffers are : ";
		for(int i = 0; i <N; i++)
		{
			cout << buffers[i]<<" ";
		}
		cout <<endl;




		cout << "The backoffs are : ";
		for(int i = 0; i <N; i++)
		{
			cout << backoff[i]<<" ";
		}
		cout <<endl;



		for(int i = 0; i < N; i++)
		{
			if(backoff[i] == 0)
			{
				if(buffers[i] == 0 && (rand()%10000)/10000.0000 < packetgenrate)
				{
					buffers[i] = 1;
					cout << i << " made a packet " << endl;
					auto end = chrono::steady_clock::now();
					timeofgen[i] = make_pair(chrono::duration_cast<chrono::milliseconds>(end - start).count(),0);
					//rtt[nom] = chrono::duration_cast<chrono::milliseconds>(end - start).count() - a[nom];
				}	
				else if(buffers[i] == 1 && (rand()%10)/10 < packetgenrate)
				{
					buffers[i] = 2;
					cout << i << " made a packet " << endl;

					auto end = chrono::steady_clock::now();
					timeofgen[i] = make_pair(timeofgen[i].first,chrono::duration_cast<chrono::milliseconds>(end - start).count());
				}
				if(buffers[i] != 0)
				{	
					cout<<" "<<i<< " transmitted "<< endl;
					currtransmissions[i] = 1;
					transmissionsnow ++;
					if(transmissionsnow == 1)
					{
						firstuser = i;
					}
					//buffers[i] --;
				}
			}
			cout << endl;
		}

		if(transmissionsnow == 1)
		{
			backoff[firstuser] = 0;
			int nearestint =  w[firstuser]*0.75;
			w[firstuser] = max(2,nearestint);
			cout << "transmission of user : "<< firstuser << " is successful at time : "<< simtime <<endl;
			packetssent++;

			if(buffers[firstuser] == 1)
			{
				total_waittime_cumulative += timeofgen[firstuser].first;
				timeofgen[firstuser] = make_pair(0.00,0.00);
			}
			else if(buffers[firstuser] == 2)
			{
				total_waittime_cumulative += timeofgen[firstuser].first;
				timeofgen[firstuser] = make_pair(timeofgen[firstuser].second,0.00);
			}
			buffers[firstuser]--;

		}

		else
		{
			for(int i = 0; i < N; i++)
			{
				if(currtransmissions[i] == 1)
				{
					w[i] = min(w[i] * 2, 256);
					backoff[i] = rand() % w[i];
				}
			}
			cout << "all the users at time : " << simtime << " failed" << endl;
		}


		for(int i = 0; i < N; i++)
		{
			if(backoff[i] > 0)
			{
				backoff[i]--;
			}
		}

		simtime ++;
		fill(currtransmissions.begin(), currtransmissions.end(), 0);
	}

	cout <<"Number of users : " <<N <<endl <<"W : "<< W<<endl <<"Packet generation rate : "<< packetgenrate << endl;
	cout << "Utilization : "<< packetssent * 1.00/simtime << endl;
	cout << "Average Packet Delay : " << total_waittime_cumulative * 1.00000 / packetssent << endl;

	return 0;
}