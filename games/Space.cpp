#include<iostream>
#include<conio.h>
#include<dos.h>
#include<windows.h>
#include<time.h>
#include<fstream>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int enemyY[2], enemyX[2];
char bird[3][5] = {
    ' ',' ','^',' ',' ',
    '|','^','^','^','|',
    '^','^','^','^','^'
};

int birdPos = WIN_WIDTH/2;
int score = 0;
int bullets[20][4];
int bIndex = 0;
int totalMissiles = 0;    
int escapedMissiles = 0;  
int mothershipLives = 3;  
bool mothershipCheat = false;
bool bossPhase = false;   
bool godMode = false;
int bossX;
int bossY = 2;
int bossDir = 1;      
int bossLives = 15;   
struct BossBullet {
    int x, y;
    int dx, dy;
    bool active;
};
BossBullet bossBullets[30];
int playerLives = 3;
bool bossCheatUsed = false;


void gotoxy(int x,int y){
    CursorPosition.X=x;
    CursorPosition.Y=y;
    SetConsoleCursorPosition(console,CursorPosition);
}

void setcursor(bool visible){
    CONSOLE_CURSOR_INFO cci;
    cci.bVisible=visible;
    cci.dwSize=20;
    SetConsoleCursorInfo(console,&cci);
}


void drawBorder(){
    for(int i=0;i<=SCREEN_WIDTH;i++){
        gotoxy(i,SCREEN_HEIGHT); cout<<"#";
    }
    for(int i=0;i<=SCREEN_HEIGHT;i++){
        gotoxy(0,i); cout<<"#";
        gotoxy(SCREEN_WIDTH,i); cout<<"#";
        gotoxy(WIN_WIDTH,i); cout<<"#";
    }
}

void updateScore(){
    if(bossPhase){
        gotoxy(WIN_WIDTH+2,5); cout<<"Boss HP   : "<<bossLives<<"   ";
        gotoxy(WIN_WIDTH+2,6); cout<<"Player HP : "<<playerLives<<"   ";
    }
    else{
        gotoxy(WIN_WIDTH+2,5); cout<<"Score      : "<<score<<"   ";
        gotoxy(WIN_WIDTH+2,6); cout<<"Missiles   : "<<totalMissiles<<"/10   ";
        gotoxy(WIN_WIDTH+2,7); cout<<"Mothership : "<<mothershipLives<<"   ";
    }
}


void genEnemy(int i){
    enemyX[i]=3+rand()%(WIN_WIDTH-10);
}

void drawEnemy(int i){
    gotoxy(enemyX[i],enemyY[i]);   cout<<"*..*";
    gotoxy(enemyX[i],enemyY[i]+1); cout<<"****";
    gotoxy(enemyX[i],enemyY[i]+2); cout<<"****";
    gotoxy(enemyX[i],enemyY[i]+3); cout<<" ** ";
}

void eraseEnemy(int i){
    for(int j=0;j<4;j++){
        gotoxy(enemyX[i],enemyY[i]+j); cout<<"    ";
    }
}

void resetEnemy(int i){
    eraseEnemy(i);
    enemyY[i]=1;
    genEnemy(i);
}

void drawBoss(){
    gotoxy(bossX, bossY);     cout<<"  #####  ";
    gotoxy(bossX, bossY+1);   cout<<" ######### ";
    gotoxy(bossX, bossY+2);   cout<<"###  ###  ###";
    gotoxy(bossX, bossY+3);   cout<<" ######### ";
    gotoxy(bossX, bossY+4);   cout<<"  #####  ";
}

void moveBoss(){
    bossX += bossDir;

    if(bossX <= 2 || bossX >= WIN_WIDTH - 15){
        bossDir *= -1;
    }
}


void eraseBoss(){
    for(int i = 0; i < 5; i++){
        gotoxy(bossX, bossY + i);
        cout<<"             ";
    }
}

void initBossBullets(){
    for(int i = 0; i < 30; i++)
        bossBullets[i].active = false;
}

void fireBossBullets(){
    int dirs[5][2] = {
        {0,1},   
        {-1,1},  
        {1,1},   
        {-2,1},  
        {2,1}    
    };

    for(int d = 0; d < 5; d++){
        for(int i = 0; i < 30; i++){
            if(!bossBullets[i].active){
                bossBullets[i].active = true;
                bossBullets[i].x = bossX + 6;
                bossBullets[i].y = bossY + 5;
                bossBullets[i].dx = dirs[d][0];
                bossBullets[i].dy = dirs[d][1];
                break;
            }
        }
    }
}

void moveBossBullets(){
    for(int i = 0; i < 30; i++){
        if(bossBullets[i].active){
            bossBullets[i].x += bossBullets[i].dx;
            bossBullets[i].y += bossBullets[i].dy;

            if(bossBullets[i].y > SCREEN_HEIGHT - 1 ||
               bossBullets[i].x < 1 ||
               bossBullets[i].x > WIN_WIDTH - 1){
                bossBullets[i].active = false;
            }
        }
    }
}

