#include<iostream>
#include<vector>
#include<string>
#include<utility>
#include<fstream>
#include<cstdlib>
#include<cmath>
#include<algorithm>
#include<cstdio>


using namespace std; 


class Bucket{
	public:
	int start; 
	int end; 
	int count;

	Bucket(int startI,int endI, int countCurI):start{startI},end{endI},count{countCurI}{};
};

class BinaryNode{
	public:
	int start;
	int end;
	int lcp;

	BinaryNode(int startI, int endI, int lcpI):start{startI},end{endI},lcp{lcpI}{};
};


class FindPattern{
	string a; 
	string file1; 
	
	string txt;
	vector<int> SA;
	vector<int> R;
	vector<int> LCP;
	vector<BinaryNode> lcpBinaryTree;


	int createSA();
	int printBucket(vector<Bucket> &vBucket);
	int printSA(vector<int> sa);
	int cleanBucketCounter(vector<Bucket> &bBucket);
	int breakBucket(vector<Bucket> &vBucket,vector<int> &bucketIndex,const vector<int> &curSA,int h);
	int compare(int firstString,int secondString,int h);	
	int checkSingleton(const vector<Bucket> &vBucket);
	int printBucketIndex(const vector<int> bucketIndex);
	int createLcpFromSA();
	int createRFromSA();
	int getCompLcp( int a, int b);
	int getCompLcp(int start,string pat,int idx); 
	int createLcpBinaryTree(int start, int end, int index);
	int printLcpBinaryTree();
	int findTreeSize(int len);
	vector<int> findRecurse(int start, int end, int l, int r,int index, string pat);	

	public:
	FindPattern(string file1I):file1{file1I}{};
	int read();
	int createSaAndLcp();
	int find(string pat);
};


int FindPattern::findTreeSize(int len)
{
	int pow = 2;
	for(int i = 1 ; i < 100 ; i++)
	{
		if( len < pow)
		{
			return 2*pow;
		}
		pow = pow*2;
	}
}

int FindPattern::printBucket(vector<Bucket> &vBucket)
{
	int i = 0;
	for(vector<Bucket>::iterator itr = vBucket.begin(); itr != vBucket.end(); itr++)
	{
		cout<<"Bucket iter "<<i<<" "<<itr->start<<" "<<itr->end<<" "<<itr->count<<endl;
		i++;
	}
}

int FindPattern::printSA(vector<int> sa)
{
	for(int i = 0 ; i < sa.size(); i++)
	{
		//printf("%s ",txt.c_str()[sa[i]]);
		cout<<i<<" "<<txt[sa[i]]<<" "<<sa[i]<<" ";
		for( int j = sa[i]; j < txt.size(); j++)
		{
		//	cout<<txt[j];
		}
		cout<<endl;
	}
	return 1; 
}

int FindPattern::printBucketIndex(const vector<int> bucketIndex)
{
	for(int i = 0 ; i < bucketIndex.size(); i++)
	{
		cout<<"b "<<txt[i]<<" "<<bucketIndex[i]<<endl;
	}
	return 1; 
}

int FindPattern::cleanBucketCounter(vector<Bucket> &vBucket)
{
	for(vector<Bucket>::iterator itr = vBucket.begin() ; itr != vBucket.end(); itr++)
	{
		itr->count = 0 ; 
	}	
	return 1; 
}


int FindPattern::checkSingleton(const vector<Bucket> &vBucket)
{
	for(vector<Bucket>::const_iterator itr = vBucket.begin() ; itr != vBucket.end() ; itr++)
	{
		if( itr->start != itr->end)
			return 0;
	}
	return 1; 
	
}

int FindPattern::getCompLcp(int start,string pat,int idx)
{
	int count = 0 ; 
	for(int i = 0 ; start+idx+i < txt.size() && idx+i < pat.size(); i++)
	{
		if( txt[start+idx+i] == pat[idx+i])
			count++;
		else
			return count;
	}
	return count; 
}

