#include <bits/stdc++.h>
using namespace std;

double sigmod(double prop)
{
	return 1.0 / (1.0 + exp(-prop) );
}

const int LEN = 58;//vector x len
const int TRAIN_SIZE = 23786/2 ;
const int TEST_SIZE = 15858;

//double w[LEN+1];//w[0]=1.0;
//double w_tmp[LEN+1];

//double w_data[30000];

const double eps = 0.005;

double step;

int n,m;

struct data{
	double x[LEN+1];//x[0]=1.0;
	int y;
	
	double w_data;
	void init(istream &in)
	{
		x[0]=1.0;
		for(int i=1;i<=LEN;i++)
		{
			in>>x[i];
			//cout << x[i] << ' ';
			char c;
			in>>c;
		}
		string la;
		in>>la;
		if(la[0]!='?')
		{
			y = atoi(la.c_str());
		}
		//in>>y;
		//cout << y << endl;
	}
	
	double dot(const double *w)const
	{
		double sum = 0.0;
		for(int i=0;i<=LEN;i++)
		{
			sum += w[i]*x[i];
		}
		return sum;
	}
	
	double dot2(const double *w)const
	{
		double sum = 0.0;
		for(int i=0;i<=LEN;i++)
		{
			sum += w[i]*x[i]*w_data;
		}
		return sum;
	}
}a[30000],b[30200];

double x_max[30000];
double x_min[30000];
//double x_sum[30000];

void pre_fac()
{
	//fill(sum,sum+n+1,0.0);
	
	for(int i=0;i<=LEN;i++)
	{
		x_max[i] = a[1].x[i];
		x_min[i] = x_max[i];
		//x_sum[i] = 0.0;
		for(int j=1;j<=n;j++)
		{
			x_max[i] = max(x_max[i],a[i].x[j]);
			x_min[i] = min(x_min[i],a[i].x[j]);
			//x_sum[i] += a[i].x[j];
		}
	}
}

void norm_dim(data *bg, data *ed, int i)
{
	data *ptr ;
	ptr = bg;
	double xmin = x_min[i];
	double xmax =  x_max[i]  ;
	//double x_sum = sum[]
	/*
	for( ptr=bg;ptr!=ed;ptr++ )
	{
		xmin = min(xmin,ptr->x[i]);
		xmax = max(xmax,ptr->x[i]);
	}
	*/
	for(ptr=bg;ptr!=ed;ptr++)
	{
		ptr->x[i] = 10.0*(ptr->x[i] - xmin)/(xmax - xmin+1.0);
		//ptr->x[i] /= x_max[i];
	}

}

int label(const data &d,const double *w)
{
	double res = sigmod( d.dot(w) );
	if(res > 0.5) return 1;
	return 0;
	
}

int label2(const data &d,const double *w)
{
	double res = sigmod( d.dot2(w) );
	if(res > 0.5) return 1;
	return -1;
	
}


double dots[30000];//a[i].dot(w) result
double errors[LEN+2];//error func (or you can call it division

void get_dots(const double *w)
{
	for(int i=1;i<=n;i++)
	{
		dots[i] = a[i].dot(w);
	}
}

double error_func(int id)
{
	double sum = 0.0;
	for(int i=1;i<=n;i++)
	{
		//sum += ( sigmod( a[i].dot(w) )  - a[i].y ) * a[i].x[id] ;
		sum += ( sigmod(dots[i]) - a[i].y ) * a[i].x[id];
	}
	return sum;
}

void get_error()
{
	for(int i=0;i<=LEN;i++)
	{
		errors[i] = error_func(i);
	}
}

void update_w( double *w)
{
	/*
	for(int i=0;i<=LEN;i++) w_tmp[i] = w[i];
	for(int i=0;i<=LEN;i++)
	{
		w[i] = w_tmp[i] - step*error_func(i,w_tmp) ;
	}
	*/
	get_dots(w);
	get_error();
	for(int i=0;i<=LEN;i++)
	{
		w[i] = w[i] - step*errors[i];
	}
	
	
}

void read_data()//all data that matters
{
	ifstream fin;
	fin.open("train.csv",ios::in);
	
	if(!fin)
	{
		cout << "failed to open data" << endl;
		exit(1);
	}
	
	string ign;
	getline(fin,ign);
	
	n = TRAIN_SIZE;
	for(int i=1;i<=n;i++)
	{
		a[i].init(fin);
		//a[i].w_data = 1.0 / n;
	}
	
	fin.close();
	
	fin.open("test.csv",ios::in);
	if(!fin)
	{
		cout << "failed to open data" << endl;
		exit(1);
	}
	
	getline(fin,ign);
	
	m = TEST_SIZE;
	
	for(int i=1;i<=m;i++)
	{
		b[i].init(fin);
	}
	pre_fac();
	cout << "read finish" << endl;
	
}

//double w_tmp[]
//only the way contains?

const int totrain = 3000;
int try_predict(const double *w)
{
	int cnt = 0;
	for(int i=1;i<=100;i++)
	{
		int res = label2(a[i],w);
		if(res==1) cnt++;
	}
	return cnt;
	//return max(100-cnt,cnt);
}

struct Adaboast{
	double w[LEN+1];
	double fac;
	
