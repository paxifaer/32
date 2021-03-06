
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <mmsystem.h>
#include<string.h>
#include<cstring>

//void AI(int board[][19], int wcount[][1700], int &player, int &winner);
/****************************************/     //基础设置
//srand((unsigned)time(NULL));
struct Point			//点坐标的结构体
{
    int x,y;
}point;
int huowu[21], chongwu[21], shuanghuosi[21], danhuosi[21], miansi[21], huosan[21], miansan[21], huoer[21];
int i1=1500,i2=400,i3=300,i4=150,i5=75,i6=30,i7=6,i8=4;


void record(int board[][20],int board1[][20],int wcount[][1700],int wcount1[][1700]);
void Search(int table[][20],  int player, int &x, int &y, int board[][20]);

struct  ranseti{int huowu;
int chongwu;
int shuanghuosi;
int danhuosi, miansi, huosan, miansan, huoer;
	
	int shiyingdu=0; 
	
};
ranseti ra[21];
ranseti gj[6];//冠军序列 

void Init(int board[][20], int wcount[][1700])				//初始化函数，将记录棋子的数组初始化
{
	int i, j;

    for(i = 1; i <=19; i++)
	{//初始换棋盘状态
        for(j = 1; j <=19; j++)
		{
            board[i][j] = 0;
		}
	}

	for(i = 0; i < 3; i++)
	{
		for(j = 1; j <=361; j++)
		{
			wcount[i][j] = 0;
		}
	}

	
}




/***********************************************************************************************/   //六子棋核心部分（先后手，胜负判定，棋盘记录）


void Record(int board[][20], int wcount[3][1700], const int player,int x,int y)              //记录棋子、棋手的情况
{
    board[x][y] = player;   
	
	 //棋盘状态记录 0表示空 1表示甲 2表示乙
//printf("\nx=%c  y=%c\n",(x-1+'A'),(y-1+'A'));
	int k;int anti=1;
int sum1=(y-1)*19+x,sum2=19*(y-1)+x+400,sum3=19*(y-1)+x+800,sum4=19*(y-1)+x+1200;
	(anti==player)?(anti=2):(anti=1);
	int flag;
	
wcount[player][(y-1)*19+x]=1;
wcount[player][19*(y-1)+x+400]=1;
wcount[player][19*(y-1)+x+800]=1;
wcount[player][19*(y-1)+x+1200]=1;
wcount[anti][sum1]=-1;
wcount[anti][sum2]=-1;
wcount[anti][sum3]=-1;
wcount[anti][sum4]=-1;

int l=0,r=0;//横连 
if(sum1-1>0)
l=wcount[player][sum1-1];
if(sum1+1<=361)
r=wcount[player][sum1+1];


if(l>=0)
wcount[player][sum1]+=l;
if(r>=0)
wcount[player][sum1]+=r;


if(l>0)
for(int sd=1;sd<=l;sd++)
{
wcount[player][sum1-sd]=wcount[player][sum1];
//printf("\nwcount[player][sum-sd]=%d",wcount[player][sum1-sd]);
}
if(r>0)
for(int sd=1;sd<=r;sd++)
{
wcount[player][sum1+sd]=wcount[player][sum1];
//printf("\n%d",wcount[player][sum1]);
}
//printf("\n l1=%d r1=%d",l,r);


l=0,r=0;//纵连 
if(y-1>0)
l=wcount[player][sum2-19];
if(y+1<=19)
r=wcount[player][sum2+19];

if(l>=0)
wcount[player][sum2]+=l;
if(r>=0)
wcount[player][sum2]+=r;

if(l>0)
for(int sd=1;sd<=l;sd++)
wcount[player][sum2-sd*19]=wcount[player][sum2];
if(r>0)
for(int sd=1;sd<=r;sd++)
wcount[player][sum2+sd*19]=wcount[player][sum2];
//printf("\n l2=%d r2=%d",l,r);


l=0,r=0;//左斜连 
if(sum3-19-1>800)
l=wcount[player][sum3-19-1];
if(sum3+19+1<=1161)
r=wcount[player][sum3+19+1];

if(l>=0)
wcount[player][sum3]+=l;
if(r>=0)
wcount[player][sum3]+=r;

if(l>0)
for(int sd=1;sd<=l;sd++)
wcount[player][sum3-sd*19-sd]=wcount[player][sum3];
if(r>0)
for(int sd=1;sd<=r;sd++)
wcount[player][sum3+sd*20]=wcount[player][sum3];
//printf("\n l3=%d r3=%d",l,r);

l=0,r=0;//右斜连 
if(sum4-19+1>1200)
l=wcount[player][sum4-19+1];
if(sum4+19-1<=1560)
r=wcount[player][sum4+19-1];
if(l>=0)
wcount[player][sum4]+=l;
if(r>=0)
wcount[player][sum4]+=r;

if(l>0)
for(int sd=1;sd<=l;sd++)
wcount[player][sum4-sd*19+sd]=wcount[player][sum4];
if(r>0)
for(int sd=1;sd<=r;sd++)
wcount[player][sum4+sd*18]=wcount[player][sum4];

//printf("\n l4=%d r4=%d\n",l,r);

}

