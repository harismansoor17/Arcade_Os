#include<iostream>
#include<conio.h>
#include<dos.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include<time.h>

#define SCREEN_WIDTH 52
#define SCREEN_HEIGHT 20

#define MINX 2
#define MINY 2
#define MAXX 49
#define MAXY 19

using namespace std;
 
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int bricks[24][2] = {
						{2,7},{2,12},{2,17},{2,22},{2,27},{2,32},{2,37},{2,42},
					  	{4,7},{4,12},{4,17},{4,22},{4,27},{4,32},{4,37},{4,42},
					  	{6,7},{6,12},{6,17},{6,22},{6,27},{6,32},{6,37},{6,42}
					};

int visibleBricks[24] = {1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1};
int sliderPos[2] = {18,22}; 
int ballPos[2] = {17,26}; 
int startBall = 0;
int dir = 1; // 1- TopRight, 2- TopLeft, 3- BottomLeft, 4-BottomRight
int bricksLeft = 24;
int level = 1;
int lives = 3;
int win = 0;
int lose = 0;

void gotoxy(int x, int y)
{
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size) // set bool visible = 0 - invisible, bool visible = 1 - visible
{
	if(size == 0)
	{
		size = 20;	// default cursor size Changing to numbers from 1 to 20, decreases cursor width
	}
	CONSOLE_CURSOR_INFO lpCursor;	
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console,&lpCursor);
}

void drawBorder(){
	gotoxy(0,0);cout<<"----------------------------------------------------";
	gotoxy(0,SCREEN_HEIGHT);cout<<"----------------------------------------------------";
	
	for(int i=0; i<SCREEN_HEIGHT; i++){
		gotoxy(0,i); cout<<"|";
		gotoxy(SCREEN_WIDTH,i); cout<<"|";
	}
}

void drawBricks(){
    for(int i=0; i<24; i++){
        if( visibleBricks[i] == 1 ){ 
        
            // COLOR � every row different
            if(i < 8) SetConsoleTextAttribute(console, 12); // red
            else if(i < 16) SetConsoleTextAttribute(console, 14); // yellow
            else SetConsoleTextAttribute(console, 10); // green

            gotoxy(bricks[i][1], bricks[i][0]);
            cout<<"????";

            SetConsoleTextAttribute(console, 7); // reset
        }
    }
}


void ballHitSlider(){
	if( ballPos[1]>=sliderPos[1] && ballPos[1]<=sliderPos[1]+8){
		if( ballPos[0] == sliderPos[0]-1 ){
			if( dir == 3 ) 
				dir = 2;
			else if( dir == 4 )
				dir = 1;
		}
	}
}

void ballHitBrick(){
    for(int i=0; i<24; i++){
        if(visibleBricks[i]==1){

            int bx = bricks[i][1];
            int by = bricks[i][0];

            
            if(ballPos[1] >= bx && ballPos[1] <= bx+4 &&
               ballPos[0] == by)
            {
                visibleBricks[i] = 0;
                bricksLeft--;

                
                if(dir == 1) dir = 2;
                else if(dir == 2) dir = 1;
                else if(dir == 3) dir = 4;
                else if(dir == 4) dir = 3;
            }
        }
    }
}

void resetObjects() {
    
    sliderPos[0] = 18;
    sliderPos[1] = 22;

    ballPos[0] = 17;
    ballPos[1] = 26;
    startBall = 0;

    
    dir = 1;
}

void loadLevel(int lvl) {
    
    for(int i=0; i<24; i++)
        visibleBricks[i] = 1;

    bricksLeft = 24;

    
    if(lvl == 1) {
        
    }
    if(lvl == 2) {
        
        for(int i=0; i<24; i++)
            bricks[i][0] += 2;
    }
    if(lvl == 3) {
        
        for(int i=0; i<24; i++)
            bricks[i][1] += (i % 2 == 0 ? 2 : -2);
    }
}