vector<int> FindPattern::findRecurse(int start, int end, int l, int r,int index, string pat)
{

	cout<<"Start "<<start<<" end "<<end<<" l "<<l<<" r "<<r<<" index "<<index<<" pat "<<pat<<endl;

	if( l == pat.size() && r == pat.size())
	{
		vector<int> result;
		result.push_back(start);
		result.push_back(end);
		return result; 
	}

	if( end == start + 1)
	{
		vector<int> result;
		if( l == pat.size())
		{
			result.push_back(start);
			result.push_back(start);
		}
		else if( r == pat.size())
		{
			result.push_back(end);
			result.push_back(end);
		
		}
		else
		{
			result.push_back(-1);	
			result.push_back(-1);	
		}
		return result;

	}

	int mid = start + (end - start)/2; 
	if( l == r)
	{
		int m = getCompLcp(SA[mid],pat,l);

		cout<<start<<" sa[mid] "<<mid<<" "<<end<<" l "<<l<<" m "<<m<<" vs "<<pat[l+m] <<" vs "<<txt[SA[mid] + l] <<endl;

		if( l+m == pat.size() )
		{
			cout<<"Doing both "<<start<<" "<<mid<<" "<<end<<endl;
			//do both size
			vector<int> res1 = findRecurse(start,mid,l,l+m,2*index+1,pat);
			vector<int> res2 = findRecurse(mid,end,l+m,r,2*index+2, pat);

			cout<<" Res: "<<res1[0]<<" "<<res1[1]<<" "<<res2[0]<<" "<<res2[1]<<endl;

			vector<int> result;
			result.push_back(res1[0]);	
			result.push_back(res2[1]);
			return result;	

		}
		if( pat[l+m] > txt[SA[mid]+l+m])
		{
			return findRecurse(mid,end,l+m,r,2*index+2, pat);
		}
		else
		{
			return findRecurse(start,mid,l,l+m,2*index+1,pat);	
		}
	}
	else if( l > r)
	{
		if( l < lcpBinaryTree[2*index+1].lcp)
		{
			//we are going to right side
			return findRecurse(mid,end,l,r,2*index+2, pat);
		}
		else if( l > lcpBinaryTree[2*index+1].lcp)
		{
			//we are going to the left side	
			return findRecurse(start,mid,l,lcpBinaryTree[2*index+1].lcp,2*index+1,pat);	
		}
		else
		{
			//equal we have to do a lexicographic comparison
			int m = getCompLcp(SA[mid],pat,l);
			cout<<"Doing  "<<start<<" "<<mid<<" "<<end<<" "<<l+m<<" "<<pat.size()<<endl;
	
			if( l+m == pat.size() )
			{
				cout<<"Doing both "<<start<<" "<<mid<<" "<<end<<endl;
				//do both size
				vector<int> res1 = findRecurse(start,mid,l,l+m,2*index+1,pat);
				vector<int> res2 = findRecurse(mid,end,l+m,r,2*index+2, pat);
			
				cout<<" Res: "<<res1[0]<<" "<<res1[1]<<" "<<res2[0]<<" "<<res2[1]<<endl;

				vector<int> result;
				result.push_back(res1[0]);	
				result.push_back(res2[1]);
				return result;	

			}

			if( pat[l+m] > txt[SA[mid]+l+m])
			{
				return findRecurse(mid,end,l+m,r,2*index+2, pat);
			}
			else
			{
				return findRecurse(start,mid,l,l+m,2*index+1,pat);	
			}
		}	
	}
	else if( l < r)
	{
		cout<<" l < r "<<lcpBinaryTree[2*index+2].lcp<<endl;
		if( r < lcpBinaryTree[2*index+2].lcp)
		{
			//we will go to the left side
			return findRecurse(start,mid,l,r,2*index+1,pat);	
		}
		else if( r > lcpBinaryTree[2*index+2].lcp)
		{
			//we will go to the right side	
			return findRecurse(mid,end, lcpBinaryTree[2*index+2].lcp,r,2*index+2, pat);
		}
		else
		{
			//equal we have to do a lexicographic comparison
			int m = getCompLcp(SA[mid],pat,r);
			cout<<"Doing  "<<start<<" "<<mid<<" "<<end<<" "<<r+m<<" "<<pat.size()<<" b"<<endl;

			if( r+m == pat.size() )
			{
				cout<<"Doing both "<<start<<" "<<mid<<" "<<end<<endl;
				//do both size
				vector<int> res1 = findRecurse(start,mid,l,r+m,2*index+1,pat);
				vector<int> res2 = findRecurse(mid,end,r+m,r,2*index+2, pat);
				
				cout<<" Res: "<<res1[0]<<" "<<res1[1]<<" "<<res2[0]<<" "<<res2[1]<<endl;

				vector<int> result;
				result.push_back(res1[0]);	
				result.push_back(res2[1]);	
				return result;	

			}



			cout<<" m "<<m<<endl;
			if( pat[r+m] > txt[SA[mid]+r+m])
			{
				return findRecurse(mid,end,r+m,r,2*index+2, pat);
			}
			else
			{
				return findRecurse(start,mid,l,r+m,2*index+1,pat);	
			}
		
		}	
	}
	else
	{
		cout<<"All conditions taken care of this should not print"<<endl;
	}
}


