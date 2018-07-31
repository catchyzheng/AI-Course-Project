#include<bits/stdc++.h>
using namespace std;
const int row=5272+2;
vector<string> c[row];
const int maxn=2600+6;
const int emotion_num=7;

set<string> se[emotion_num+1]; // set 0 is the total packet of words
map<string,int> loc;//the location of the string
int OH[row][maxn]={0}; //One Hot matrix with frequency
int testOH[maxn]={0}; // OH matrix for test sentence
int emo[row]; // emotion array
map<string, int > emotion;//from string to number
map<int ,string> reveremotion;//from number to string
string origin_emo[row];

int emo_num[emotion_num+1]={0};
int wordnum_of_emo[emotion_num+1]={0};
int wordnum_of_emo_repeat[emotion_num+1]={0};
int wordnum_of_emo_no_repeat[emotion_num+1]={0};
map<string,int> fre[emotion_num+1];//frequency of this word under specific emotion  

int main(){
	ifstream train("train.csv");
	//ifstream test("test.csv");
	ofstream out("007_3_.txt");
	string s;
	
	int count=1,cnt=0; // change 1
	emotion["anger"]=1;emotion["disgust"]=2;
	emotion["fear"]=3;emotion["guilt"]=4;
	emotion["joy"]=5;emotion["sadness"]=6;emotion["shame"]=7;
	reveremotion[1]="anger";reveremotion[2]="disgust";
	reveremotion[3]="fear";reveremotion[4]="guilt";
	reveremotion[5]="joy";reveremotion[6]="sadness";reveremotion[7]="shame";
	
	while(getline(train,s)){
		for(int i=0;i<s.size();i++){
			if(s[i]==','){
				s[i]=' ';
			}
		}
		stringstream ss;
		ss<<s;
		string p;
		int e; // emotion number of this sentence
		while(ss>>p){
			if(cnt==0){
				e=emotion[p];
				emo[count]=e;//record the emotion
				emo_num[e]++; // count the emotion total number
			}
			if(cnt>=1){//drop the string which read before 
				c[count].push_back(p); 
				wordnum_of_emo_repeat[e]++;
				if(se[0].find(p)==se[0].end()) se[0].insert(p); // total packet
				
				if(se[e].find(p)==se[e].end()){//check whether repeats 
					se[e].insert(p); wordnum_of_emo_no_repeat[e]++;
					fre[e][p]=1;
					 
				}
				else fre[e][p]++;
				//OH[count][loc[p]]=1;
			}
			cnt++;
		}
		cnt=0;count++;
		//cout<<count<<" ";
	}
	
	cout<<"total samples num: "<<count<<endl;
	int sum_repeat=0,sum_no_repeat=0;
	for(int i=1;i<=emotion_num;i++){
		cout<<"sample of emotion "<<reveremotion[i]<<" "<<i<<" is "
		<<wordnum_of_emo_repeat[i]<<" "<<wordnum_of_emo_no_repeat[i]<<endl;
		sum_repeat+=wordnum_of_emo_repeat[i];
		sum_no_repeat+=wordnum_of_emo_no_repeat[i];
	}
	//system("pause");
	/*
	for(int i=1;i<=6;i++){
		wordnum_of_emo_repeat[i]=sum_repeat*1.0/6;
		wordnum_of_emo_no_repeat[i]=sum_no_repeat*1.0/6;
	}
	cout<<sum_repeat<<" "<<sum_no_repeat<<endl;
	*/ 
	string ts;
	
	int rowcnt=1; // for test document
	double rate;
	int correct=0;// count the correct
	ifstream in("test.csv");
	while(getline(in,ts)){
		for(int i=0;i<ts.size();i++){
			if(ts[i]==','){
				ts[i]=' ';break;
			}
		}
		stringstream tss;
		tss<<ts;
		string p;
		
		int wordcnt=0;
		vector<string> testword;
		set<string> testset;
		while(tss>>p){
			//cout<<p<<endl;
			//----should hide for test---- 
			//if(wordcnt==0) origin_emo[rowcnt]=p; 
			///----------------------------
			if(wordcnt>=1){
				if(testset.find(p)==testset.end()) testword.push_back(p);
			}
			wordcnt++;
			//system("pause");
		}
		
		// find the emotion with greatest possibility
		double emo_possible[emotion_num],max_possible=-10;
		int emo_distribute=-1;
		
		
		for(int i=1;i<=emotion_num;i++){
			emo_possible[i]=emo_num[i]*1.0/count;
			for(int j=0;j<testword.size();j++){
				if(se[i].find(testword[j])!=se[i].end()){
					
					emo_possible[i]*=(fre[i][testword[j]]*1.0+1)/(wordnum_of_emo_no_repeat[i]+sum_no_repeat);
				}
				//smooth the word not in packet but in training set
				else if(se[0].find(testword[j])!=se[0].end()){ 
					
					emo_possible[i]*=1.0/(wordnum_of_emo_repeat[i]+sum_no_repeat);
				}
			}
			if(emo_possible[i]>max_possible){
				max_possible=emo_possible[i];
				emo_distribute=i;
			}
		}
		
		
		
		if(emo_distribute==-1){
			cout<<rowcnt<<" failed\n";
			out<<"failed"<<endl;
		}
		else out<<reveremotion[emo_distribute]<<endl;
		/*  //should hide for test
		out<<origin_emo[rowcnt]<<" "<<reveremotion[emo_distribute]<<endl;
		if(emo_distribute>0 && reveremotion[emo_distribute]==origin_emo[rowcnt]){
			correct++;
		}
		*/
		rowcnt++;
	}
	//cout<<correct*1.0/rowcnt<<endl;
	//cout<<rowcnt-count<<endl;
	//out<<correct*1.0/(rowcnt-count)<<endl;
	return 0;
}