int Judge(int wcount[][1700], const int player)                   //胜负判断
{
	int i;
	int temp;
	int anti=1;
	(anti==player)?(anti=2):(anti=1);

	temp = 0;
	for(i = 1; i <=1600; i++)
	{
		if(wcount[player][i] >= 0)
		{
			temp++;
			if(wcount[player][i] >= 6)
			{
				return player;
			}
		}
	}

	

}




/*****************************************************************************************************/           //AI核心

void Grade(const int board[][20], const int wcount[][1700], int table[][20], const int ply,ranseti gene )         //棋盘评分的程序，给予棋盘每个点一定的分数
{
	int x, y, k,r=0,l=0,sum1,sum2,sum3,sum4;
	int temp = 0;
	int anti=1;
	int player=ply;int heng,zong,zuo,you;
	(anti==ply)?(anti=2):(anti=1);
/*六连=888888(极值)，活五=2500，冲五=600，双活四=400
单活四加200，每个眠四加100，每个活三加50，每个眠三加10
每个活二加4， 每个眠二加1*/
// huowu 12  chongwu 9 shuanghuosi 9  danhuosi 8 miansi 7  huosan 6 miansan 4  huoer 3




	for(x =1 ;x <= 19; x++)
	{
		
		for(y = 1; y <= 19; y++)
		{
			
			heng=1,zong=1,zuo=1,you=1;
			//printf("\n%d%d%d%d",y,y,y,y);
			
			if(board[x][y]==0)

           {//printf("\n%d%d%d%d",y,y,y,y);
           	       sum1=(y-1)*19+x,sum2=19*(y-1)+x+400,sum3=19*(y-1)+x+800,sum4=19*(y-1)+x+1200;
           	     
					l=0,r=0;//横连 
                   if(x-1>0)
                    l=wcount[ply][sum1-1];
                    if(x+1<=19)
                    r=wcount[player][sum1+1];
                    if(l>0)
                    heng+=l;
                    if(r>0)
                    heng+=r;
                    
               temp=0;
			   
	
			   	int sub=0;
			   for(int k=1;k<=6-heng;k++)
			   {
			  
			   
				   	if(l>=0)
			   	if((wcount[player][sum1-l-k]<0||(x-l-k)==0))//左边界 
			   	{sub++;
			   		
				   }
				   if(r>=0)
				   if(wcount[player][sum1+k+r]<0||(x+r+k)==19)//右边界 
			   	{sub++;
			   		
				   }
				   
				   if(sub==0)
				   {
				   	temp=0;
				   }
				   else if(sub==1)
				   temp=1;
				   else temp=9;
			
			   }
			   
			   
			       



if(heng<=6)
switch(heng)
{
case 1:table[x][y] += 0;break;
case 2:
if(temp == 0)
							table[x][y] += gene.huoer;
						else if(temp == 1)
							table[x][y] += 1;
							else if(temp==9)
							table[x][y] += -10;
						
						break;
					case 3:
						if(temp == 0)
							table[x][y] += gene.huosan;
						else if(temp==1)
							table[x][y] += gene.miansan;
						else if(temp==9)
							table[x][y] += -10;
						break;
					case 4:
						if(temp == 0)
						{
							table[x][y] += gene.shuanghuosi;
							break;
						}
						if(temp == 1)
						{
							table[x][y] += gene.danhuosi;
							break;
						}
						else if(temp==9)
						{
								table[x][y] += -10;break;}
									break;
					case 5:
						if(temp == 0)
							table[x][y] += gene.huowu;
						else if(temp==1)
							table[x][y] += gene.chongwu;
							else if(temp==9)
							{
								table[x][y] += -10;}
								break;
					case 6:
						table[x][y] += 8888888;
						break;
					default:
						;
					}
				
if(heng>=7) table[x][y]+=8888888;
//if(l>=3||r>=3)
//printf("\nl1=%d   r1=%d \n",l,r);
//printf("\nheng  %d",heng);

 //printf("\n\n\n 3 table[x][y]=%d ",table[x][y]) ; 





l=0,r=0;//纵连 
if(y-1>0)
l=wcount[player][sum2-19];
if(y+1<20)
r=wcount[player][sum2+19];
//if(l>=1||r>=1)
//printf("\nl2=%d   r2=%d \n",l,r);
  if(l>=0)
zong+=l;
if(r>=0)
zong+=r;


 sub=0;
 for(int k=1;k<=6-zong;k++)
		if((6-zong)>0)	   {
			   	
			   	if(l>=0)
			   	if((wcount[player][sum2-l*19-k*19]<0||(y-l-k)==0))//左边界 
			   	{sub++;
			   		
				   }
				   if(r>=0)
				   if(wcount[player][sum2+k*19+r*19]<0||(y+r+k)==19)//右边界 
			   	{sub++;
			   		
				   }
				   
				   if(sub==0)
				   {
				   	temp=0;
				   }
				   else if(sub==1)
				   temp=1;
				   else temp=9;
				   
			   }




if(zong<=6)
           switch(zong)
					{
						case 1:table[x][y] += 0;break;
					case 2:
						if(temp == 0)
							table[x][y] += gene.huoer;
						else if(temp == 1)
							table[x][y] += 1;
							else if(temp==9)
							table[x][y] += -10;
						
						break;
					case 3:
						if(temp == 0)
							table[x][y] += gene.huosan;
						else if(temp==1)
							table[x][y] += gene.miansan;
							else if(temp==9)
							table[x][y] += -10;
						break;
					case 4:
						if(temp == 0)
						{
							table[x][y] += gene.shuanghuosi;
							break;
						}
						if(temp == 1)
						{
							table[x][y] += gene.danhuosi;
							break;
						}
						else if(temp==9)
							table[x][y] += -10;
							break;
					case 5:
						if(temp == 0)
							table[x][y] += gene.huowu;
						else if(temp==1)
							table[x][y] += gene.chongwu;
							else if(temp==9)
							table[x][y] += -10;
							break;
							
					case 6:
						table[x][y] += 8888888;
						break;
					default:
						;
					}
				
if(zong>=7) table[x][y]+=8888888;
//if((x-1+'A')=='B'&&(y-1+'A')=='F')
//printf("\n BF=zong  %d",zong);
//if((x-1+'A')=='B'&&(y-1+'A')=='J'&&board[2][6]!=0)
//printf("\n BJ=zong  %d  l=%d  r=%d table= %d \n",zong,l,r,table[x][y]);

//if(l>=3||r>=3)
//printf("\nl2=%d   r2=%d \n",l,r);
 //printf("\n 4 table[x][y]=%d ",table[x][y]) ; 


           	
           	
           	
           	l=0,r=0;//左斜连 
if((x-1)>0&&(y-1)>0)
l=wcount[player][sum3-19-1];
if((x+1)<=19&&(y+1)<=19)
r=wcount[player][sum3+19+1];
 if(l>=0)
zuo+=l;
if(r>=0)
zuo+=r;




 sub=0;
 for(int k=1;k<=6-zuo;k++)
			   {
			   	
			   	if(l>=0)
			   	if(wcount[player][sum3-l*20-k*20]<0||((y-l-k)==0&&(x-l-k)==0))//左边界 
			   	{sub++;
			   		
				   }
				   if(r>=0)
				   if(wcount[player][sum3+k*20+r*20]<0||((y+r+k)==0&&(x+r+k)==0))//右边界 
			   	{sub++;
			   		
				   }
				   
				   if(sub==0)
				   {
				   	temp=0;
				   }
				   else if(sub==1)
				   temp=1;
				   else temp=9;
				   
			   }


//printf("\nl=%d   r=%d \n",l,r);
/*for(k=1;k<=6-zuo;k++)
{int sub=0;
   
	if((wcount[anti][sum3-l*19-k*19-l-k]>0||((x-1)>0&&(y-1)>0)&&wcount[anti][sum3+r*19+k*19+r+k]<=0)||(wcount[anti][sum3+r*20+k*20]>0||((x+1)<=19&&(y+1)<=19)&&wcount[anti][sum3-l*20-k*20]<=0))
	{
	if(sub<2)
	temp=1;//眠棋 
	sub++;
	}
	else if(sub>=2)
{	temp=9;

break;

}

//死棋 
	else
	temp=0; //活棋 
}*/
       if(zuo<=6)
       {
	   switch(zuo)
					{
						case 1:table[x][y] += 0;break;
					case 2:
						if(temp == 0)
							table[x][y] += gene.huoer;
						else if(temp == 1)
							table[x][y] += 1;
							else if(temp==9)
							table[x][y] += -10;
						
						break;
					case 3:
						if(temp == 0)
							table[x][y] += gene.huosan;
						else if(temp==1)
							table[x][y] += gene.miansan;
							else if(temp==9)
							table[x][y] += -10;
						break;
					case 4:
						if(temp == 0)
						{
							table[x][y] += gene.shuanghuosi;
							break;
						}
						if(temp == 1)
						{
							table[x][y] += gene.danhuosi;
							break;
						}
						else if(temp==9)
							table[x][y] += -10;
								break;
					case 5:
						if(temp == 0)
							table[x][y] += gene.huowu;
						else if(temp==1)
							table[x][y] += gene.chongwu;
							else if(temp==9)
							table[x][y] += -10;
								break;
					case 6:
						table[x][y] += 8888888;
						break;
					default:
						;
					}
				
			}
if(zuo>=7) table[x][y]+=8888888;
//printf("\nzuo  %d",zuo);
 //printf("\n 5 table[x][y]=%d ",table[x][y]) ; 
//if(l>=3||r>=3)
//printf("\nl3=%d   r3=%d \n",l,r);

l=0,r=0;//右斜连 
if(sum4-19+1>1200)
{

l=wcount[player][sum4-18];
//printf("\nl=%d",l);
}
if(sum4+19-1<1560)
{
r=wcount[player][sum4+18];
//printf("\nr=%d",r);
}
  if(l>=0)
you+=l;
if(r>=0)
you+=r;


//printf("\nl4=%d r4=%d ",l,r);

 sub=0;
 for(int k=1;k<=6-you;k++)
			   {
			   	
			   	if(l>=0)
			   	if(wcount[player][sum4-l*18-k*18]<0||((y-l-k)==0&&(x+l+k)==0))//左边界 
			   	{sub++;
			   		
				   }
				   if(r>=0)
				   if(wcount[player][sum4+k*18+r*18]<0||((y+r+k)==0&&(x-r-k)==0))//右边界 
			   	{sub++;
			   		
				   }
				   
				   if(sub==0)
				   {
				   	temp=0;
				   }
				   else if(sub==1)
				   temp=1;
				   else temp=9;
				   
			   }





           if(you<=6)
            switch(you)
					{
						case 1:table[x][y] += 0;
						break;
					case 2:
						if(temp == 0)
							table[x][y] += gene.huoer;
						else if(temp == 1)
							table[x][y] += 1;
							else if(temp==9)
							table[x][y] +=-10;
						
						break;
					case 3:
						if(temp == 0)
							table[x][y] += gene.huosan;
						else if(temp==1)
							table[x][y] += gene.miansan;
							else if(temp==9)
							table[x][y] += -10;
						break;
					case 4:
						if(temp == 0)
						{
							table[x][y] += gene.shuanghuosi;
							break;
						}
						if(temp == 1)
						{
							table[x][y] += gene.danhuosi;
							break;
						}
						else if(temp==9)
							table[x][y] += -10;
								break;
					case 5:
						if(temp == 0)
							table[x][y] += gene.huowu;
						else if(temp==1)
							table[x][y] += gene.chongwu;
							else if(temp==9)
							table[x][y] +=-10;
								break;
					case 6:
						table[x][y] += 8888888;
						break;
					default:
						;
					}
				
if(you>=7) table[x][y]+=8888888;
           	
  // printf("\nyou %d\n",you);       	
     //  printf("\n 6 table[x][y]=%d ",table[x][y]) ;  
	//   if(l>=3||r>=3)
//printf("\nl4=%d   r4=%d \n",l,r);
//	 if(x==2&&y==6)
//	printf("\n x=%c y=%c heng=%d  zong=%d  zuo=%d  you=%d  l4=%d  r4=%d   player=%d  table=%d\n",(x-1+'A'),(y-1+'A'),heng,zong,zuo,you,l,r,player,table[x][y]);	
//if((x-1+'A')=='B'&&(y-1+'A')=='J'&&board[2][6]!=0)
///printf("\n BJ=zong  %d  l=%d  r=%d heng=%d  zuo=%d you=%d table= %d \n",zong,l,r,heng,zuo,you,table[x][y]);
}
}}

}
void Search(int table[][20],  int player, int &x, int &y, int board[][20])       //搜索函数 找出评分表中分值最大的位置
{
	int i, j;
	int  max;
	int num = 0, time = 0;

	max = 0;
	for(i = 1; i <= 19; i++)
	{
		for(j = 1; j <= 19; j++)
		{
			if(!board[i][j] && table[i][j] && table[i][j] > max)
			{
				max = table[i][j];
			}
		}
	}

	if(!board[7][7] && !max)
	{
		x =7;
		y = 7;
		return;
	}

	for(i = 1; i <= 19; i++)
	{
		for(j = 1; j <= 19; j++)
		{
			if(!board[i][j] && table[i][j] == max)
			{
			
				x=i;
				y=j;
				break;
			}
		}
	}
//	printf("\nmax=%d %c%c \n",max,i+'A'-1,j+'A'-1);

}

