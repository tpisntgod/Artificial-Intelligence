#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<cstring>
#include<cmath>

using namespace std;

struct Node {
    int a[9];  //8皇后位置(i,a[i]) 
    int c[9];  //第i列皇后和其他皇后冲突数量 
} st;
int times,m,succ=0,sum_cost=0,issolve=0,sum_restart=0,temp_times=0;
double T=70;

Node generate_initial_state() {
	Node k;
	for (int i=1;i<=8;i++) {
		int j=1+rand()%8;
		k.a[i]=j;
	}
	return k;
}

int get_collision(Node &k) {
	int s=0;
	for (int i=1;i<9;i++) k.c[i]=0;
	for (int i=1;i<9;i++) {
		for (int j=1;j<9;j++) 
		if (j!=i) {
			if (k.a[j]==k.a[i] || i+k.a[i]==j+k.a[j] ||
			(k.a[i]-i)==(k.a[j]-j)) {
				k.c[i]++; s++;
			}
		}
	}
	return s;
}

void most_steep_hillclimbing(Node u) {
	Node v;
	int steps=0,p;
	int max=0,j;
	while(1) {
		p=0; j=0; max=0;
		if (get_collision(u)==0) {
			succ++;
		    sum_cost+=steps;
		    issolve=1;
		    return;			
		}
		for (int i=1;i<9;i++)
			if (u.c[i]>max) {
				max=u.c[i];
				j=i;
			}
		if (j!=0) {
			int min_col=1000,new_row;
			Node e=u;
			for (int i=1;i<9;i++) {
				e.a[j]=i;
				int f=get_collision(e);
				if (f<min_col) {
					min_col=f;
					new_row=i;
				}
			}
			if (u.a[j]==new_row) {
				return;
			}
			e.a[j]=new_row;
        	u=e;
		} else {
			succ++;
		    sum_cost+=steps;
		    issolve=1;
		    return;
		}
		steps++;
	}
}

void first_choice_hillclimbing(Node u) {
	Node v;
	int steps=0,p,col;
	int max=0,j;
	while(1) {
		p=0; j=0; max=0;
		col=get_collision(u);
		if (col==0) {
			succ++;
		    sum_cost+=steps;
		    return;			
		}
		int try_times=0;
		while(1) {
			int x=1+rand()%8,y=1+rand()%8;
			while (u.a[x]==y) {
				x=1+rand()%8; y=1+rand()%8;
			}
			Node e=u;
			e.a[x]=y;
			if (get_collision(e)<col) {
				u=e;
				break;
			}
			if (++try_times>100) {
				return;
			}
		}
		steps++;
	}
}

void simulated_annealing(Node u) {
	Node v;
	int steps=0,p,col;
	int max=0,j;
	T=70; temp_times=0;
	while(1) {
		p=0; j=0; max=0;
		col=get_collision(u);
		if (col==0) {
			succ++;
		    sum_cost+=steps;
		    return;			
		}
		int try_times=0;
		while(1) {
			int x=1+rand()%8,y=1+rand()%8;
			while (u.a[x]==y) {
				x=1+rand()%8; y=1+rand()%8;
			}
			Node e=u;
			e.a[x]=y;
			if (get_collision(e)<col) {
				u=e;
				break;
			} else {
				if (++temp_times%300==0) {
					T*=0.8;
				}
				if (T<0.1) return;
				double delta_e=col-get_collision(e);
				double pro=exp(delta_e/(double)T);
				double r=rand() / double(RAND_MAX);
				if (r<pro) {
					u=e;
            		p=1;
            		break;
				}
			}
		}
		steps++;
	}
}

int main() {
	srand((int)time(NULL));
	scanf("%d",&times);
	m=times;
	while(times--) {
		st=generate_initial_state();
		if (get_collision(st)==0) {
			succ++;
			continue;
        }
        simulated_annealing(st);
        /* 
        //random restart hill climbing 
        issolve=0; sum_restart=0;
        while(!issolve) {
        	most_steep_hillclimbing(st);
        	if (!issolve) {
				st=generate_initial_state();
				sum_restart++;
			}
		}
		printf("restart times: %d \n",sum_restart);
		*/
	}
	printf("test: %d success: %d ",m,succ);
	if (succ>0) printf("average cost: %d",sum_cost/succ);
}
