#include<iostream>
#include<vector>
#include<string>
#include<utility>
#include<fstream>
#include<cstdlib>
#include<cmath>

using namespace std; 


class Bucket{
	public:
	int start; 
	int end; 
	int count;

	Bucket(int startI,int endI, int countCurI):start{startI},end{endI},count{countCurI}{};
};



class FindMummer{
	string a; 
	string b; 

	string file1; 
	string file2; 


	string txt;
	vector<int> sa;


	int createSA();
	int printBucket(vector<Bucket> &vBucket);
	int printSA(vector<int> sa);
	int cleanBucketCounter(vector<Bucket> &bBucket);
	int breakBucket(vector<Bucket> &vBucket,vector<int> &bucketIndex,const vector<int> &curSA,int h);
	int compare(int firstString,int secondString,int h);	
	int checkSingleton(const vector<Bucket> &vBucket);
	int printBucketIndex(const vector<int> bucketIndex);

	public:
	FindMummer(string file1I, string file2I):file1{file1I},file2{file2I}{};
	int read();
	pair<int,int> run();
};



int FindMummer::printBucket(vector<Bucket> &vBucket)
{
	int i = 0;
	for(vector<Bucket>::iterator itr = vBucket.begin(); itr != vBucket.end(); itr++)
	{
		cout<<"Bucket iter "<<i<<" "<<itr->start<<" "<<itr->end<<" "<<itr->count<<endl;
		i++;
	}
}

int FindMummer::printSA(vector<int> sa)
{
	for(int i = 0 ; i < sa.size(); i++)
	{
		cout<<txt[sa[i]]<<" "<<sa[i]+1<<endl;
	}
	return 1; 
}

int FindMummer::printBucketIndex(const vector<int> bucketIndex)
{
	for(int i = 0 ; i < bucketIndex.size(); i++)
	{
		cout<<"b "<<txt[i]<<" "<<bucketIndex[i]<<endl;
	}
	return 1; 
}

int FindMummer::cleanBucketCounter(vector<Bucket> &vBucket)
{
	for(vector<Bucket>::iterator itr = vBucket.begin() ; itr != vBucket.end(); itr++)
	{
		itr->count = 0 ; 
	}	
	return 1; 
}


int FindMummer::checkSingleton(const vector<Bucket> &vBucket)
{
	for(vector<Bucket>::const_iterator itr = vBucket.begin() ; itr != vBucket.end() ; itr++)
	{
		if( itr->start != itr->end)
			return 0;
	}
	return 1; 
	
}



int FindMummer::compare(int firstString,int secondString,int h)
{
	string first = txt.substr(firstString,h);
	string second = txt.substr(secondString,h);

	if( first == second)
		return 1;
	else
		return 0; 
}

int FindMummer::breakBucket(vector<Bucket> &vBucket,vector<int> &bucketIndex,const vector<int> &curSA,int h)
{
	int firstString = curSA[0];
	for(int i = 0 ; i < curSA.size() ; i++)
	{
		int secondString = curSA[i];
		cout<<firstString<<endl;

		int bucIF = bucketIndex[firstString];
		int bucIF2 = bucketIndex[secondString];

		if( bucIF == bucIF2)
		{

			int result = compare(firstString,secondString,h);
		
			if(result != 1)	//break the bucket
			{
				int tempEnd = vBucket[bucIF].end;
				vBucket[bucIF].end = vBucket[bucIF].start + vBucket[bucIF].count;

				//create a new bucket
				Bucket newBucket(vBucket[bucIF].end+1,tempEnd,1);
				bucketIndex[secondString] = vBucket.size();
				vBucket.push_back(newBucket);
			}
			else
			{
				vBucket[bucIF].count++;	
			}
		}
		firstString = curSA[i];
	}	
}