void AI(int board[][20], int wcount[][1700], int player,int &winner,ranseti r1)          //AI函数 先调用评分函数，对双方棋盘评分， 再调用搜索函数，找出最优位置
{
	int x1_max, y1_max;
	int x2_max, y2_max;
	int table_cmp[20][20]={0};
	int table_ply[20][20]={0};
int player2;

if(player==1)
player2=2;
else
player2=1;
winner=0;
/*printf("board=\n");
for(int i=1;i<=19;i++)
{printf("\n");
	for(int j=1;j<=19;j++)
	{
		printf("%d ",board[i][j]);
	}
}*/
	Grade(board, wcount, table_cmp,player ,r1);
	Grade(board, wcount, table_ply, player2,r1);

	Search(table_cmp, player, x1_max, y1_max, board);
	Search(table_ply, player2, x2_max, y2_max, board);
   

	if(table_cmp[x1_max][y1_max] >= table_ply[x2_max][y2_max])
	{
		point.x = x1_max;
		point.y = y1_max;
	 // Record(board,wcount,player,x1_max,y1_max);//
	}
	else
	{
		point.x = x2_max;
		point.y = y2_max;
	//	  Record(board,wcount,player,x2_max,y2_max);
	}
	
//	printf("\n player==%d  的最大table值为 %d %d\n",player,table_ply[x2_max][y2_max],table_cmp[x1_max][y1_max]) ;
winner=Judge(wcount,player); 


}
/*六连=888888(极值)，活五=2500，冲五=600，双活四=400
单活四加200，每个眠四加100，每个活三加50，每个眠三加10
每个活二加4， 每个眠二加1*/

