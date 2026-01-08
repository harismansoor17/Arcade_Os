#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
using namespace std;


// ???? ALERT ???? 
// If you got the Code From Git Hub 
// Then For Games to Run 
// Pls Change The Line 245
//system(("\"D:\\Gaming_Arcade_OS\\games\\" + games[realIndex] + ".exe\"").c_str());

// Change the Path "D:\\Gaming_Arcade_OS\\games\\"
// To The Path Where You Have the Games Stored
// An Example Change Whould Look Like This
// system(("\"C:\\Users\\YourName\\Documents\\ArcadeGames\\" + games[realIndex] + ".exe\"").c_str());

// Here Are the Codes:
// Gamer Codes: 1, 2, 3
// Admin Codes: 1122, 3000, 3130

//Thank You :)


// ===== FILES =====
const string G_CODES_FILE = "gamer_codes.txt";
const string A_CODES_FILE = "admin_codes.txt";
const string GAME_STATUS_FILE = "game_status.txt";

// ===== Utility =====
void clear() { system("cls"); }
void sleep(int ms) { Sleep(ms); }
void color(int c) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c); }

// ===== Animated Arcade Logo =====
void arcadeLogo() {
    for (int i = 0; i < 2; i++) {
        clear();
        color(10); // Green
        cout <<
               "================================================\n"
               "   ###   ####    ####    ###   ####    #####\n"
               "  #   #  #   #  #       #   #  #   #   #\n"
               "  #####  ####   #       #####  #   #   ####\n"
               "  #   #  #  #   #       #   #  #   #   #\n"
               "  #   #  #   #   ####   #   #  ####    #####\n"
               "================================================\n";

        sleep(400);

        clear();
        sleep(200);
    }
}

// ===== Game Logos =====
void brickBreaker() {
    color(12); // Red
    cout <<
    "====================================\n"
    "        BRICK BREAKER\n"
    "====================================\n"
    " [####] [####] [####] [####]\n"
    " [####] [####] [####] [####]\n"
    " [####] [####] [####] [####]\n"
    "------------------------------------\n";
}

void pingPong() {
    color(11); // Cyan
    cout <<
    "====================================\n"
    "           PING  PONG\n"
    "====================================\n"
    "| |                          | |\n"
    "| |            O             | |\n"
    "| |                          | |\n"
    "====================================\n";
}

void spaceShooter() {
    color(13); // Magenta
    cout <<
    "====================================\n"
    "        SPACE DEFENDERS\n"
    "====================================\n"
    "           /\\\n"
    "          |==|\n"
    "         /====\\\n"
    "           ||\n"
    "          /||\\\n"
    "====================================\n";
}


// ===== Game Info =====
string games[3] = {"Brick Breaker", "Ping Pong", "Space Defenders"};
bool gameEnabled[3];
int gameCount = 3;

// ===== Databases =====
int g_dataBase[3];
int a_dataBase[3];

// ===== File Handling =====
void saveCodes() {
    ofstream g(G_CODES_FILE), a(A_CODES_FILE);
    for (int i = 0; i < gameCount; i++) {
        g << g_dataBase[i] << endl;
        a << a_dataBase[i] << endl;
    }
}

void loadCodes() {
    ifstream g(G_CODES_FILE), a(A_CODES_FILE);
    if (!g || !a) {
        int gd[3] = {1, 2, 3};
        int ad[3] = {12, 13, 14};
        for (int i = 0; i < gameCount; i++) {
            g_dataBase[i] = gd[i];
            a_dataBase[i] = ad[i];
        }
        saveCodes();
        return;
    }
    for (int i = 0; i < gameCount; i++) {
        g >> g_dataBase[i];
        a >> a_dataBase[i];
    }
}

void saveGameStatus() {
    ofstream f(GAME_STATUS_FILE);
    for (int i = 0; i < gameCount; i++)
        f << gameEnabled[i] << endl;
}

