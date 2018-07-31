#include<bits/stdc++.h>
using namespace std;
const int maxn=59; // number of rows(including the label)
const int train_num=23787;
double w[maxn+1];

double sigmod(double x){
	return 1.0/(1+exp(-x));
}
double x[train_num][maxn];
int ori_label[train_num];

double Accuracy(int a,int b,int c,int d){
	return (a+d)*1.0/(a+b+c+d);
}
double Precision(int a,int b){
	return a*1.0/(a+b);
}
double Recall(int a,int b){
	return a*1.0/(a+b);
}
double F1(double a,double b){
	return 2.0*a*b/(a+b);
}

/*
double x1[maxn+1]={0},x2[maxn+1]={0};
int rowcnt1=0,rowcnt2=0;
//*/

double sum[train_num];
double sum1[maxn];

double min_[maxn],max_[maxn];
int main(){
	
	memset(w,1,sizeof(w));
	ifstream train("train.csv");
	//ifstream test("test.csv");
	ofstream out("007_1_.txt");
	string f;
	
	int rowcnt=0,colcnt;
	bool start=false;
	
	while(getline(train,f)){
		if(!start){
			start=true;continue;  // rowcnt from 0 
		}
		for(int i=0;i<f.size();i++){
			if(f[i]==',') f[i]=' '; 
		}
		stringstream ss;
		ss<<f;
		string p;
		colcnt=1;   //colcnt from 1
		x[rowcnt][0]=1;
		
		while(ss>>p){
			if(colcnt==maxn){
				ori_label[rowcnt]=atoi(p.c_str());//the right label
				break;
			}
			else x[rowcnt][colcnt]=atof(p.c_str());
			colcnt++;
		}
		
		rowcnt++;
	}
	
	//normalization for each attribute
	
	for(int j=1;j<=maxn;j++){
		for(int i=0;i<rowcnt;i++){
			if(i==0){
				min_[j]=max_[j]=x[i][j];
			}
			if(x[i][j]>max_[j]) max_[j]=x[i][j];
			if(x[i][j]<min_[j]) min_[j]=x[i][j];
		}
		//cout<<j<<" "<<max_<<" "<<min_<<endl;
		for(int i=0;i<rowcnt;i++){
			x[i][j]=(x[i][j]-min_[j])*1.0/(max_[j]-min_[j]);
		}
		//system("pause");
	}
	
	
	bool train_done=false;
	int iterate_time=0;
	
	
	double rate=0.35; // step length
	while(iterate_time<208){
		train_done=true;
		for(int row=0;row<rowcnt;row++){
			sum[row]=0;
			//分别计算出每个训练样本的加权和
			for(int col=0;col<maxn;col++){
				sum[row]+=w[col]*x[row][col];
			}
			//cout<<sum[row]<<" ";
		}
		//计算w每一个维度的梯度。
		for(int k=0;k<maxn;k++){
			sum1[k]=0;//初始化当前维度的梯度值 
			for(int j=0;j<rowcnt;j++){
				sum1[k]+=(1/(1+exp(-sum[j]))-ori_label[j])*x[j][k];	
			}
			//根据一定步长更新w每个维度的值
			w[k]-=rate*sum1[k];
			
		}
		iterate_time++;		
		//cout<<endl;
		//system("pause");
	}
	cout<<"rate:"<<rate<<endl;
	cout<<"train iterate time:"<<iterate_time<<endl;
	
	rowcnt=0;
	string d;
	int tp=0,fn=0,fp=0,tn=0;
	ifstream fin("train.csv");
	while(getline(fin,d)){
		if(rowcnt==0){
			rowcnt++;continue;
		}
		stringstream ss;
		for(int i=0;i<d.size();i++){
			if(d[i]==',') d[i]=' ';
		}
		ss<<d;
		string p;
		int ans_label;
		double sum=w[0];
		colcnt=1;
		double data;
		while(ss>>p){
			if(colcnt==maxn){
				ans_label=atoi(p.c_str()); // should hide for test
				break;
			}
			data=atof(p.c_str());
			sum+=w[colcnt]*(data-min_[colcnt])*1.0/(max_[colcnt]-min_[colcnt]);
			colcnt++;
		}
		// should hide for test
		//cout<<sum<<" "<<ans_label<<endl;
		if(sigmod(sum)>=0.5&&ans_label==1){
			tp++;
		}
		if(sigmod(sum)>=0.5&&ans_label==0){
			fn++;
		}
		if(sigmod(sum)<0.5&&ans_label==1){
			fp++;
		}
		if(sigmod(sum)<0.5&&ans_label==0){
			tn++;
		}
		//*/
		if(sigmod(sum)>0.5) out<<1<<endl;
		else if(sigmod(sum)<0.5) out<<0<<endl;
		else{
			srand(time(NULL));
			out<<rand()%2<<endl;
		}
		rowcnt++;
		//system("pause");
	}
	cout<<rowcnt-1<<endl;
	//should hide for test
	cout<<"TP= "<<tp<<" FN= "<<fn<<" FP= "<<fp<<" TN= "<<tn<<endl;
	//four indexes
	double accu=Accuracy(tp,fn,fp,tn);
	double pre=Precision(tp,fp);
	double rec=Recall(tp,fn);
	double f1=F1(pre,rec);
	
	printf("Accuracy =%lf\nPrecision=%lf\nRecall   =%lf\nF1       =%lf\n",accu,pre,rec,f1);
	//*/
	return 0;
}