ranseti chongzu[21];
int Pm(ranseti *gj,int avg,int fmax)//基因变异概率 
{float pmm;
		if(gj->shiyingdu<avg)
	{pmm=0.1;
	}
	else if(fmax!=avg)
	{pmm=0.1-(0.1-0.01)*((float)fmax-(float)gj->shiyingdu)/(fmax-avg);
	}
	float p=(rand()%16)*pmm;
if(p>=1)
return 1;
else return 0;
	
}// huowu 12  chongwu 9 shuanghuosi 9  danhuosi 8 miansi 7  huosan 6 miansan 4  huoer 3
void by(ranseti *g,int avg,int fmax)
{int p,x=0;
	for(int i=0;i<12;i++)
	{p=Pm(g,avg,fmax);
	x+=p;
	x<<1;
	
	
	}
x>>1;
g->huowu=g->huowu^x+400;//实现基因突变，运用数学的方法

x=0;
	for(int i=0;i<9;i++)
	{p=Pm(g,avg,fmax);
	x+=p;
	x<<1;
	
	
	}
x>>1;

g->chongwu=g->chongwu^x+200;//实现基因突变，运用数学的方法
x=0;
	for(int i=0;i<9;i++)
	{p=Pm(g,avg,fmax);
	x+=p;
	x<<1;
	
	
	}
x>>1;
g->shuanghuosi=g->shuanghuosi^x+150;//实现基因突变，运用数学的方法
x=0;
	for(int i=0;i<8;i++)
	{p=Pm(g,avg,fmax);
	x+=p;
	x<<1;
	
	
	}
x>>1;
g->danhuosi=g->danhuosi^x+80;//实现基因突变，运用数学的方法
 x=0;
	for(int i=0;i<7;i++)
	{p=Pm(g,avg,fmax);
	x+=p;
	x<<1;
	
	
	}
x>>1;
g->miansi=g->miansi^x+60;//实现基因突变，运用数学的方法
x=0;
	for(int i=0;i<6;i++)
	{p=Pm(g,avg,fmax);
	x+=p;
	x<<1;
	
	
	}
x>>1;
g->huosan=g->huosan^x+100;//实现基因突变，运用数学的方法
 x=0;
	for(int i=0;i<4;i++)
	{p=Pm(g,avg,fmax);
	x+=p;
	x<<1;
	
	
	}
x>>1;
g->miansan=g->miansan^x+40;//实现基因突变，运用数学的方法
 x=0;
	for(int i=0;i<3;i++)
	{p=Pm(g,avg,fmax);
	x+=p;
	x<<1;
	
	
	}
x>>1;
g->huoer=g->huoer^x;//实现基因突变，运用数学的方法

 
}
void bianyi(ranseti *g,int avg,int fmax)//基因变异 
{
	
	by(g,avg,fmax);
}