int FindPattern::find(string pat)
{
	// manber and myers 1991 algorithm to search patter in SA using LCP array
	int start = 0;
	int end = txt.size()-1;

	//calculate lcp for pat and start 
	//calculate lcp for pat and end
	
	int l = getCompLcp(SA[start],pat,0);
        int r = getCompLcp(SA[end],pat,0);

	//cout<<"R "<<r<<" l "<<l<<endl;
        
	vector<int> result = findRecurse(start, end, l, r, 0, pat);	

	cout<<" result "<<result[0]<<" to "<<result[1]<<endl;

	cout<<"Pattern found at the following places: "<<endl;
	for(int i = result[0] ; i <= result[1] ; i++)
	{
		cout<<SA[i]<<endl;
	}
	
}

int FindPattern::compare(int firstString,int secondString,int h)
{
	string first = txt.substr(firstString,h);
	string second = txt.substr(secondString,h);
	//cout<<first<<" vs "<<second;

	if( first == second)
		return 1;
	else
		return 0; 
}


int FindPattern::createRFromSA()
{
	vector<int> rTemp(txt.length(),0);
	for(int i = 0; i < SA.size(); i++)
	{
		rTemp[SA[i]] = i; 
	}
	R = rTemp;

}
	
int FindPattern::getCompLcp( int a, int b)
{
	//cout<<"a "<<a<<" vs "<<b<<endl;
	int lcp = 0;
	for(int i = 0 ; a+i < txt.size() && b+i < txt.size();i++)
	{
		if( txt[a+i] == txt[b+i])
		{
			lcp++;
		}
		else
		{
			break;
		}
	}
	return lcp;
}


int FindPattern::createLcpFromSA()
{
#if DEBUG
	for(int i = 0 ; i < SA.size() ; i++)
		cout<<"LCP "<<SA[i]<<endl;

	for(int i = 0 ; i < R.size() ; i++)
		cout<<"R "<<txt[i]<<" "<<R[i]+1<<endl;

#endif
	vector<int> lcpTemp(txt.size()-1,0);

	lcpTemp[ R[0] ] = getCompLcp( 0, SA[R[0]+1]);
	//cout<<"lcpTemp "<<SA[R[0]+1]<<" "<<lcpTemp[R[0]]<<" R[0] "<<R[0]<<endl;

	for(int i = 0 ; i < txt.size()-1; i++)
	{
		int l = lcpTemp[R[i]];
		int k = 0;
		if(l > 1)
			k = l - 1;

		int comp = getCompLcp(i+1+k,  SA[R[i+1]+1]+k); 	
		lcpTemp[R[i+1]] = k + comp; 
		//cout<<lcpTemp[R[i+1]]<<" "<<i+1<<" "<<SA[R[i+1]+1]<<" "<<l<<" comp "<<comp<<endl;
	}

//#ifdef DEBUG
	for(int i = 0 ; i < txt.size()-1; i++)
	{
		cout<<"lcp "<<lcpTemp[i]<<endl;
	}
//#endif
	LCP = lcpTemp;	
	return 1; 	
}


int FindPattern::breakBucket(vector<Bucket> &vBucket,vector<int> &bucketIndex,const vector<int> &curSA,int h)
{
	int vBucketSize = vBucket.size();
	for(int j = 0 ; j < vBucketSize ; j++)
	{
#if DEBUG
		cout<<"j = "<<j<<" start "<<vBucket[j].start<<" end "<<vBucket[j].end<<" "<<vBucket[j].count<<endl;
#endif
		
		int firstString = curSA[ vBucket[j].start ];
		int start = vBucket[j].start;
		int end = vBucket[j].end;

		for( int i = start ; i <= end && i != -1; i++)
		{
#if DEBUG
			cout<<"I = "<<i<<" start "<<start;
#endif
			int secondString = curSA[i];
		
			int bucIF = bucketIndex[firstString];
			int bucIF2 = bucketIndex[secondString];

#if DEBUG
			cout<<" "<<bucIF<<" comparing ";
#endif
			int result = compare(firstString,secondString,h);

#if DEBUG
			cout<<" Result "<<result<<endl;
#endif
			if(result != 1)
			{
				int tempEnd = vBucket[bucIF].end;
				vBucket[bucIF].end = vBucket[bucIF].start + vBucket[bucIF].count - 1;

				//create a new bucket
				Bucket newBucket(vBucket[bucIF].end+1,tempEnd,1);
				bucketIndex[secondString] = vBucket.size();
				vBucket.push_back(newBucket);
				
			}
			else
			{
				bucketIndex[secondString] = bucIF;
				vBucket[bucIF].count++;	
			}
			
			//cout<<"j = "<<j<<" "<<vBucket[j].start<<" "<<vBucket[j].end<<" "<<vBucket[j].count<<endl;
			firstString = curSA[i];
		}	
	}	
}