void drawBossBullets(){
    for(int i = 0; i < 30; i++){
        if(bossBullets[i].active){
            gotoxy(bossBullets[i].x, bossBullets[i].y);
            cout<<"o";
        }
    }
}

bool bossBulletHitPlayer(){
    for(int i = 0; i < 30; i++){
        if(bossBullets[i].active){
            if(bossBullets[i].y >= 22 &&
               bossBullets[i].x >= birdPos &&
               bossBullets[i].x <= birdPos + 4){

                bossBullets[i].active = false; 
                return true;
            }
        }
    }
    return false;
}




void eraseBossBullets(){
    for(int i = 0; i < 30; i++){
        if(bossBullets[i].active){
            gotoxy(bossBullets[i].x, bossBullets[i].y);
            cout<<" ";
        }
    }
}

bool bulletHitBoss(){
    for(int i = 0; i < 20; i++){
        if(bullets[i][0] > 0){
            int bx = bullets[i][1];
            int by = bullets[i][0];

            if(by >= bossY && by <= bossY + 4 &&
               bx >= bossX && bx <= bossX + 12){

                bullets[i][0] = bullets[i][2] = 0;
                bossLives--;
                updateScore();
                return true;
            }
        }
    }
    return false;
}




void genBullet(){
    bullets[bIndex][0]=22;
    bullets[bIndex][1]=birdPos;
    bullets[bIndex][2]=22;
    bullets[bIndex][3]=birdPos+4;
    bIndex=(bIndex+1)%20;
}

void moveBullet(){
    for(int i=0;i<20;i++){
        if(bullets[i][0]>0) bullets[i][0]--;
        if(bullets[i][2]>0) bullets[i][2]--;
    }
}

void drawBullets(){
    for(int i=0;i<20;i++){
        if(bullets[i][0]>0){
            gotoxy(bullets[i][1],bullets[i][0]); cout<<"|";
            gotoxy(bullets[i][3],bullets[i][2]); cout<<"|";
        }
    }
}

void eraseBullets(){
    for(int i=0;i<20;i++){
        gotoxy(bullets[i][1],bullets[i][0]); cout<<" ";
        gotoxy(bullets[i][3],bullets[i][2]); cout<<" ";
    }
}

void drawBird(){
    for(int i=0;i<3;i++)
        for(int j=0;j<5;j++)
            gotoxy(birdPos+j,22+i),cout<<bird[i][j];
}

void eraseBirdAt(int x){
    for(int i=0;i<3;i++)
        for(int j=0;j<5;j++)
            gotoxy(x+j,22+i), cout<<" ";
}



int collision(){
    for(int i = 0; i < 2; i++){
        if(enemyY[i] + 4 >= 22 &&
           enemyX[i] + 4 >= birdPos &&
           enemyX[i] <= birdPos + 4){

            
            resetEnemy(i);
            totalMissiles++;
            score++;
            updateScore();

            return 1;
        }
    }
    return 0;
}




int bulletHit(){
    for(int i=0;i<20;i++){
        for(int e=0;e<2;e++){
            if(bullets[i][0]>=enemyY[e] && bullets[i][0]<=enemyY[e]+4 &&
               bullets[i][1]>=enemyX[e] && bullets[i][1]<=enemyX[e]+4){

                gotoxy(bullets[i][1],bullets[i][0]); cout<<" ";
                gotoxy(bullets[i][3],bullets[i][2]); cout<<" ";
                bullets[i][0]=bullets[i][2]=0;

                resetEnemy(e);
				totalMissiles++;
				score++;
				updateScore();
				return 1;

            }
        }
    }
    return 0;
}


void gameover(){

    system("cls");
    cout<<"\n\t-------- GAME OVER --------\n\n";
    cout<<"\tScore     : "<<score<<endl;
    cout<<"\n\tPress any key...";
    getch();
}