int pc(int avg,int fmax,int f)//每条染色体的基因是否发生交叉互换 
{
	float p=0.7;
	if(f<avg)
	p=0.9;
	else if(fmax!=avg)
	p=0.9-(float)(0.9-0.6)*(f-avg)/(fmax-avg);
	float sd;
	int x=rand()%11;
sd=p*x;	
if(sd>=1)
return 1;
else return 0;
}
ranseti cz[21]; 
void exchange(ranseti x,ranseti y,int &num,int avg,int fmax)
{
int n=num;int nu=0;
int st=pc(avg,fmax,x.shiyingdu);
	
if(st==1)
{cz[n].huowu=y.huowu;
cz[n++].huowu=x.huowu;
n++;

}	

else{
	cz[n++].huowu=y.huowu;
cz[n].huowu=x.huowu;
n++;

}
n-=2;
st=pc(avg,fmax,x.shiyingdu);

if(st==1)
{cz[n].chongwu=y.chongwu;
cz[n++].chongwu=x.chongwu;
n++;

}
else{
	cz[n++].chongwu=y.chongwu;
cz[n].chongwu=x.chongwu;
n++;

}

n-=2;

st=pc(avg,fmax,x.shiyingdu);

if(st==1)
{
cz[n].shuanghuosi=y.shuanghuosi;
cz[n++].shuanghuosi=x.shuanghuosi;
n++;

}
else{

cz[n].shuanghuosi=x.shuanghuosi;
	cz[n++].shuanghuosi=y.shuanghuosi;

n++;
}
n-=2;

st=pc(avg,fmax,x.shiyingdu);
if(st==1)
{cz[n].danhuosi=y.danhuosi;
cz[n++].danhuosi=x.danhuosi;
n++;
}
else{
	cz[n++].danhuosi=y.danhuosi;
cz[n].danhuosi=x.danhuosi;
n++;
}
n-=2;
st=pc(avg,fmax,x.shiyingdu);
if(st==1)
{cz[n].miansi=y.miansi;
cz[n++].miansi=x.miansi;
n++;
}
else{
	cz[n++].miansi=y.miansi;
cz[n].miansi=x.miansi;
n++;
}
n-=2;
st=pc(avg,fmax,x.shiyingdu);
if(st==1)
{cz[n].huosan=y.huosan;
cz[n++].huosan=x.huosan;
n++;
}
else{
	cz[n++].huosan=y.huosan;
cz[n].huosan=x.huosan;
n++;
}
n-=2;
st=pc(avg,fmax,x.shiyingdu);
if(st==1)
{cz[n].miansan=y.miansan;
cz[n++].miansan=x.miansan;
n++;
}
else{
	cz[n++].miansan=y.miansan;
cz[n].miansan=x.miansan;
n++;
}

n-=2;
st=pc(avg,fmax,x.shiyingdu);
if(st==1)
{cz[n].huoer=y.huoer;
cz[n++].huoer=x.huoer;
n++;
}
else{
	cz[n++].huoer=y.huoer;
cz[n].huoer=x.huoer;
n++;
}
num=n;

}