void play(){
	while(1){
		system("cls");
		drawBricks();
		drawBorder();
		
		gotoxy(2, 21); cout<<"Level: "<<level;
        gotoxy(20, 21); cout<<"Lives: "<<lives;
		gotoxy(sliderPos[1],sliderPos[0]);
		cout<<"||||||||";

		gotoxy(ballPos[1],ballPos[0]);
		cout<<"O";
 
		if(kbhit()){
			char ch = getch();
			if( ch=='d'||ch=='D'|| ch==77 ){
				if(sliderPos[1] < 44)
					sliderPos[1] = sliderPos[1]+5;
			} 
			if( ch=='a'||ch=='A'|| ch==75 ){
				if(sliderPos[1] > 2)
					sliderPos[1] = sliderPos[1]-5;
			} 
			if(ch==32){
				startBall = 1;
			} 
			if(ch==27){
				break;
			}
		}
		
		if( startBall == 1 ){
			if( dir == 1) { // TOP RIGHT
				ballPos[0] = ballPos[0] - 1;
				ballPos[1] = ballPos[1] + 2;
				if( ballPos[1] > MAXX ){
					dir = 2;
				}  
				else if( ballPos[0] < MINY ){
					dir = 4;
				}   
			}
			else if( dir == 2) { // TOP LEFT
				ballPos[0] = ballPos[0] - 1;
				ballPos[1] = ballPos[1] - 2;
				if( ballPos[0] < MINY ){
					dir = 3;
				}  
				else if( ballPos[1] < MINX  ){
					dir = 1;
				}   
			}
			else if( dir == 3) { // BOTTOM LEFT
				ballPos[0] = ballPos[0] + 1;
				ballPos[1] = ballPos[1] - 2;
			  
			if(ballPos[0] > MAXY){  
			   lives--;
			     if(lives > 0){
			         resetObjects(); 
			         continue;       
			    } else {
			         lose = 1;
				     break;
			    }
            }

				else if( ballPos[1] < MINX  ){
					dir = 4;
				}    
			}
			else if( dir == 4) { 
				ballPos[0] = ballPos[0] + 1;
				ballPos[1] = ballPos[1] + 2;  
				if( ballPos[1] > MAXX ){
					dir = 3;
				} 
				if(ballPos[0] > MAXY){  
			        lives--;
			         if(lives > 0){
			            resetObjects(); 
			            continue;       
			       } else {
			            lose = 1;
				       break;
			       }
                }
			}

			ballHitSlider();
		}
		
		ballHitBrick();
	
		if(bricksLeft == 0){

        	if(level < 3){
      		level++;
        	loadLevel(level);
        	resetObjects();
        	continue;
    		}

       		win = 1;
       		break;
    	}
		

		Sleep(30);
	}
	
	if( lose == 1){
		system("cls");
		
		gotoxy(10,5); cout<<" --------------------- "; 
		gotoxy(10,6); cout<<" |     YOU LOSE      | "; 
		gotoxy(10,7); cout<<" --------------------- "; 			

		gotoxy(10,9); cout<<"Press any key to go back to Menu."; 	
		getch(); 
	}

	if( win == 1){
		system("cls");
		gotoxy(10,5); cout<<" --------------------- "; 
		gotoxy(10,6); cout<<" |     YOU WIN       | "; 
		gotoxy(10,7); cout<<" --------------------- "; 			

		gotoxy(10,9); cout<<"Press any key to go back to Menu.";
		getch(); 	  			 
	}
}

void resetGame() {
    // reset bricks visibility
    for(int i = 0; i < 24; i++){
        visibleBricks[i] = 1;
    }

    // reset ball position
    ballPos[0] = 17;
    ballPos[1] = 26;

    // reset slider position
    sliderPos[0] = 18;
    sliderPos[1] = 22;

    // reset ball movement
    startBall = 0;
    dir = 1;

    // reset bricks count
    bricksLeft = 24;

    // reset win/lose
    win = 0;
    lose = 0;
}




void instructions(){
	
	system("cls");
	cout<<"Instructions";
	cout<<"\n----------------";
	cout<<"\n1. Use 'a' key to move slider to left";
	cout<<"\n2. Use 'd' key to move slider to right";
	cout<<"\n3. Press spacebar to start game";
	cout<<"\n\nPress any key to go back to menu";
	getch();
}

int main()
{
	setcursor(0,0);  
	
	do{
		system("cls");
		gotoxy(10,5); cout<<" -------------------------- "; 
		gotoxy(10,6); cout<<" |     BRICK BREAKER      | "; 
		gotoxy(10,7); cout<<" --------------------------";
		gotoxy(10,9); cout<<"1. Start Game";
		gotoxy(10,10); cout<<"2. Instructions";	 
		gotoxy(10,11); cout<<"3. Quit";
		gotoxy(10,13); cout<<"Select option: ";
		char op = getche();
		
		if( op=='1') {
        resetGame();
	    play();
		}
		else if( op=='2') instructions();
		else if( op=='3') exit(0);
		
	}while(1);

	play();

	cout<<endl<<endl;	
	return 0;
}