void loadGameStatus() {
    ifstream f(GAME_STATUS_FILE);
    if (!f) {
        for (int i = 0; i < gameCount; i++)
            gameEnabled[i] = true;
        saveGameStatus();
        return;
    }
    for (int i = 0; i < gameCount; i++)
        f >> gameEnabled[i];
}

// ===== Admin =====
void showGames() {
    for (int i = 0; i < gameCount; i++) {
        cout << i + 1 << ". " << games[i];
        if (!gameEnabled[i]) cout << " (DISABLED)";
        cout << endl;
    }
}

void showCodes() {
    cout << "\nGamer Codes:\n";
    for (int i = 0; i < gameCount; i++) {
        cout << g_dataBase[i] << (i == gameCount - 1 ? "" : ", " );
    }
     

    cout << "\n\nAdmin Codes:\n";
    for (int i = 0; i < gameCount; i++) {
        cout <<  a_dataBase[i] << (i == gameCount - 1 ? "" : ", " );
    }
    cout<<endl;
}


void adminPanel() {
    int choice;
    do {
        clear();
        cout << "===== ADMIN PANEL =====\n";
        cout << "1. Enable/Disable Game\n";
        cout << "2. View Status\n";
        cout << "3. Edit Codes\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) {
            showGames();
            int g;
            cout << "Toggle game #: ";
            cin >> g;
            if (g >= 1 && g <= gameCount) {
                gameEnabled[g - 1] = !gameEnabled[g - 1];
                saveGameStatus();
            }
            system("pause");
        }
        else if (choice == 2) {
            showGames();
            showCodes();
            system("pause");
        }
        else if (choice == 3) {
            int t, i, c;
            cout << "1.Gamer  2.Admin : ";
            cin >> t;
            cout << "Index: ";
            cin >> i;
            cout << "New code: ";
            cin >> c;
            if (i >= 1 && i <= gameCount) {
                if (t == 1) g_dataBase[i - 1] = c;
                if (t == 2) a_dataBase[i - 1] = c;
                saveCodes();
            }
            system("pause");
        }
        
    } while (choice != 0);
}

// ===== Gamer Menu =====
void gamerMenu() {
    clear();
    cout << "===== SELECT YOUR GAME =====\n\n";

    int mapIndex[3];
    int visibleCount = 0;

    for (int i = 0; i < gameCount; i++) {
        if (gameEnabled[i]) {
        	
            // show logo ONLY if enabled
            if (i == 0) brickBreaker();
            if (i == 1) pingPong();
            if (i == 2) spaceShooter();
            
            mapIndex[visibleCount] = i;
            cout << visibleCount + 1 << ". " << games[i] << endl;

            cout << endl;
            visibleCount++;
        }
    }

    if (visibleCount == 0) {
        cout << "No games available!\n";
        system("pause");
        return;
    }

    int choice;
    cout << "Select Game: ";
    cin >> choice;

    if (choice >= 1 && choice <= visibleCount) {
        int realIndex = mapIndex[choice - 1];
        system(("\"D:\\Gaming_Arcade_OS\\games\\" + games[realIndex] + ".exe\"").c_str());
    } else {
        cout << "Invalid selection!\n";
        system("pause");
    }
}

// ===== MAIN =====
int main() {
    arcadeLogo();
    loadCodes();
    loadGameStatus();

    int code;
    bool gamer = false, admin = false;

    cout << "Insert Access Code: ";
    cin >> code;

    for (int i = 0; i < gameCount; i++) {
        if (code == g_dataBase[i]) gamer = true;
        if (code == a_dataBase[i]) admin = true;
    }

    if (admin) {
        adminPanel();
        return 0;  
    }
    else if (gamer) {
        gamerMenu();
        return 0;   
    }
    else {
        cout << "ACCESS DENIED!\n";
        cout << "Psst.. Loook in The Gamer Code File or Admin Codes File or Just see The top comment in your Code Editor\n";
        system("pause");
        return 0;   // ?? EXIT PROGRAM
    }
}

