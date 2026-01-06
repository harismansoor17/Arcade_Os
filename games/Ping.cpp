#include<iostream>
#include<conio.h>
#include<windows.h>
#include<time.h>

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

char paddle[] = "?????";
int gameMode = 1; // 1 = practice, 2 = two player

void gotoxy(int x, int y) {
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible) {
    CONSOLE_CURSOR_INFO c;
    c.bVisible = visible;
    c.dwSize = 20;
    SetConsoleCursorInfo(console, &c);
}

/* ---------- UI ---------- */

void border() {
    for (int i = 2; i <= 79; i++) {
        gotoxy(i, 1); cout << "-";
        gotoxy(i, 25); cout << "-";
    }
    for (int i = 1; i <= 25; i++) {
        gotoxy(2, i); cout << "|";
        gotoxy(79, i); cout << "|";
    }
}

void erasePaddle(int x, int y) {
    for (int i = 0; i < 5; i++) {
        gotoxy(x, y + i);
        cout << " ";
    }
}

void drawPaddle(int x, int y) {
    for (int i = 0; i < 5; i++) {
        gotoxy(x, y + i);
        cout << "?";
    }
}

/* ---------- GAME ---------- */

void playGame() {
    system("cls");
    border();

    int lives1 = 3, lives2 = 3;

    int p1 = 10, p2 = 10;
    int x = 40, y = 12;
    int dx = -1, dy = -1;

    while (true) {
        gotoxy(x, y); cout << " ";
        x += dx;
        y += dy;

        if (y <= 5 || y >= 24) dy = -dy;

        // Paddle collision
        if (x == 6 && y >= p1 && y <= p1 + 4) dx = 1;
        if (x == 75 && y >= p2 && y <= p2 + 4) dx = -1;

        // Miss logic
        if (x <= 3) {
            if (gameMode == 2) lives1--;
            x = 40; y = 12; dx = 1;
        }
        if (x >= 78) {
            if (gameMode == 2) lives2--;
            x = 40; y = 12; dx = -1;
        }

        // Game over (2 Player only)
        if (gameMode == 2 && (lives1 == 0 || lives2 == 0)) {
            system("cls");
            border();

            gotoxy(28, 12);
            cout << (lives1 == 0 ? "PLAYER 2 WINS!" : "PLAYER 1 WINS!");

            gotoxy(24, 14);
            cout << "Rematch? (Y / N)";

            char ch;
            do { ch = getch(); } while (ch!='y'&&ch!='Y'&&ch!='n'&&ch!='N');

            if (ch == 'y' || ch == 'Y') {
                playGame();
                return;
            }
            return;
        }

        // AI paddle (Practice Mode)
        if (gameMode == 1) {
            erasePaddle(76, p2);
            if (y > p2 + 2 && p2 < 19) p2++;
            if (y < p2 + 2 && p2 > 6) p2--;
        }

        gotoxy(x, y); cout << "O";

        drawPaddle(5, p1);
        drawPaddle(76, p2);

        gotoxy(4, 3);
        if (gameMode == 1)
            cout << "PRACTICE MODE";
        else
            cout << "P1 Lives: " << lives1 << "   P2 Lives: " << lives2;

        Sleep(50);

        if (kbhit()) {
            char ch = getch();

            erasePaddle(5, p1);
            if (gameMode == 2) erasePaddle(76, p2);

            if ((ch == 'w' || ch == 'W') && p1 > 6) p1--;
            if ((ch == 's' || ch == 'S') && p1 < 19) p1++;

            if (gameMode == 2) {
                if (ch == 72 && p2 > 6) p2--;
                if (ch == 80 && p2 < 19) p2++;
            }

            if (ch == 27) return;
        }
    }
}

/* ---------- MODE SELECT ---------- */

void selectMode() {
    system("cls");
    gotoxy(25,5); cout<<"----------------------------";
    gotoxy(25,6); cout<<"|         Ping Pong        |";
    gotoxy(25,7); cout<<"----------------------------";  
    gotoxy(25, 12); cout << "1. Practice Mode";
    gotoxy(25, 14); cout << "2. Two Player";
    gotoxy(25, 16); cout << "Choose: ";

    char ch = getch();
    if (ch == '1') { gameMode = 1; playGame(); }
    if (ch == '2') { gameMode = 2; playGame(); }
}

/* ---------- MAIN ---------- */

int main() {
    setcursor(false);
    srand(time(0));
    selectMode();
    return 0;
}

