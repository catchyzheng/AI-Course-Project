#include<bits/stdc++.h>
using namespace std;
int is_number(string s){
    if(s[0]<'0'||'9'<s[0]){
        return 1;
    }
    return 0;
}
bool isContinueAttri(int j){
	if(j==0||j==2||j==4||j==10||j==11||j==12){
		return true;
	}
	else return false;
}
double log_(double n){
	if(abs(n)<0.001) return log(0.001);
	else return log(n);
}

const double pi=acos(-1.0);
double F(double x,double aver,double vari){
	return 1.0/(sqrt(2*pi)*sqrt(vari))*exp(-(x-vari)*(x-aver)/(2.0*vari));
}
const int row_num=30162+1;// total row number
const int col_num=14+1; // total column number 
const int attri_num=col_num-1;
vector<string> atrib1[attri_num];
set<string> atrib2[attri_num];
vector<double> atrib_continue[attri_num][2]; //0,1,total
double average[attri_num][2];
double variance[attri_num][2];
string data[row_num][col_num];

map<string,int> proj[3]; // [0]for label 0, [1] for 1 
int attri_cnt[attri_num][2]; //[0] for label 0,[1] for 1 
int attri_total_cnt[attri_num]; // attri_total_cnt[i]= attri_cnt[i][0]+attri_cnt[i][1];
double conti_max_[attri_num], conti_min_[attri_num];
int TP,FN,FP,TN;
int main(){
	int rowcnt=0;
	ifstream train("train.csv");
	ofstream out("007_2_233.txt");
	string s;
	stringstream ss;
	int colcnt=0;
	cout<<"fuck\n";
	int label0num=0,label1num=0; // numeber of label 
	while(getline(train,s)){
		if(rowcnt==0){
			rowcnt++;continue; // the sample id starts from 1
		}
		
		for(int h=0;h<s.size();h++){
			if(s[h]==',') s[h]=' ';
		}
		
		ss<<s;
		string p;
		colcnt=0;
		
		while(ss>>p){
			data[rowcnt][colcnt]=p;
			if(colcnt==attri_num){
				if(p=="1") label1num++;
				else if(p=="0") label0num++;
			}
			if(!isdigit(p[0])||!isdigit(p[p.size()-1])){// discrete attribute
				if(atrib2[colcnt].find(p)==atrib2[colcnt].end()){
					atrib2[colcnt].insert(p);
					atrib1[colcnt].push_back(p);
				}
			}
			colcnt++;
		}
		//cout<<rowcnt<<" ";
		rowcnt++;
		s.clear();ss.clear();
		//system("pause");
	}
	
	// read data again to count the condition possibility
	for(int i=0;i<rowcnt;i++){
		if(data[i][attri_num]=="1"){//label 1
			for(int j=0;j<attri_num;j++){
			
					proj[1][data[i][j]]++;
			}
		}
		else{ //label 0
			for(int j=0;j<attri_num;j++){
				
					proj[0][data[i][j]]++;
			}
		}
	}
	
	
	// for each discrete attribute, count the sample sum under 0 / 1 label
	for(int j=0;j<attri_num;j++){
		attri_cnt[j][0]=attri_cnt[j][1]=0;
		for(int k=0;k<atrib1[j].size();k++){
			attri_cnt[j][0]+=proj[0][atrib1[j][k]];
			attri_cnt[j][1]+=proj[1][atrib1[j][k]];
			//cout<<"0:"<<proj[0][atrib1[j][k]]<<" "<<attri_cnt[j][0]<<endl;
			//cout<<"1:"<<proj[1][atrib1[j][k]]<<" "<<attri_cnt[j][1]<<endl;
		}
		//cout<<attri_cnt[j][0]<<" "<<attri_cnt[j][1]<<endl;
	}
	
	//system("pause");
	for(int j=0;j<attri_num;j++){
		attri_total_cnt[j]=attri_cnt[j][0]+attri_cnt[j][1];
		//cout<<attri_total_cnt[j]<<endl; 
	}
	//system("pause");
	
	
	/*
	double x;
	while(cin>>x){
		cout<<F(x,average[10][0],variance[10][0])<<endl;
	}*/
	
	
	
	
	
	string d;   // if declare outside, remember to clear after one iteration
	stringstream dd;
	ifstream in("train.csv");
	int row=0;
	int correct=0;
	
	TP=TN=FP=FN=0;
	int count1=0;
	while(getline(in,d)){
		if(row==0){
			row++;continue;
		}
		
		for(int r=0;r<d.size();r++){
			if(d[r]==',') d[r]=' ';
		}
		
		double for0=log(label0num*1.0/(label0num+label1num));
		double for1=log(label1num*1.0/(label0num+label1num));
		//cout<<for0<<" "<<for1<<endl;
		int col=0;
		string c;
		int predict;
		int ori_label;
		dd<<d;
		while(dd>>c){
			if(col==col_num-1) {
				ori_label=atoi(c.c_str());//should hide for test
				continue;
			}
			// for discrete attribute
			for0+=log((proj[0][c]+1)*1.0/(attri_cnt[1][0]));
			for1+=log((proj[1][c]+1)*1.0/(attri_cnt[1][1]));
			//for0+=log((proj[0][c]+1)*1.0/(attri_cnt[col][0]));
			//for1+=log((proj[1][c]+1)*1.0/(attri_cnt[col][1]));
			//cout<<c<<" "<<proj[0][c]<<" "<<proj[1][c]<<endl;
			
			//cout<<for0<<" "<<for1<<endl;
			col++;
			//system("pause");
		}
		//system("pause");
		//cout<<for0<<" "<<for1<<endl;
		//possible for 0
		if(for1>for0) {
			predict=1;
			count1++;
		}
		else predict=0;
		
		out<<predict<<endl;//should show for test
		//should hide for test
		if(ori_label==1&&predict==1){
			TP++;
		}
		else if(ori_label==0&&predict==1){
			FP++;
		}
		else if(ori_label==1&&predict==0){
			FN++;
		}
		else if(ori_label==0&&predict==0){
			TN++;
		}//*/
		row++;
		d.clear();dd.clear();
		//system("pause");
	}
	
	cout<<"row: "<<row-1<<" 1: "<<count1<<endl;
	//should hide for test
	printf("TP= %d FN= %d FP= %d TN= %d\n",TP,FN,FP,TN);
	double Accuracy,Precision,Recall,F1;
	Accuracy=(TP+TN)*1.0/(TP+FN+FP+TN);
	Precision=TP*1.0/(TP+FP);
	Recall=TP*1.0/(TP+FN);
	F1=2*Precision*Recall/(Precision+Recall);
	printf("Accuracy= %.3lf\nPrecision=%.3lf\nRecall   =%.3lf\nF1       =%.3lf\n",Accuracy,Precision,Recall,F1);
	//*/
	
	
	
	return 0;
}
