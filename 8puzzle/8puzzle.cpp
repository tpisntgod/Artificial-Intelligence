#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<cstring>
#include<cmath>

using namespace std;

struct Node {
    int a[3][3];  //8数码 
    int h;
    int x,y;  //空位的位置
    bool check() {
        if (x>=0 && x<3 && y>=0 && y<3) return true;
        return false;
    }
} st;

int w[9]={1,1,2,6,24,120,720,5040,40320};
int dest=322560;
int b[4][2]={{0,1},{1,0},{0,-1},{-1,0}};
int times,m,succ=0,sum_cost=0,z=0,issolve,sum_restart=0,temp_times=0;
double T=70;

bool solveable(Node t){    //求出逆序对数，判断是否有解 
    int a[9],k=0;  
    for(int i=0;i<3;i++)  
        for(int j=0;j<3;j++)  
            a[k++]=t.a[i][j];  
    int s=0;  
    for(int i=0;i<9;i++)  
        for(int j=i+1;j<9;j++)  
            if(a[j] && a[i] && a[i]>a[j])  
                s++;  
    return !(s&1);    //如果逆序对数为偶数才有解 
}

Node generate_initial_state() {
	Node k;
	for (int i=0;i<3;i++)
		for (int j=0;j<3;j++)
			k.a[i][j]=3*i+j;
	while(1) {
		for (int i=0;i<9;i++) {
			int p=rand()%9,t;
			t=k.a[p/3][p%3];
			k.a[p/3][p%3]=k.a[i/3][i%3];
			k.a[i/3][i%3]=t;
		}
		if (solveable(k)) break;
	}
	for (int i=0;i<3;i++)
		for (int j=0;j<3;j++)
			if (k.a[i][j]==0) {
				k.x=i; k.y=j;
			}
	return k;
}

int get_hash(Node h) {
    int c[9],k=0;  
    for(int i=0;i<3;i++)  
        for(int j=0;j<3;j++)  
            c[k++]=h.a[i][j];
    int s=0;  
    for(int i=0;i<9;i++){  
        int k=0;  
        for(int j=0;j<i;j++)  
            if(c[j]>c[i])  
                k++;  
        s+=w[i]*k;  
    }  
    return s;  
}

int get_h(Node h) {
    int s=0;
    for (int i=0;i<3;i++)
        for (int j=0;j<3;j++) 
        if (h.a[i][j]) {
            s+=abs(i-(h.a[i][j]-1)/3)+abs(j-(h.a[i][j]-1)%3);
        }
    return s;
}

void most_steep_hillclimbing(Node u) {
	Node v;
	int steps=0,p;
	int min=get_h(u);
	while(1) {
		Node j;
		p=0;
		for (int i=0;i<4;i++) {
            v=u;
            v.x=v.x+b[i][0];
            v.y=v.y+b[i][1];
            if (v.check()) {
                v.a[u.x][u.y]=v.a[v.x][v.y];
                v.a[v.x][v.y]=0;
                if (get_h(v)<min) {
                	j=v;
                	p=1;
                	min=get_h(v);
				}
            }
        }
        if (p==0) {
		    if (get_hash(u)==dest) {
		    	succ++;
		    	issolve=1;
		    	sum_cost+=steps;
		    }
			return;
		}
        u=j;
		steps++;
	}
}

void first_choice_hillclimbing(Node u) {
	Node v;
	int steps=0,p;
	while(1) {
		p=0;
		int min=get_h(u);
		int i=rand()%4,t=4;
		while(t--) {
            v=u;
            v.x=v.x+b[i][0];
            v.y=v.y+b[i][1];
            if (v.check()) {
                v.a[u.x][u.y]=v.a[v.x][v.y];
                v.a[v.x][v.y]=0;
                if (get_h(v)<min) {
                	u=v;
                	p=1;
                	break;
				}
            }
            i=(i+1)%4;
        }
		steps++;
        if (p==0) {
	        if (get_hash(u)==dest) {
	        	succ++;
	        	sum_cost+=steps;
	        }
			return;
		}
	}
}

void simulated_annealing(Node u) {
	Node v;
	int steps=0,p;
	T=70; temp_times=0;
	while(1) {
		p=0;
		int min=get_h(u);
		int i=rand()%4,t=4;
		while(t--) {
            v=u;
            v.x=v.x+b[i][0];
            v.y=v.y+b[i][1];
            if (v.check()) {
                v.a[u.x][u.y]=v.a[v.x][v.y];
                v.a[v.x][v.y]=0;
                if (get_h(v)<min) {
                	u=v;
                	p=1;
                	break;
				} else {
					if (++temp_times%300==0) {
						T*=0.8;
					}
					if (T<0.1) return;
					double e=min-get_h(v);
					double pro=exp(e/(double)T);
					double r=rand() / double(RAND_MAX);
					if (r<pro) {
						u=v;
                		p=1;
                		break;
					}
				}
            }
            i=(i+1)%4;
        }
		steps++;
        if (get_hash(u)==dest) {
        	succ++;
        	sum_cost+=steps;
        	return;
        }
        if (p==0) {
	        if (get_hash(u)==dest) {
	        	succ++;
	        	sum_cost+=steps;
	        }
			return;
		}
	}
}

int main() {
	srand((int)time(NULL));
	scanf("%d",&times);
	m=times;
	while(times--) {
		st=generate_initial_state();
		if (get_hash(st)==dest) {
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