void Cz(ranseti gj[],int avg,int i,int fmax)//开始基因重组 
{int num=1;
int nu=0;
	for(int i=1;i<=5;i++)
	{
		for(int j=i+1;j<=5;j++)
		{
			exchange(gj[i],gj[j],num,avg,fmax);//每两个不同的染色体开始交叉 
		}
	}
}
int max(ranseti sb[])
{
	int ma=0;
	for(int i=1;i<=5;i++)
	{
		if(ma<=sb[i].shiyingdu)
		{ma=sb[i].shiyingdu;
		}
	}
	return ma;
	
 } 

void jiaocha(ranseti gj[],int avg)
{int nu=0;
	int fmax=max(gj);//找到适应度最大值
	
	for(int i=1;i<=5;i++)//冠军染色体开始变异 
	{
		bianyi(&gj[i],avg,fmax);
		
		
		
			Cz(gj,avg,i,fmax);//基因重组 
		//	printf("%d ",nu++);
			
	}
	
	for(int i=1;i<=20;i++)//生成新一代染色体 
	{
		memcpy(&ra[i],&cz[i],sizeof(ra[i]));
	}

}


int bisai(ranseti ra1,ranseti ra2,int board[][20],int wcount[][1700])
{int num=50;
int flag=0;
int board1[20][20],wcount1[3][1700],wcount2[3][1700]; 
record(board,board1,wcount,wcount1);
record(board,board1,wcount,wcount2);
int winner1=0,winner2=0;

	while(num--)
	{int winner1=0,winner2=0;
	

			AI(board1, wcount1,1, winner1,ra1);
			//	printf("\n%d ",winner1);
			Record(board1,wcount1,1,point.x,point.y);
			if(winner1)
			{flag=1;
			break;
			}
			
				AI(board1, wcount2, 2, winner2,ra2);
					Record(board1,wcount2,2,point.x,point.y);
				if(winner2)
			{flag=2;
			break;
			}
	}
	if(flag==1)
	return 1;//ra1赢 
	else if(flag==2)
	return 2;//ra2赢 
	else if(flag==0)
	return 0;//平局 
 } 
 
 void guanjun(ranseti ra[20])
{ranseti a[21];
ranseti b,c;
for(int i=1;i<20;i++)
{
	memcpy(&a[i],&ra[i],sizeof(a[i]));
}
	for(int i=1;i<=20;i++)
	{
	
		for(int j=i+1;j<=20;j++)
		{
		if(a[i].shiyingdu<a[j].shiyingdu)
		{
			memcpy(&b,&a[i],sizeof(b));
			memcpy(&c,&a[j],sizeof(c));
			memcpy(&a[i],&c,sizeof(a[i]));
			memcpy(&a[j],&b,sizeof(a[j]));
			}	
		}
	}
	for(int j=1;j<=5;j++)
memcpy(&gj[j],&a[j],sizeof(gj[j]));
}

