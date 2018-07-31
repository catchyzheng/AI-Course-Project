#include <bits/stdc++.h>
using namespace std;


double max_d[12]={0.0,8000.0,2000.0,0.5,100.0,1000000.0,100.0,5000.0,50.0,19.0,10.0};

struct data{
	double x[10];
	double y;
	
	friend istream &operator >> (istream &fin,data &d)//first ignore the title
	{
		char c;
		for(int i=1;i<=9;i++)
		{
			fin>>d.x[i]>>c;
			//max_d[i] = max( max_d[i], d.x[i] );
			d.x[i] /= max_d[i];
			
		}
		//finally input result
		string judge;
		fin>>judge;
		if(judge[0]!='?')//train data
		{
			d.y = atof(judge.c_str());
		}
		return fin;
	}
	
};

const int TRAIN = 13903-2+1;
const int VALID = 13903 - TRAIN - 1;
const int TEST = 3478-2+1;

const int K = 11;

data train[TRAIN+10];
data valid[VALID+10];

data test[TEST+10];

double dist( const data &a,const data &b)
{
	double sum = 0.0;
	for(int i=1;i<=9;i++)
	{
		double dl = a.x[i] - b.x[i];
		sum += dl*dl;
	}
	return sqrt(sum);
}


	
void get_data()//include train,valid and test
{
	ifstream fin;
	fin.open("train.csv",ios::in);
	if(!fin)
	{
		cout << "check name" << endl;
		exit(1);
	}
	
	string ign;
	getline(fin,ign);
	
	for(int i=1;i<=TRAIN;i++)
	{
		fin>>train[i];
	}
	
	for(int i=1;i<=VALID;i++)
	{
		fin>>valid[i];
	}
	

	
	fin.close();
	fin.open("test.csv",ios::in);
	if(!fin)
	{
		cout << "check name" << endl;
		exit(1);
	}
	getline(fin,ign);
	for(int i=1;i<=TEST;i++)
	{
		fin>>test[i];
	}
	fin.close();
}

void work_train()//dummy
{
	return;
}

double tmp_dist[TRAIN+10];
int id[TRAIN+10];

double w[K+10];

bool cmp(int x,int y)//less comes first
{
	return tmp_dist[x] < tmp_dist[y];
}

void norm(double *w)
{
	double sum = 0.0;
	for(int i=1;i<=K;i++)
	{
		sum += w[i];
	}
	for(int i=1;i<=K;i++)
	{
		w[i] /= sum;
	}
}

double get_res(const data &tar)
{
	for(int i=1;i<=TRAIN;i++)
	{
		id[i] = i;
		tmp_dist[i] = dist(tar,train[i]);
	}
	
	sort(id+1,id+TRAIN+1,cmp);
	
	if( tmp_dist[ id[1] ] < 1e-6 ) return train[ id[1] ].y;
	
	for(int i=1;i<=K;i++)//take just inverse
	{
		w[i] = 1.0 / tmp_dist[ id[i] ];
		//w[i] = sqrt(w[i]);
		//w[i]*=w[i]*w[i];
		w[i] = pow(w[i],3.03);
	}
	
	norm(w);
	
	double res = .0;
	for(int i=1;i<=K;i++)
	{
		res += w[i] * train[ id[i] ].y;
	}
	return res;
	
	
}

void check_valid()///that's for adjust arg
{
	ofstream fout;
	fout.open("res.txt",ios::out);
	
	for(int i=1;i<=VALID;i++)
	{
		double res = get_res(valid[i]);
		
		//double dv = (valid[i].y - res )/ valid[i].y;
		
		//cout << dv << endl;
		//cout << valid[i].y << ' ' << res << endl;
		fout << valid[i].y << ' ' << res << endl;
	}
}

void work_test()
{
	ofstream fout;
	fout.open("007_4.txt",ios::out);
	
	for(int i=1;i<=TEST;i++)
	{
		double res = get_res(test[i]);
		
		//double dv = (valid[i].y - res )/ valid[i].y;
		
		//cout << dv << endl;
		//cout << valid[i].y << ' ' << res << endl;
		fout <<  res << endl;
	}
}

int main()
{
	get_data();
	work_train();
	//check_valid();
	work_test();
}
