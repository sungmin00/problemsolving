#include <cstdio>
#include <vector>
#include <algorithm>
#include <map>
int MAX = 1000000000;
int MIN = -1000000000;
using namespace std; 
int c1,c2,n1,n2,base,n;
map<int,int> x1,y1; 
vector<int> x,y;
int tranx[2048],trany[2048],mp[2048][4048];
vector<int> lazy,max_tree,min_tree; 
vector<pair<pair<int,int>,int>> temp;
void query_update(int st, int ed , int value)
{
	st+=base; 
	ed+=base;
	lazy[st]+=value;
	if(st!=ed)lazy[ed]+=value;  
	while(st/2!=ed/2)
	{
		if(st%2==0)lazy[st+1]+=value; 
		if(ed%2==1)lazy[ed-1]+=value;
		int temp_st=(st/2)*4+1-st; 
		int temp_ed=(ed/2)*4+1-ed;
		max_tree[st/2]=max(max_tree[st]+lazy[st],max_tree[temp_st]+lazy[temp_st]);
		max_tree[ed/2]=max(max_tree[ed]+lazy[ed],max_tree[temp_ed]+lazy[temp_ed]);
		min_tree[st/2]=min(min_tree[st]+lazy[st],min_tree[temp_st]+lazy[temp_st]);
		min_tree[ed/2]=min(min_tree[ed]+lazy[ed],min_tree[temp_ed]+lazy[temp_ed]);
		st/=2; 
		ed/=2;  
	}
	while(ed>=2)
	{
		int temp_ed=(ed/2)*2+1-ed;
		max_tree[ed/2]=max(max_tree[ed]+lazy[ed],max_tree[temp_ed]+lazy[temp_ed]);
		min_tree[ed/2]=min(min_tree[ed]+lazy[ed],min_tree[temp_ed]+lazy[temp_ed]);
		ed/=2; 
	}
	
}
int query_max(int st, int ed)
{
	st+=base; 
	ed+=base; 
	int maxl,maxr,max_tot;
	maxl=max_tree[st];
	maxr=max_tree[ed];  
	while(st/2!=ed/2)
	{
		maxl+=lazy[st];
		maxr+=lazy[ed]; 
		if(st%2==0)maxl=max(maxl,max_tree[st+1]+lazy[st+1]);
		if(ed%2==1)maxr=max(maxr,max_tree[ed-1]+lazy[ed-1]); 
		st/=2; 
		ed/=2; 
	}
	maxl+=lazy[st];
	maxr+=lazy[ed]; 
	ed/=2;  
	max_tot = max(maxl,maxr); 
	while(ed>=1)
	{
		max_tot+=lazy[ed]; 
		ed/=2; 
	}
	return max_tot; 
}
int query_min(int st, int ed)
{
	st+=base; 
	ed+=base; 
	int minl,minr,min_tot;
	minl=min_tree[st];
	minr=min_tree[ed];  
	while(st/2!=ed/2)
	{
		minl+=lazy[st];
		minr+=lazy[ed]; 
		if(st%2==0)minl=min(minl,min_tree[st+1]+lazy[st+1]);
		if(ed%2==1)minr=min(minr,min_tree[ed-1]+lazy[ed-1]); 
		st/=2; 
		ed/=2; 
	}
	minl+=lazy[st];
	minr+=lazy[ed]; 
	ed/=2; 
	min_tot = min(minl,minr); 
	while(ed>=1)
	{
		min_tot+=lazy[ed]; 
		ed/=2; 
	}
	return min_tot; 
}
void reset_all()
{
	lazy.clear();
	max_tree.clear();
	min_tree.clear(); 
	lazy.assign(base*2,0);
	max_tree.assign(base*2,0);
	min_tree.assign(base*2,0); 
}
int main()
{ 
	freopen("input.txt","r",stdin);
	scanf("%d",&n1);
	for (int i=0; i<n1; i++)
	{
		int a,b;
		scanf("%d %d",&a,&b);
		x.push_back(a);
		y.push_back(b);
		temp.push_back(make_pair(make_pair(a,b),0));  
	}
	scanf("%d",&n2); 
	for (int i=0; i<n2; i++)
	{
		int a,b;
		scanf("%d %d",&a,&b);
		x.push_back(a);
		y.push_back(b);  
		temp.push_back(make_pair(make_pair(a,b),1));
	}
	int q[2]; 
	scanf("%d %d",&q[0],&q[1]); 
	q[1]*=-1; 
	for(base=1; base<n1+n2; base*=2);
	sort(x.begin(),x.end());
	sort(y.begin(),y.end()); 
	for(int i=0; i<x.size(); i++)
	{
		x1[x[i]]=i; 
		y1[y[i]]=i;  
	}
	for(int i=0; i<temp.size(); i++)
	{
		mp[x1[temp[i].first.first]][y1[temp[i].first.second]]=q[temp[i].second];
		tranx[x1[temp[i].first.first]]=y1[temp[i].first.second];
		trany[y1[temp[i].first.second]]=x1[temp[i].first.first];
	}
	
	n=n1+n2; 
	/*
	for(int i=0; i<n; i++)
	{
		for(int j=0; j<n; j++)
		{
			printf("%d ",mp[j][i]);
		}
		printf("\n");
	}
	*/
	int maxk = MIN,i,j; 
	for(i=0; i<n; i++)
	{
		reset_all(); 
		for(j=i; j>=0; j--)
		{
			query_update(trany[j],base-1,mp[trany[j]][j]);
			int t,p;
			/*
			for(int k=1; k<base*2; k*=2)
			{
				for(int l=k; l<k*2; l++)
				{
					printf("%d ",lazy[l]);
				}
				printf("\n");
			}
			for(int k=1; k<base*2; k*=2)
			{
				for(int l=k; l<k*2; l++)
				{
					printf("%d ",min_tree[l]);
				}
				printf("\n");
			}
			*/
			t = query_max(trany[j],n-1);
			p=0; 
			if(trany[j]!=0 && trany[j]<=n-1)p = min(p,query_min(0,trany[j]-1));
			maxk = max(maxk,t-p); 
			//printf("-----%d %d %d %d %d\n",i,j,t,p,mp[trany[j]][j]);
			
		}
	}
	printf("%d",maxk); 
}