int FindPattern::createSA()
{
	//txt = "aabaaabbaacda$";
	//txt = "MISSISSIPPI$";
	//txt = "TTAATTTTAGAAATACAGGTTTCTAAAACGCTTTTATGCGGCTCGCCCTCATAGCCAAAACTCGCATGCA$";
	txt = a+"$";

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
		Bucket bucketTemp(-1,-1,0);
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

#if DEBUG
	//printBucket(vBucket);
	//printSA(curSA);
	//exit(1);
	//cleanCounter();
#endif
	

	//initial SA has been set now follow the algorithm to generate the required final SA
	
	int allSingleton = 0;
	int k = 0; 
#if DEBUG
	printBucketIndex(bucketIndex);
#endif

	while(allSingleton != 1)
	{
		prevSA = curSA;
		
		int h = pow(2,k);
		k++;
		cleanBucketCounter(vBucket);
			
		for(int i = 0 ; i < curSA.size() ; i++)
		{
			int j = prevSA[i];

#if DEBUG
			cout<<" j "<<j<<endl;
#endif

			if( vBucket[bucketIndex[j]].start == vBucket[bucketIndex[j]].end)
			{
				curSA[i] = j; 	
			}

			if((j - h) >= 0)
			{
				int bucI = bucketIndex[j-h];
				int p =  vBucket[bucI].start + vBucket[bucI].count;
				curSA[p] = j - h ; 
				vBucket[bucI].count++;	
				
#if DEBUG
				cout<<j<<" bucI "<<bucI<<" p "<<p<<" j-h "<<j-h<<" txt "<<txt[j-h]<<endl;
#endif
			}
		}

#if DEBUG
		printSA(curSA);
		printBucket(vBucket);
#endif
		cleanBucketCounter(vBucket);
		breakBucket(vBucket,bucketIndex,curSA,2*h);
		allSingleton = checkSingleton(vBucket);
#if DEBUG
		printBucket(vBucket);
		printSA(curSA);
#endif
		//exit(1);
	}

	SA = curSA;
	printSA(SA);	
}

int FindPattern::createLcpBinaryTree(int start, int end, int index)
{
	//BinaryNode
	//cout<<"Start "<<start<<" End "<<end<<" Index "<<index<<endl;
	if(end == start+1)
	{
		BinaryNode binaryNode(start,end, LCP[start]);
		lcpBinaryTree[index] = binaryNode;
		return LCP[start];
	}

	int lcpL = createLcpBinaryTree(start,start+(end-start)/2, 2*index + 1);
	int lcpR = createLcpBinaryTree(start+(end-start)/2,end, 2*index + 2);

	int lcp = min(lcpL,lcpR);
	BinaryNode binaryNode(start,end,lcp);
	lcpBinaryTree[index] = binaryNode;
	return lcp;
}

int FindPattern::printLcpBinaryTree()
{
	for(int i = 0 ; i < lcpBinaryTree.size(); i++)
	{
		cout<<"Start "<<lcpBinaryTree[i].start<<" End "<<lcpBinaryTree[i].end<<" lcp "<<lcpBinaryTree[i].lcp<<endl;
	}
}

int FindPattern::createSaAndLcp()
{
	//create suffix array	
	createSA();

	int size = findTreeSize(txt.size());
	cout<<"Size "<<size<<" txt.size "<<txt.size()<<endl;
	vector<BinaryNode> binTreeTemp(size,{0,0,0});

	createRFromSA();
	createLcpFromSA();
	lcpBinaryTree = binTreeTemp;
	createLcpBinaryTree(0,SA.size()-1,0);
	//printLcpBinaryTree();
}



int FindPattern::read()
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
			else
				a = line;
			count++;
		}
		file1D.close();
	}
	else
	{
		cout<<"Unable to open file"<<file1<<endl; 
	}

//#ifdef DEBUG
	cout<<"Final: "<<a<<endl;
//#endif
}



int main(int argc, char **argv)
{
	if(argc != 2)
	{
		cout<<"Usage: "<<argv[0]<<" file1 "<<endl;
		exit(1);
	}
	
	FindPattern findPattern(argv[1]);
	findPattern.read();
	findPattern.createSaAndLcp();

	string pattern;

	while(true)
	{
		cout<<"Enter Pattern: ";
		cin>>pattern;
		findPattern.find(pattern);
	}
	

}