void jingsai(int board1[][20],int wcount1[][1700],int player)
{int nu=0;
	int jieguo1=0,jieguo2=0;
	for(int i=1;i<=4;i++)//20组染色体分成五组，每组四个，这四个染色体进行比较。 
	{
	for(int j=i;j<=20;j+=4){
	for(int k=j+1;k<=j+3;k++)
	{
	
	jieguo1=bisai(ra[j],ra[k],board1,wcount1);
	jieguo2=bisai(ra[k],ra[j],board1,wcount1);
	if(jieguo1==0)
	{
		ra[j].shiyingdu+=25;
		ra[k].shiyingdu+=25;
	}
	else if(jieguo1==1)
	{ra[j].shiyingdu+=50;
	ra[k].shiyingdu+=0;
	}
		else if(jieguo1==2)
	{ra[k].shiyingdu+=50;
	ra[j].shiyingdu+=0;
	}
	
		if(jieguo2==0)
	{
		ra[j].shiyingdu+=25;
		ra[k].shiyingdu+=25;
	}
	else if(jieguo2==1)
	{ra[j].shiyingdu+=50;
	ra[k].shiyingdu+=0;
	}
		else if(jieguo2==2)
	{ra[k].shiyingdu+=50;
	ra[j].shiyingdu+=0;
	}
	
	}}
	
	}

	int avg=0,num=0;
	for(int i=1;i<=20;i++)
	num+=ra[i].shiyingdu;
	avg=num/20;
	guanjun(ra);//得出冠军的五个序列；
	
	jiaocha(gj,avg); //

	
}

