#include <bits/stdc++.h>
#include <fstream>

using namespace std;
const int N = 17;	//degree of generating polynomial 		
const int L = 32;	//input size		
const string crcgenerator = "11000000000000101";	


int actual_size_ofstring(bitset<48> in)
{
	for (size_t i = 47; i > 0; --i)
		if (in.test(i)) return i;
	return 0;
}


int main(int argc, char const *argv[])
{
	string inffile = argv[1];
	ifstream myfile;
  	myfile.open(argv[1]);
  	string s;


  	ofstream outfile;
  	outfile.open(argv[2]);

 while(getline(myfile, s)){ //til the infile is exhausted
	bitset<L> input (s); //using bitsets to contain the binary strings

	bitset<L + (N - 1)> sending_stream(input.to_string());
	sending_stream <<= N - 1;


	int i = actual_size_ofstring(sending_stream);
		while (i >=16)
		{
			bitset<L + (N - 1)> xor_initialise;
			xor_initialise |= bitset<xor_initialise.size()>(crcgenerator);
			xor_initialise <<= i - 16;
			sending_stream ^= xor_initialise;				
			i = actual_size_ofstring(sending_stream);
		}
		sending_stream <<= (sending_stream.size() - 16);				
		bitset<16> crcremainder = bitset<16>(sending_stream.to_string());
	

	bitset<L + (N - 1)> crcappendedstring(crcremainder.to_string());
	crcappendedstring |= sending_stream;


	outfile << "Input : " << input << endl;
	outfile << "CRC : " << crcremainder<< endl << endl;

	for(int r = 0; r < 10; r++)
	{
		string modifiedstring = s;
		int ran1 = 1 + 2 * (rand() % 16);
		int ran2 = 1 + 2 * (rand() % 16);
		while(ran2 == ran1)
		{
			ran2 = 1 + 2 * (rand() % 16);
		}
		int ran3 = 1 + 2 * (rand() % 16);
		while(ran3 == ran2 || ran3 == ran1)
		{
			ran3 = 1 + 2 * (rand() % 16);
		}
		//cout << ran1 <<" "<< ran2 <<" " << ran3 << endl;
		if(modifiedstring[ran1]=='1')modifiedstring[ran1]='0';
		else if(modifiedstring[ran1]=='0')modifiedstring[ran1]='1';
		else if(modifiedstring[ran2]=='1')modifiedstring[ran2]='0';
		else if(modifiedstring[ran2]=='0')modifiedstring[ran2]='1';
		else if(modifiedstring[ran3]=='1')modifiedstring[ran3]='0'; 
		else if(modifiedstring[ran3]=='0')modifiedstring[ran3]='1';



		//cout << modifiedstring << "new string";
		outfile << "Original string : "<<s << endl;
		outfile << "Original string with CRC : "<< s<< crcremainder << endl;
		outfile << "Corrupted string : " << modifiedstring << endl;
		outfile << "Number of errors introduced : 3" << endl; 
		bitset<L> inputnew(modifiedstring);
		string crcgnew = "11000000000000101";
		bitset<L + (N - 1)> sending_streamnew(inputnew.to_string());
		sending_streamnew <<= N - 1;

		int i = actual_size_ofstring(sending_streamnew);
			while (i >= N - 1)
			{
				bitset<L + (N - 1)> xor_initialise;
				xor_initialise |= bitset<xor_initialise.size()>(crcgnew);
				xor_initialise <<= i - 16;
				sending_streamnew ^= xor_initialise;			
				i = actual_size_ofstring(sending_streamnew);
			}
			sending_streamnew <<= (sending_streamnew.size() - (16));				
			bitset<16> crcnew = bitset<16>(sending_streamnew.to_string());


			//cout << crcnew << "ooh" << endl;

			if(crcnew != crcremainder)
			{	
				//cout << "Failed" << endl;
				outfile << "CRC check : Failed"<<endl << endl;
			}
			else
			{
				outfile << "CRC check : Passed"<<endl << endl;
			}
	}

	for(int r = 0; r < 10; r++)
	{
		//introduce 10 even number of mutations and check if the crcremainder helps to detect them
		string modifiedstring = s;
		int ran1 = 1 + 2 * (rand() % 16);
		int ran2 = 1 + 2 * (rand() % 16);
		while(ran2 == ran1)
		{
			ran2 = 1 + 2 * (rand() % 16);
		}

		if(modifiedstring[ran1]=='1')modifiedstring[ran1]='0';
		else if(modifiedstring[ran1]=='0')modifiedstring[ran1]='1';
		else if(modifiedstring[ran2]=='1')modifiedstring[ran2]='0';
		else if(modifiedstring[ran2]=='0')modifiedstring[ran2]='1';
		



		outfile << "Original string : "<<s << endl;
		outfile << "Original string with CRC : "<< s << crcremainder<< endl;
		outfile << "Corrupted string : " << modifiedstring << endl;
		outfile << "Number of errors introduced : 2" << endl; 

		//cout << modifiedstring << "new string";
		bitset<L> inputnew(modifiedstring);
		string crcgnew = "11000000000000101";
		bitset<L + (N - 1)> sending_streamnew(inputnew.to_string());
		sending_streamnew <<= N - 1;

		int i = actual_size_ofstring(sending_streamnew);
			while (i >= 16)
			{
				bitset<L + (N - 1)> xor_initialise;
				xor_initialise |= bitset<xor_initialise.size()>(crcgnew);
				xor_initialise <<= i - 16;
				sending_streamnew ^= xor_initialise;				
				i = actual_size_ofstring(sending_streamnew);
			}
			sending_streamnew <<= (sending_streamnew.size() - 16);			
			bitset<16> crcnew = bitset<16>(sending_streamnew.to_string());


			//cout << crcnew << "ooh" << endl;

			if(crcnew != crcremainder)
			{	
				//cout << "Failed" << endl;
				outfile << "CRC check : Failed"<<endl << endl;
			}
			else
			{
				outfile << "CRC check : Passed"<<endl << endl;
			}
	}


	for(int r = 0; r < 10; r++)
	{
		//introduce 10 bursty mutations (length = 10) and check if the crcremainder helps to detect them
		int rands = rand() % 20;
		string modifiedstring = s;
		for(int i = rands; i < rands + 10; i++)
		{
			if(modifiedstring[i] == '1')
				modifiedstring[i] = '0';
			else
				modifiedstring[i] = '1';
		}

		outfile << "Original string : "<<s << endl;
		outfile << "Original string with CRC : "<< s<< crcremainder<< endl;
		outfile << "Corrupted string : " << modifiedstring << endl;
		outfile << "Number of errors introduced : 10" << endl; 

		//cout << modifiedstring << "new string";
		bitset<L> inputnew(modifiedstring);
		string crcgnew = "11000000000000101";
		bitset<L + (N - 1)> sending_streamnew(inputnew.to_string());
		sending_streamnew <<= N - 1;

		int i = actual_size_ofstring(sending_streamnew);
			while (i >= 16)
			{
				bitset<L + (N - 1)> xor_initialise;
				xor_initialise |= bitset<xor_initialise.size()>(crcgnew);
				xor_initialise <<= i - 16;
				sending_streamnew ^= xor_initialise;				
				i = actual_size_ofstring(sending_streamnew);
			}
			sending_streamnew <<= (sending_streamnew.size() - 16);					
			bitset<16> crcnew = bitset<16>(sending_streamnew.to_string());
		

			if(crcnew != crcremainder)
			{	
				//cout << "Failed" << endl;
				outfile << "CRC check : Failed"<<endl << endl;
			}
			else
			{
				outfile << "CRC check : Passed"<<endl << endl;
			}

	}

	outfile << endl << endl;
   }
	return 0;
}
