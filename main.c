#include"acllib.h"
#include<stdlib.h> 
#define Food_x (rand()%25)
#define Food_y (rand()%17)
#define CONTINUE "继续"
#define START "开始"
#define win_color RGB(0,100,100)
#define W 1200
#define H 780 
#define To_top 30
#define To_left 30
#define To_right 150
#define To_bottom 50
#define R_round 20
#define Dia_a 40
#define W_num 25
#define H_num 17
#define Time_step 130
#define Key_dis 10
#define Key_h 50
#define Key_totop 70
#define Key_key 30
#define Key_color_stkey RGB(color_stkey,0,0)
#define Key_color_st RGB(color_st,0,0)
#define Key_color_ex RGB(color_ex,0,0)
#define Key_color_pa RGB(color_pa,0,0)
#define Key_color_re RGB(color_re,0,0)
#define gameover_color RGB(0,0,200)
#define T_color RGB(255,255,255)
#define dx 5
#define dy 5
int score=0;
int HEAD=0,GAMEOVER=0,st_or_n=0,board_st=0,SPACE=0,eat_or_n=0;
typedef enum
{
	UP,
	RIGHT,
	DOWN,
	LEFT
}Drection;
Drection drection;
void launch_food(void);
void init_ar(void);
void ar_move(Drection);
int x,y,food_x,food_y;
int ar[H_num][W_num],ar_i=0,ar_j=0;
int color_st=100,color_ex=100,color_pa=100,color_re=100,color_stkey=100;
typedef struct snake
{
	int*body;
	struct snake*next_body;
}Snake;
Snake*head,*tail,*t;
void init_snake(void);
void start_click(void);
void exit_click(void);
void pause_click(void);
void replay_click(void); 
void stkey_click(void);
void replay(void);
void free_all(void);
void mouselistener(int x,int y,int button,int event);
void keyboardlistener(int key,int event);
char*T_start="开始",*T_exit="退出",*T_pause="暂停",*T_replay="重玩",*T_gameover="GAME OVER!";
char*T_rule="游戏规则",*T_ctrl1="键盘上的“↑”“↓”“←”“→”控制蛇前进方向",*T_ctrl3="蛇吃到食物（方块）时长大一节",*T_ctrl4="蛇头碰到自身或者碰到边界游戏结束"; 
char*T_ctrl2="空格键可控制游戏暂停/开始"; 
char*T_design="——Designed By WildWolf",*T_know="我知道啦";
char T_score[6]="00000",*T_SCORE="得分";
void inttochar(int score);
int cf(int,int);
void paint_score(int score);
void paint_key(void);
void paint_stkey(void);
void paint_win(int [][W_num]);
void init_win(void);
void paint_stwin(void);
void key_clean(void);
void gameover(int);
void timer(int timerID);
void paint_score(int score)
{
	inttochar(score);
	beginPaint();
	setTextColor(RED);
	setTextSize(40);
	setTextBkColor(win_color);
	paintText(W-120,450,T_SCORE);
	setTextSize(50);
	setTextColor(T_color);
	paintText(W-140,510,T_score);
	endPaint();
}
int cf(int a,int b)
{
	int c;
	c=a;
	int t;
	if(b==0)
	return 1;
	else
	{
	for(t=1;t<b;t++)
	a*=c;
	return a;
    }
}
void inttochar(int score)
{
	int a,t,b;
	b=score;
	for(a=5;a>0;a--)
	{
		t=b/cf(10,a-1);
		b=b%cf(10,a-1);
		switch(t)
		{
			case 0:T_score[5-a]='0';break;
			case 1:T_score[5-a]='1';break;
			case 2:T_score[5-a]='2';break;
			case 3:T_score[5-a]='3';break;
			case 4:T_score[5-a]='4';break;
			case 5:T_score[5-a]='5';break;
			case 6:T_score[5-a]='6';break;
			case 7:T_score[5-a]='7';break;
			case 8:T_score[5-a]='8';break;
			case 9:T_score[5-a]='9';break;
		}
	}
}
void timer(int timerID)
{
	if(timerID==0)
	{
		paint_score(score);
		gameover(GAMEOVER);
		if(GAMEOVER==0)
	   ar_move(drection);
	   board_st=1;
	}
}
int Setup()
{
	init_win();
	paint_key();
	init_ar();
	init_snake();
	registerMouseEvent(mouselistener);
	registerTimerEvent(timer);
	registerKeyboardEvent(keyboardlistener);
	paint_stwin();
	paint_score(score);
	return 0;
}
void paint_stwin(void)
{
	beginPaint();
	setPenColor(BLACK);
	setBrushColor(BLACK);
	roundrect(To_left,To_top,W-To_right,H-To_bottom,R_round,R_round);
	setTextColor(RED);
	setTextSize(80);
	setTextBkColor(BLACK);
	paintText((W-To_left-To_right)/2-130,To_top+50,T_rule);
	setTextColor(T_color);
	setTextSize(30);
	paintText(To_left+170,To_top+200,T_ctrl1);
	paintText(To_left+330,To_top+270,T_ctrl2);
	paintText(To_left+300,To_top+340,T_ctrl3);
	paintText(To_left+270,To_top+410,T_ctrl4);
	setTextSize(30);
	paintText(W-600,To_top+480,T_design);
	endPaint();
	paint_stkey();
}
void gameover(int GAMEOVER)
{
	if(GAMEOVER!=0)
	{
		cancelTimer(0);
		beginPaint();
		setBrushColor(gameover_color);
		setPenColor(gameover_color);
		rectangle(0,H/2,W,H/2+100);
		setTextBkColor(gameover_color);
		setTextSize(100);
		paintText(W/2-250,H/2,T_gameover);
		endPaint();
	}
}
void free_all(void)
{
	Snake*current;
	current=head;
	while(current!=NULL)
	{
		free(current);
		current=current->next_body;
	}
}
void init_snake(void)
{
	drection=UP;
	x=W_num/2;
	y=H_num/2+5;
	head=(Snake*)malloc(sizeof(Snake));
    tail=(Snake*)malloc(sizeof(Snake));
	head->body=&ar[y][x];
    head->next_body=tail;
    tail->body=&ar[y+1][x];
    tail->next_body=NULL;
    HEAD=1;
}
void ar_move(Drection drection)
{
	Snake*head_t=head;
	Snake*current;
	t=tail;
	current=(Snake*)malloc(sizeof(Snake));
	switch(drection)
	{
		case UP:current->body=&ar[--y][x];
		        current->next_body=head;
		        head=current;
		     if(ar[y][x]==1)
		         {
		        if(y!=food_y||x!=food_x)
		        {
		            GAMEOVER=1;
		            head=head_t;
		        }
		        else
		             {
		       	 eat_or_n=0;
		        score+=10;
		        launch_food();
			    }
			    
		        break;
		        }
		        while(current->next_body!=tail)
		        current=current->next_body;
		        *(current->next_body->body)=0;
		        current->next_body=NULL;
		        tail=current;
		        free(t);
		        break;
		case RIGHT:current->body=&ar[y][++x];
		        current->next_body=head;
		        head=current;
		        if(ar[y][x]==1)
		         {
		        if(y!=food_y||x!=food_x)
		        {
		            GAMEOVER=1;
		            head=head_t;
		        }
		        else
		             {
		          eat_or_n=0;    	
		        score+=10;
		        launch_food();
			    }	
			   
		        break;
		        }
		        while(current->next_body!=tail)
		        current=current->next_body;
		        *(current->next_body->body)=0;
		        current->next_body=NULL;
		        tail=current;
		        free(t);
		        break;
		case DOWN:current->body=&ar[++y][x];
		        current->next_body=head;
		        head=current;
	      	   if(ar[y][x]==1)
		         {
		        if(y!=food_y||x!=food_x)
		        {
		            GAMEOVER=1;
		            head=head_t;
		        }
		        else
		             {
		       	 eat_or_n=0;
		        score+=10;
		        launch_food();
			    }
			   
		        break;
		        }
		        while(current->next_body!=tail)
		        current=current->next_body;
		        *(current->next_body->body)=0;
		        current->next_body=NULL;
		        tail=current;
		        free(t);
		        break;
		case LEFT:current->body=&ar[y][--x];
		        current->next_body=head;
		        head=current;
		        if(ar[y][x]==1)
		         {
		        if(y!=food_y||x!=food_x)
		        {
		            GAMEOVER=1;
		            head=head_t;
		        }
		        else
		        {
		       	eat_or_n=0;
		        score+=10;
		        launch_food();
			    }
			    
		        break;
		        }
		        while(current->next_body!=tail)
		        current=current->next_body;
		        *(current->next_body->body)=0;
		        current->next_body=NULL;
		        tail=current;
		        free(t);
		        break;				        
		        
	}
	current=head;
	while(current!=NULL)
	{
		*(current->body)=1;
		current=current->next_body;
	}
	if(x<0||x>=W_num||y<0||y>=H_num)
	GAMEOVER=1;
	else
	{
	    paint_win(ar);
    }
}
void launch_food(void)
{
	if(GAMEOVER==0&&eat_or_n==0)
	{
	srand((unsigned int)time(0));
	do
	{
	food_x=Food_x;
	food_y=Food_y;
    }while(ar[food_y][food_x]!=0);
    ar[food_y][food_x]=1;
	paint_win(ar);
	eat_or_n=1;
    }
}
void init_ar(void)
{
		int i,j;
	for(i=0;i<H_num;i++)
	{
		for(j=0;j<W_num;j++)
		{
		ar[i][j]=0;
     	} 
	}
}
void init_win(void)
{
	initWindow("贪吃蛇",170,10,W,H);
	beginPaint();
	setPenColor(win_color);
	setBrushColor(win_color);
	rectangle(0,0,W,H);
	setPenColor(BLACK);
	setBrushColor(BLACK);
	roundrect(To_left,To_top,W-To_right,H-To_bottom,R_round,R_round);
	endPaint();
}
void replay(void)
{
	score=0;
	ar_i=ar_j=0;
		int i,j;
	for(i=0;i<H_num;i++)
	{
		for(j=0;j<W_num;j++)
		{
		ar[i][j]=0;
     	} 
	}
	paint_win(ar);
	cancelTimer(0);
	GAMEOVER=0;
	beginPaint();
	setPenColor(win_color);
	setBrushColor(win_color);
	rectangle(0,H/2,To_left,H/2+100);
	rectangle(W-To_right,H/2,W,H/2+100);
	setPenColor(BLACK);
	setBrushColor(BLACK);
	roundrect(To_left,To_top,W-To_right,H-To_bottom,R_round,R_round);
	endPaint();

	
}
void key_clean(void)
{
	beginPaint();
	setPenColor(win_color);
	setBrushColor(win_color);
	roundrect(W-To_right+Key_dis,Key_totop,W-Key_dis+dx,Key_totop+Key_h+dy,R_round,R_round);
	roundrect(W-To_right+Key_dis,Key_totop+Key_h+Key_key,W-Key_dis+dx,Key_totop+Key_key+2*Key_h+dy,R_round,R_round);
	roundrect(W-To_right+Key_dis,Key_totop+(Key_h+Key_key)*2,W-Key_dis+dx,Key_totop+Key_key*2+3*Key_h+dy,R_round,R_round);
	roundrect(W-To_right+Key_dis,Key_totop+(Key_h+Key_key)*3,W-Key_dis+dx,Key_totop+Key_key*3+4*Key_h+dy,R_round,R_round);
	endPaint();
}
void paint_key(void)
{
	key_clean();
	beginPaint();
	setPenColor(Key_color_st);
	setBrushColor(Key_color_st);
	roundrect(W-To_right+Key_dis,Key_totop,W-Key_dis,Key_totop+Key_h,R_round,R_round);
	setPenColor(Key_color_pa);
	setBrushColor(Key_color_pa);
	roundrect(W-To_right+Key_dis,Key_totop+Key_h+Key_key,W-Key_dis,Key_totop+Key_key+2*Key_h,R_round,R_round);
	setBrushColor(Key_color_re);
	setPenColor(Key_color_re);
	roundrect(W-To_right+Key_dis,Key_totop+(Key_h+Key_key)*2,W-Key_dis,Key_totop+Key_key*2+3*Key_h,R_round,R_round);
	setBrushColor(Key_color_ex);
	setPenColor(Key_color_ex);
	roundrect(W-To_right+Key_dis,Key_totop+(Key_h+Key_key)*3,W-Key_dis,Key_totop+Key_key*3+4*Key_h,R_round,R_round);
	setTextColor(T_color);
	setTextSize(30);
	setTextBkColor(Key_color_st);
	paintText(W-To_right+Key_dis+35,Key_totop+10,T_start);
	setTextBkColor(Key_color_pa);
	paintText(W-To_right+Key_dis+35,Key_totop+10+Key_key+Key_h,T_pause);
	setTextBkColor(Key_color_re);
	paintText(W-To_right+Key_dis+35,Key_totop+(Key_h+Key_key)*2+10,T_replay);
	setTextBkColor(Key_color_ex);
	paintText(W-To_right+Key_dis+35,Key_totop+(Key_h+Key_key)*3+10,T_exit);
	endPaint();
}
void paint_stkey(void)
{
	beginPaint();
	setPenColor(Key_color_stkey);
	setBrushColor(Key_color_stkey);
	roundrect(W-400,To_top+600,W-250,To_top+650,R_round,R_round);
	setTextColor(T_color);
	setTextSize(30);
	setTextBkColor(Key_color_stkey);
	paintText(W-400+15,To_top+600+10,T_know);
	endPaint();
}
void exit_click(void)
{
	beginPaint();
	setPenColor(BLACK);
	setBrushColor(BLACK);
	roundrect(W-To_right+Key_dis,Key_totop+(Key_h+Key_key)*3,W-Key_dis,Key_totop+Key_key*3+4*Key_h,R_round,R_round);
	setBrushColor(Key_color_ex);
	setPenColor(Key_color_ex);
	roundrect(W-To_right+Key_dis+dx,Key_totop+(Key_h+Key_key)*3+dy,W-Key_dis+dx,Key_totop+Key_key*3+4*Key_h+dy,R_round,R_round);
	setTextColor(T_color);
	setTextSize(30);
	setTextBkColor(Key_color_ex);
	paintText(W-To_right+Key_dis+35+dx,Key_totop+(Key_h+Key_key)*3+10+dy,T_exit);
	endPaint();
}
void stkey_click(void)
{
    st_or_n=1;
    beginPaint();
    setPenColor(BLACK);
	setBrushColor(BLACK);
	roundrect(To_left,To_top,W-To_right,H-To_bottom,R_round,R_round);
	endPaint();
}
void replay_click(void)
{
	beginPaint();
	setPenColor(BLACK);
	setBrushColor(BLACK);
	roundrect(W-To_right+Key_dis,Key_totop+(Key_h+Key_key)*2,W-Key_dis,Key_totop+Key_key*2+3*Key_h,R_round,R_round);
	setBrushColor(Key_color_re);
	setPenColor(Key_color_re);
	roundrect(W-To_right+Key_dis+dx,Key_totop+(Key_h+Key_key)*2+dy,W-Key_dis+dx,Key_totop+Key_key*2+3*Key_h+dy,R_round,R_round);
	setTextColor(T_color);
	setTextSize(30);
	setTextBkColor(Key_color_re);
	paintText(W-To_right+Key_dis+35+dx,Key_totop+(Key_h+Key_key)*2+10+dy,T_replay);
	endPaint();
}
void start_click(void)
{
	beginPaint();
	setPenColor(BLACK);
	setBrushColor(BLACK);
	roundrect(W-To_right+Key_dis,Key_totop,W-Key_dis,Key_totop+Key_h,R_round,R_round);
	setBrushColor(Key_color_st);
	setPenColor(Key_color_st);
	roundrect(W-To_right+Key_dis+dx,Key_totop+dy,W-Key_dis+dx,Key_totop+Key_h+dy,R_round,R_round);
	setTextColor(T_color);
	setTextSize(30);
	setTextBkColor(Key_color_st);
	paintText(W-To_right+Key_dis+35+dx,Key_totop+10+dy,T_start);
	endPaint();
}
void pause_click(void)
{
	beginPaint();
	setPenColor(BLACK);
	setBrushColor(BLACK);
	roundrect(W-To_right+Key_dis,Key_totop+Key_h+Key_key,W-Key_dis,Key_totop+Key_key+2*Key_h,R_round,R_round);
	setBrushColor(Key_color_pa);
	setPenColor(Key_color_pa);
	roundrect(W-To_right+Key_dis+dx,Key_totop+Key_h+Key_key+dy,W-Key_dis+dx,Key_totop+Key_key+2*Key_h+dy,R_round,R_round);
	setTextColor(T_color);
	setTextSize(30);
	setTextBkColor(Key_color_pa);
	paintText(W-To_right+Key_dis+35+dx,Key_totop+10+Key_key+Key_h+dy,T_pause);
	endPaint();
}
void keyboardlistener(int key,int event)
{
	if(st_or_n!=0)
	{
	if(event==0&&board_st!=0)
	{
		switch(key)
		{
			case 37:
			if(drection!=RIGHT)	
			{
			drection=LEFT;
			board_st=0;
		    }
			break;
			case 38:
			if(drection!=DOWN)	
			{
			drection=UP;
			board_st=0;
		    }
			break;
			case 39:
			if(drection!=LEFT)
			{	
			drection=RIGHT;
			board_st=0;
		    }
			break;
			case 40:
			if(drection!=UP)
			{
			drection=DOWN;
			board_st=0;
		    }
			break;
		}
	}
	if(event==0&&key==32)
	{
	if(SPACE==0)
	{
		SPACE=1;
		board_st=1;
		launch_food();
		startTimer(0,Time_step);
	}
	else
	{
		SPACE=0;
		board_st=0;
		cancelTimer(0);
		T_start=CONTINUE; 
	    paint_key();
	}
    }
    }
}
void mouselistener(int x,int y,int button,int event)
{
	if(st_or_n==0)
	{
	if(x>W-400&&x<W-250&&y>To_top+600&&y<To_top+650)
	{
		color_stkey=200;
		paint_stwin();
		if(button==1&&event==2)
		stkey_click();
	}
	else
	{
		color_stkey=100;
		paint_stwin();
	paint_key();
	}
    }
	if(st_or_n!=0)
	{
	if(x>W-To_right+Key_dis&&x<W-Key_dis&&y>Key_totop&&y<Key_totop+Key_h)
	{
	color_st=200;
	if(button==1&&event==0)
	start_click();
	else if(button==1&&event==2)
	{
		board_st=1;
		launch_food();
	startTimer(0,Time_step);
	SPACE=1;
	paint_key();
    }
	else
	paint_key();
    }
	else if(x>W-To_right+Key_dis&&x<W-Key_dis&&y>Key_totop+Key_h+Key_key&&y<Key_totop+Key_key+2*Key_h)
	{
	color_pa=200;
	if(button==1&&event==0)
	pause_click();
	else if(button==1&&event==2)
	{
		board_st=0;
	cancelTimer(0);
	SPACE=0;
	T_start=CONTINUE; 
	paint_key();
    }
	else
	paint_key();
    }
    else if(x>W-To_right+Key_dis&&x<W-Key_dis&&y>Key_totop+2*(Key_h+Key_key)&&y<Key_totop+Key_key*2+3*Key_h)
    {
    color_re=200;
    if(button==1&&event==0)
	replay_click();
	else if(button==1&&event==2)
	{
		eat_or_n=0;
		board_st=0;
		SPACE=0;
	replay();
	free_all();
	init_snake();
	T_start=START;	
	paint_key();
	paint_score(score);	
	}
	else
	paint_key();
	}
	else if(x>W-To_right+Key_dis&&x<W-Key_dis&&y>Key_totop+3*(Key_h+Key_key)&&y<Key_totop+Key_key*3+4*Key_h)
    {
    color_ex=200;
    if(button==1&&event==0)
	exit_click();
	else if(button==1&&event==2)
	{
	paint_key();
	exit(0);
    }
	else
	paint_key();
	}
	else
	{
	color_ex=color_st=color_pa=color_re=100;
	paint_key();
    }
    }
}
void paint_win(int (*ar)[W_num])
{
	int i,j;
	beginPaint();
	setPenColor(BLACK);
	for(i=1;i<=H_num;i++)
	{
		for(j=1;j<=W_num;j++)
		{		
			if(ar[i-1][j-1]!=0)
			{
				if(HEAD!=0)
				{
				if(&ar[i-1][j-1]==head->body)
				setBrushColor(RGB(200,0,0));
				else if(i-1==food_y&&j-1==food_x)
			    setBrushColor(RGB(0,200,0));
				else
			    setBrushColor(RGB(200,170,0));
			    }
			    else
			    setBrushColor(RGB(0,100,100));
			    rectangle((To_left+R_round/2)*j,(To_top+R_round/2)*i,(To_left+R_round/2)*j+Dia_a,(To_top+R_round/2)*i+Dia_a);
		    }
			else
			{
				setBrushColor(RGB(0,0,0));
			    rectangle((To_left+R_round/2)*j,(To_top+R_round/2)*i,(To_left+R_round/2)*j+Dia_a,(To_top+R_round/2)*i+Dia_a);
			}
				
		}
	}
	endPaint();
}