void play(){
    system("cls");
    drawBorder();

    birdPos=WIN_WIDTH/2;
    score=0;
    totalMissiles = 0;
    escapedMissiles = 0;
    if(!mothershipCheat)  mothershipLives = 3;
    bossPhase = false;

    for(int i=0;i<2;i++){
        enemyY[i]=1;
        genEnemy(i);
    }
    updateScore();

    
    gotoxy(10,5); cout<<"Press any key to start";
    getch();
    gotoxy(10,5); cout<<"                      ";
    gotoxy(10,6); cout<<"                      ";
    gotoxy(10,7); cout<<"                      ";

    bool canShoot=true;

    while(1){
    	int oldBirdPos = birdPos;
        if(GetAsyncKeyState('A')&0x8000 && birdPos>2) birdPos-=2;
        if(GetAsyncKeyState('D')&0x8000 && birdPos<WIN_WIDTH-7) birdPos+=2;
        static bool cheatUsed = false;
		if(!bossPhase && !cheatUsed && (GetAsyncKeyState('C') & 0x8000)){
		    mothershipLives = 100;
		    mothershipCheat = true;
		    cheatUsed = true;
		    updateScore();
		}



        if(GetAsyncKeyState(VK_SPACE)&0x8000){
            if(canShoot){
                genBullet();
                canShoot=false;
            }
        }else canShoot=true;

        if(GetAsyncKeyState(VK_ESCAPE)&0x8000) break;

        drawBird();
        
        for(int i = 0; i < 2; i++){
          if(totalMissiles < 10){
            drawEnemy(i);
           }
        }

        drawBullets();
		collision();
        bulletHit();

        Sleep(120);

        eraseBirdAt(oldBirdPos);
        eraseEnemy(0); eraseEnemy(1);
        eraseBullets();
        moveBullet();

        enemyY[0]++; enemyY[1]++;
        for(int i = 0; i < 2; i++){
		    if(enemyY[i] > SCREEN_HEIGHT - 5){
		        escapedMissiles++;
		        mothershipLives--;
		        totalMissiles++;
				updateScore();		
		        resetEnemy(i);
		
		        if(mothershipLives <= 0){
		            gameover();
		            return;
		        }
		    }
       }
       if(totalMissiles >= 10){
		    bossPhase = true;
		    break;
		}


    }
    if(bossPhase){
	    system("cls");
	    drawBorder();
	
	    bossX = WIN_WIDTH / 2 - 6;
	    bossLives = 15;
	    playerLives = 3;
	    initBossBullets();
	    bossDir = 1;
        

	
	    gotoxy(20, 10);
	    cout<<"? ENEMY MOTHERSHIP APPROACHING ?";
	    Sleep(1500);
	    system("cls");
	    drawBorder();
	
	   int fireDelay = 0;
bool canShootBoss = true;

while(1){
	
	int oldBirdPos = birdPos;
    // EXIT
    if(GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        break;

    
    if(GetAsyncKeyState('A') & 0x8000 && birdPos > 2)
        birdPos -= 2;

    if(GetAsyncKeyState('D') & 0x8000 && birdPos < WIN_WIDTH - 7)
        birdPos += 2;

    if(GetAsyncKeyState(VK_SPACE) & 0x8000){
        if(canShootBoss){
            genBullet();
            canShootBoss = false;
        }
    } else canShootBoss = true;
    
    static bool cheatLock = false;

	if(!cheatLock){
	    static bool iLock = false;

		if((GetAsyncKeyState('I') & 0x8000) && !iLock){
		    godMode = true;
		    iLock = true;
		}
		
		if(!(GetAsyncKeyState('I') & 0x8000)){
		    iLock = false;
		}

	    if(GetAsyncKeyState('B') & 0x8000){
	        bossLives = 1;
	        cheatLock = true;
	    }
	    if(GetAsyncKeyState('L') & 0x8000){
	        playerLives += 5;
	        cheatLock = true;
	    }
	}

	if(!(GetAsyncKeyState('I') & 0x8000) &&
	   !(GetAsyncKeyState('B') & 0x8000) &&
	   !(GetAsyncKeyState('L') & 0x8000)){
	    cheatLock = false;
	}

    
    fireDelay++;

	if(fireDelay >= 12){   
	    fireBossBullets();
	    fireDelay = 0;
	}

	// ========= ERASE =========
	eraseBirdAt(oldBirdPos);
	eraseBossBullets();
	eraseBoss();
	eraseBullets();
	
	// ========= UPDATE =========
	moveBoss();
	moveBossBullets();
	moveBullet();
	
	// ========= DRAW =========
	drawBird();
	drawBoss();
	drawBossBullets();
	drawBullets();

    updateScore();

    // ========= COLLISIONS =========
 if(bossBulletHitPlayer()){
    if(!godMode){
        playerLives--;
        updateScore();

        if(playerLives <= 0){
            gameover();
            return;
        }
    }
}



    if(bulletHitBoss()){
        if(bossLives <= 0){
            system("cls");
            cout<<"\n\t?? YOU WIN! ??\n\n";
            cout<<"\tBoss Destroyed!\n";
            cout<<"\n\tFinal Score: "<<score<<"\n";
            Sleep(3000);  
			exit(0); 
        }
    }

    Sleep(90);
}
 }
	  }

	


int main(){
    setcursor(false);
    srand(time(0));

    while(1){
        system("cls");
        gotoxy(10,5); cout<<"----------------------------";
        gotoxy(10,6); cout<<"|       SPACE DEFENDERS       |";
        gotoxy(10,7); cout<<"----------------------------";
        gotoxy(10,9); cout<<"1. Start Game";
        gotoxy(10,10);cout<<"2. Instructions";
        gotoxy(10,11);cout<<"3. Quit";
        gotoxy(10,13);cout<<"Select option: ";

        char op=getch();
        if(op=='1') play();
        else if(op=='2'){
            system("cls");
            cout<<"\nControls:\nA = Left\nD = Right\nSpace = Shoot\nESC = Exit Game";
            cout<<"\n\nPress any key...";
            getch();
        }
        else if(op=='3') exit(0);
    }
}