void record(int board[][20],int board1[][20],int wcount[][1700],int wcount1[][1700])
{
	memcpy(board1,board,sizeof(int)*20*20);
		memcpy(wcount1,wcount,sizeof(int)*3*1700);
}
void yichuan(int board[][20],int wcount[][1700],int daishu,int winner,int player)
{
	int wcount1[3][1700];
	int board1[20][20];
	record(board,board1,wcount,wcount1);//拷贝棋盘记录 
//	printf("std");
//int num=0;
	while(daishu--)//进化多少代 
	{
		jingsai(board1,wcount1,player);
		 
	}
	
	
}
void renji()  
{int winner = 0;	char message[256];int num=0;
int player = 0;          //棋手 1表示甲 2表示乙
		//int step_num = 0; //步数记录
		int board[20][20]={{0}};        //棋盘记录
		int wcount[3][1700]={{0}};
	int anti=0;

		int i = 15;
int daishu=50;
ranseti r;
int x1,y1,x2,y2;
char xs1,xs2,ys1,ys2;	
	while(1)
	{fflush(stdout);
	
		 //记录游戏中的胜者  0表示无胜者 1表示甲胜 2表示乙胜 3表示平局
		

	
	scanf("%s",message);
	if(strcmp(message,"move")==0)
	{
		scanf("%s",message);
		fflush(stdin);
		x1=message[0]-'A'+1;
		y1=message[1]-'A'+1;
		x2=message[2]-'A'+1;
		y2=message[3]-'A'+1;
		
	
//	printf("%d %d %d %d \n",x1,y1,x2,y2);
		
         	Record(board,wcount,anti,x1,y1);
			 Record(board,wcount,anti,x2,y2);
			yichuan(board,wcount,daishu,winner,anti);
			
			
			
			
		//	memcpy(&r,&ra[1],sizeof(r));
			
			memcpy(&r,&gj[2],sizeof(r));
			
				AI(board, wcount, player, winner,r);//待定 
		      Record(board,wcount,player,point.x,point.y);
		       xs1=point.x+'A'-1;
		       ys1=point.y+'A'-1;
		       AI(board, wcount, player, winner,r);//待定 
		      Record(board,wcount,player,point.x,point.y);
		        xs2=point.x+'A'-1;
		       ys2=point.y+'A'-1; //printf(" %d",num++);
		       		//	printf("\n冠军数组 染色体值为：%d %d %d %d %d %d  \n",gj[2].huowu,gj[2].chongwu,gj[2].shuanghuosi,gj[2].huosan,gj[2].huoer,gj[2].shiyingdu);

         printf("\n");    printf("move %c%c%c%c\n",xs1,ys1,xs2,ys2);
           
		
}
else if(strcmp(message,"new")==0)
{

	scanf("%s",message);
	fflush(stdin);
	if(strcmp(message,"black")==0)
	{
	anti=2;player=1;}
	else {
	player=2;anti=1;}
	Init(board,wcount);
	if(player==1)
	{char s1='A'+7,s2='A'+7;
	Record(board,wcount,player,7,7);
	   printf("\n");	printf("move GG@@\n");
		
	}
}
else if(strcmp(message,"error")==0)//着法错误?
{
fflush(stdin);
 } 
 else if(strcmp(message,"name?")==0)//询问引擎名称
 {
 fflush(stdin);
 	printf("name Parsifal\n");
 	
  } 
  else if(strcmp(message,"end")==0)//对局结束
  {
  	fflush(stdin);
   } 
    else if(strcmp(message,"quit")==0)
    {
   	fflush(stdin);
    	printf("Quit!\n");
    	break;
	}
	
	//printf("\n%d ",winner);
	
	}
}
/**************************************************************************************************************/
int main()   //主函数
{srand((unsigned)time(NULL));
	for(int i=1;i<=20;i++)//给20组染色体定初值 
{i1+=100;
i2+=20;i1+=16;
i1+=8;i1+=4;
i1+=2;i1+=1;i1+=1;
	huowu[i]=i1;
	chongwu[i]=i2;
	shuanghuosi[i]=i3;
	danhuosi[i]=i4;
	miansi[i]=i5;
	huosan[i]=i6;
	miansan[i]=i7;
	huoer[i]=i8;
	ra[i].chongwu=	chongwu[i];
	ra[i].huowu=huowu[i];
	ra[i].shuanghuosi=shuanghuosi[i];
	ra[i].danhuosi=danhuosi[i];
	ra[i].miansi=miansi[i];
	ra[i].huoer=huoer[i];
	ra[i].huosan=huosan[i];
	ra[i].miansan=miansan[i];
}

memcpy(&gj[1],&ra[1],sizeof(ra[1]));
memcpy(&gj[2],&ra[2],sizeof(ra[2]));
memcpy(&gj[3],&ra[3],sizeof(ra[3]));
memcpy(&gj[4],&ra[4],sizeof(ra[4]));
memcpy(&gj[5],&ra[5],sizeof(ra[5]));
	renji();
	
}



