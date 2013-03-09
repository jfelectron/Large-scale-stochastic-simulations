#include <fstream>
#include <iostream>
#include <cstring>
#include <cmath>
#include "mtrand.h"

using namespace std;

int main(int argc,char* argv[]){
	float ka=0.3/60;
	float kf=20.0/60;
	float ktp;
	float ktn=0.2/60;
	float keb=0.12/60;
	int Lon;
	int Loff;
	int Tn;
	int Tc;
	unsigned long init[4] = {0x123, 0x234, 0x345, 0x456}, length = 4;
	MTRand_int32 irand(init, length);
	int n=10000;
	int maxT=2880;
	
	int bSize=0;
	cout<<"Num arg: "<<argc<<endl;
	for(int i=0;i<argc;i++) {
		if(strcmp(argv[i],"-bSize")==0) {
			bSize=atoi(argv[i+1]);
		}
		
	}
	ktp=kf*bSize;
	cout<<"burst size: "<<bSize<<endl;
	cout<<"export rate: "<<keb<<endl;
	float c []={ka,kf,ktp,ktn,keb};
	float t;
	int Tfinal [2][n];
	
	for (int k=0;k<n;k++) {
		Loff=1;
		Lon=0;
		Tc=0;
		Tn=0;
		int X []={Loff,Lon,Tn,Tc};
		t=0;
		while (t<2880) {
			float a []={c[0]*X[0],c[1]*X[1],c[2]*X[1],c[4]*X[2],c[3]*X[3]};
			
			float s=0;
			for (int j=0;j<5;j++){
				
				s+=a[j];
			}
			float r1=mt();
			float h=-log(r1)/s;
			t=t+h;
			float r2=mt();
			float ctr=0;
			int rxn=0;
			
			for (int j=0;j<5;j++){
				if (r2>ctr&&r2<ctr+a[j]/s) {
					rxn=j;
					break;
				}
				else ctr+=a[j]/s;
			}
			//cout<<"Reaction Chosen: "<<rxn<<endl;
			switch(rxn) {
				case 0:
					X[0]=0;
					X[1]=1;
					break;
				case 1:
					X[1]=0;
					X[0]=1;
					break;
				case 2:
					X[2]++;
					break;
				case 3:
					X[2]--;
					X[3]++;
					break;
				case 4:
					X[3]--;
					break; 
			}
		}
		Tfinal[0][k]=X[2];
		Tfinal[1][k]=X[3];
		
		
	}
	ofstream outputfile;
	char filename[100];
	sprintf(filename,"output_b%d",bSize);
	outputfile.open(filename);	
	for(int i=0;i<n;i++){
		outputfile<<Tfinal[0][i]<<","<<Tfinal[1][i]<<endl;
		
	}
	outputfile.close();
	
	return 0;
}