	void init()
	{
		w[0]=1.0;
		fill(w+1,w+LEN+1,0.0);
	}
	void to_train()
	{	
		cout << "in data train" << endl;
		step = 0.004;
		int iter_num = 1700;
	
		int cnt = 0;
		
		for(int cnt=1;cnt<=iter_num;cnt++)
		{
			update_w(w);//now we get the training
		}
	}
	
	double get_em()const
	{
		double em = 0.0;
		for(int i=1;i<=totrain;i++)
		{
			int res = label(a[i],w);
			if(res != a[i].y )
			em += a[i].w_data;
		}
		return em;
	}
	
	void update_fac(const double &val)
	{
		fac = val;
	}
	
	void update_wv()
	{
		double em = get_em();
		cout << "em: " << em << endl;
		
		update_fac(0.5 * log( (1.0 - em) / em ) );
		
		double z = 0.0;
		for(int i=1;i<=totrain;i++)
		{
			int res = label(a[i],w);
			if(res==0)res--;
			
			int y = a[i].y;
			if(y==0)y--;
			
			z += a[i].w_data * exp( -fac * y * res );
		}
		
		for(int i=1;i<=totrain;i++)
		{
			int res = label(a[i],w);
			if(res==0)res--;
			
			int y = a[i].y;
			if(y==0)y--;
			
			a[i].w_data = a[i].w_data * exp( -fac * y * res ) / z;
		}
	}
};

vector< Adaboast > vec;
void work_train()
{
	cout << "in train" << endl;
	for(int i=1;i<=LEN;i++)
	{
		norm_dim(a+1,a+n+1,i);
	}
	
	for(int i=1;i<=totrain;i++)
	{
		a[i].w_data = 1.0 / totrain;
	}
	
	//data *train;
	//data *valid;
	//train = a+1;
	n = 1000;
	
	//valid = a+600;
	
	
	
	
	
	int train_num =10;
	for(int train_time = 1; train_time <= train_num; train_time++ )
	{
		Adaboast tmp;
		tmp.init();
		tmp.to_train();
		tmp.update_wv();//update weight for data and itself
		vec.push_back(tmp);
		random_shuffle(a+1,a+totrain+1);//get new one
	}
	
	//for(int i=1;i<=LEN;i++)w[i]=0.0;
	//w[0]=1.0;
	
	//while(1)
	//for(int cnt=1;cnt<=1000;cnt++)
	
	
	/*
	for(int i=1;i<=n;i++)
	{
		cout << a[i].dot(w) << ' ';
		cout << sigmod( a[i].dot(w) ) << ' ' << a[i].y << endl;
	}
	*/
	
	cout << "finish" << endl;
}

void work_valid()
{
	data *valid = a+4*n+5;
	int sz = 7000;
	
	int cnt[4][4];
	memset(cnt,0,sizeof(cnt));
	
	ofstream fout;
	fout.open("check2.txt",ios::out);
	
	for(int i=1;i<=sz;i++)
	{
	//	cout << to_decide(i) << ' ' << b[i].y << endl;
		int ori = valid[i].y;
		int prd;// = label(valid[i]);
		//if( prd == ori) cnts++;
		double fin = 0.0;
		for(int j=0;j<vec.size();j++)
		{
			int yy = label(valid[i],vec[j].w);
			if(yy==0)yy--;
			fin += vec[j].fac * yy;
		}
		
		if(fin>1e-5) prd=1;
		else prd=0;
		
		cnt[ ori+1 ][ prd+1 ]++;
		
		fout << ori <<  ' '  << prd << endl;
		//fout << prd << endl;
	}
	
	
	int tp = cnt[2][2];
	int fn = cnt[2][1];
	int fp = cnt[1][2];
	int tn = cnt[1][1];
	
	cout << "TP: " << tp << endl;
	cout << "FN: " << fn << endl;
	cout << "FP: " << fp << endl;
	cout << "TN: " << tn << endl;
	
	double Accuracy = 1.0*(tp+tn)/(tp+fp+tn+fn);
	double Recall = 1.0*tp / (tp+fn);
	double Precision = 1.0*tp/(tp+fp);
	double F1 = 2.0*Recall*Precision/(Precision+Recall);
	
	printf("Accuracy: %.5lf\n",Accuracy);
	printf("Recall: %.5lf\n",Recall);
	printf("Precision: %.5lf\n",Precision);
	printf("F1: %.5lf\n",F1);
	
}

void work_test()
{	
	cout << "in test"<< endl;
	ofstream fout;
	fout.open("result.txt",ios::out);
	for(int i=1;i<=LEN;i++)
	{
		norm_dim(b+1,b+m+1,i);
	}
	for(int i=1;i<=m;i++)
	{
		int prd ;//= label(b[i]);
		double fin = 0.0;
		for(int j=0;j<vec.size();j++)
		{
			int yy = label(b[i],vec[j].w);
			if(yy==0)yy--;
			fin += vec[j].fac * yy;
		}
		
		if(fin>1e-5) prd=1;
		else prd=0;		
		fout << prd << endl;
	}
	
}

int main()
{
	read_data();
	work_train();
	work_valid();
	work_test();
	return 0;
}
