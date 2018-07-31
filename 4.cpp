#include<bits/stdc++.h>
using namespace std;
const int maxn=10;  // TOTAL COLCUMN  
const int train_num=13902+2;//TOTAL ROW

double x[train_num][maxn]; // training set data 
double ori_ans[train_num];
double min_[maxn-1],max_[maxn-1];
int main(){
	
	ifstream train("train.csv");
	ofstream out("007_4_v2.txt");
	string f;
	stringstream ss;
	int rowcnt=0,colcnt;
	while(getline(train,f)){ // read train data
		if(rowcnt==0){
			rowcnt++;continue;  // row count from 1
		}
		for(int i=0;i<f.size();i++){
			if(f[i]==',') f[i]=' '; 
		}
		
		ss<<f;
		string p;
		colcnt=0;  // column count from 0
		
		while(ss>>p){
			if(colcnt==maxn-1) {
				ori_ans[rowcnt]=atof(p.c_str());
				//cout<<ori_ans[rowcnt]<<endl;
				break;
			}
			x[rowcnt][colcnt]=atof(p.c_str())/10;
			if(rowcnt==1){
				min_[colcnt]=max_[colcnt]=x[rowcnt][colcnt];
			}
			else{
				if(min_[colcnt]>x[rowcnt][colcnt]) min_[colcnt]=x[rowcnt][colcnt];
				if(max_[colcnt]<x[rowcnt][colcnt]) max_[colcnt]=x[rowcnt][colcnt];
			}
			//cout<<x[rowcnt][colcnt]<<" ";
			colcnt++;
		}
		rowcnt++;
		f.clear();ss.clear();
		//system("pause");
	}
	//normalization
	for(int i=1;i<rowcnt;i++){
		for(int j=0;j<maxn-1;j++){
			x[i][j]=(x[i][j]-min_[j])*10.0/(max_[j]-min_[j]);
		}
	}
	
	int trainrow=rowcnt-1; 
	rowcnt=0;
	cout<<trainrow<<endl;
	int K=8; // K for knn
	
	ifstream in("test.csv");
	double test_data[maxn];
	vector<double> knn_distance;
	vector<int> knn_rowid;
	double RWSD;
	double cancha=0; // can cha
	int fucknum=0;
	while(getline(in,f)){// read valid/test data
		if(rowcnt==0){
			rowcnt++;continue;  // row count from 1
		}
		for(int i=0;i<f.size();i++){
			if(f[i]==',') f[i]=' ';
		}
		ss<<f;
		colcnt=0; // colcumn count from 0
		string d;
		while(ss>>d){
			if(colcnt==maxn-1){
				//RWSD=atof(d.c_str());  //should hide for test
				//cout<<RWSD<<endl;
				break;
			}
			test_data[colcnt]=atof(d.c_str())/10;
			test_data[colcnt]=
			(test_data[colcnt]-min_[colcnt])*10.0/(max_[colcnt]-min_[colcnt]);
			//cout<<test_data[colcnt]<<" ";
			colcnt++;
		}
		
		set<pair<double, int> > neigh; // neighbor
		for(int i=1;i<=trainrow;i++){ // calculate all the distances
			double distance=0;
			for(int j=0;j<maxn-1;j++){
				distance+=(x[i][j]-test_data[j])*(x[i][j]-test_data[j]);
			}
			neigh.insert(make_pair(distance,i));
		}
		
		set<pair<double, int> >::iterator it=neigh.begin();
		double tmp_sum=0;
		for(int K_tmp=0;it!=neigh.end();it++,K_tmp++){ // pick up the first K distance
			if(K_tmp>=K){
				break;
			}
			if((*it).first<0.00001){//special judge, two samples are totally same with each other. 
				knn_distance.push_back((*it).first);
				knn_rowid.push_back((*it).second);
				break;
			}
			tmp_sum+=1.0/(*it).first;
			knn_distance.push_back((*it).first);
			knn_rowid.push_back((*it).second);
		}
		/*
		for(int i=0;i<knn_distance.size();i++){
			cout<<knn_distance[i]<<" ";
		}*/
		/*
		double weight[maxn-1];
		for(int i=0;i<maxn-1;i++){
			weight[i]=(1.0/knn_distance[i])/tmp_sum;
		}*/
		double average=0;
		if(knn_rowid.size()<K){// absolutely equalled
			average=ori_ans[knn_rowid[0]];
		}
		else{
			int Size=knn_rowid.size();
			for(int i=0;i<knn_rowid.size();i++){
				//standard average
				//average+=ori_ans[knn_rowid[i]]*1.0/Size;
				// weighted average
				average+=ori_ans[knn_rowid[i]]*(1.0/knn_distance[i])/tmp_sum;
			}
		}
		out<<average<<endl;
		/*  // should hide for test 
		cancha+=(average-RWSD)*(average-RWSD);
		out<<RWSD<<" "<<average<<" ";
		out<<abs(average-RWSD)<<" ";
		if(abs(RWSD-average)>3){
			out<<"fuck!\n";
			fucknum++;
		}
		else out<<endl;
		//*/
		f.clear();ss.clear();
		knn_distance.clear();
		knn_rowid.clear();
		//system("pause");
		fucknum++;
	}
	cout<<fucknum<<endl; 
	return 0;
}