int FindMummer::createSA()
{
	txt = "aabaaabbaacda$";

	vector<Bucket> vBucket;
	vector<int> bucketIndex(txt.length(),0);
	
	vector<int> curSA(txt.length(),0);
	vector<int> prevSA(txt.length(),0);

	char start = ' ';
	char end = '~';

	//fill up the first iteration of bucket
	

	//fill up hash buckets
	for(char i = start ; i <= end ; i++)
	{
		Bucket bucketTemp(0,0,0);
		vBucket.push_back(bucketTemp);
	}
	

	for(int i = 0 ; i < txt.length(); i++)
	{
		vBucket[txt[i]-start].count++;
		bucketIndex[i] = txt[i]-start; 	
	}

#ifdef DEBUG
	for(int i = 0 ; i < vBucket.size(); i++)
	{
		cout<<"size "<<vBucket[i].count<<endl;
	}
#endif
	//update the buckets
	int tot = 0;
	for(int i = 0 ; i < vBucket.size(); i++)
	{
		if( vBucket[i].count != 0)
		{
			vBucket[i].start = tot;
			vBucket[i].end = tot + vBucket[i].count -1 ;
			tot = vBucket[i].end+1;
		}
		//cout<<"Start "<<vBucket[i].start<<" "<<vBucket[i].end<<" "<<vBucket[i].count<<endl;
		vBucket[i].count = 0; 
	}
	//cout<<"txt len "<<txt.length()<<endl;

	for(int i = 0 ; i < txt.length() ; i++)
	{
		curSA[ vBucket[bucketIndex[i]].start+vBucket[bucketIndex[i]].count] = i;
		//cout<<"sa: "<<txt[i]<<" "<<curSA[i]<<endl;
		vBucket[bucketIndex[i]].count++;	
	}

	//printBucket(vBucket);
	printSA(curSA);
	//exit(1);
	//cleanCounter();
	

	//initial SA has been set now follow the algorithm to generate the required final SA
	
	int allSingleton = 0;
	int k = 0; 

	printBucketIndex(bucketIndex);

	while(allSingleton != 1)
	{
		prevSA = curSA;
		
		int h = pow(2,k);
		k++;
		cleanBucketCounter(vBucket);
			
		for(int i = 0 ; i < curSA.size() ; i++)
		{
			int j = prevSA[i];
			cout<<" j "<<j<<endl;

			if( vBucket[bucketIndex[j]].start == vBucket[bucketIndex[j]].end)
			{
				curSA[i] = j; 	
			}

			if((j - h) > 0)
			{
				int bucI = bucketIndex[j-h];
				int p =  vBucket[bucI].start + vBucket[bucI].count;
				curSA[p] = j - h ; 
				vBucket[bucI].count++;	
				
				cout<<"bucI "<<bucI<<" p "<<p<<" j-h "<<j-h<<" txt "<<txt[j-h]<<endl;
			}
			//else
			//{
				//int bucI = bucketIndex[j];
				//curSA[i] = vBucket[bucI].start + vBucket[bucI].count;
				//vBucket[bucI].count++;
			//}
		}
		printSA(curSA);
		exit(1);

		cleanBucketCounter(vBucket);
		breakBucket(vBucket,bucketIndex,curSA,h);
		allSingleton = checkSingleton(vBucket);
		printBucket(vBucket);
		printSA(curSA);

				
	}
	
}



pair<int,int> FindMummer::run()
{
	//create suffix array	
	createSA();
}



int FindMummer::read()
{
	string line; 
	ifstream file1D(file1.c_str());
	
	if(file1D.is_open())
	{
		int count = 0;
		while(getline(file1D,line))
		{
			//cout<<line<<endl;
			if(count != 0)
				a += line;
			count++;
		}
		file1D.close();
	}
	else
	{
		cout<<"Unable to open file"<<file1<<endl; 
	}

#ifdef DEBUG
	cout<<"Final: "<<a<<endl;
#endif
	
	ifstream file2D(file2.c_str());
	if(file2D.is_open())
	{
		int count = 0;
		while(getline(file2D,line))
		{
			//cout<<line<<endl;
			if(count != 0)
				b += line;
			count++;
		}
		file2D.close();
	}
	else
	{
		cout<<"Unable to open file"<<file1<<endl; 
	}

#ifdef DEBUG
	cout<<b<<endl;
#endif

}







int main(int argc, char **argv)
{
	if(argc != 3)
	{
		cout<<"Usage: "<<argv[0]<<" file1 file2"<<endl;
		exit(1);
	}

	FindMummer findMummer(argv[1],argv[2]);
	findMummer.read();
	findMummer.run();
}
