#include "game.h"


class XYkoord
{
public:
    int x;
    int y;
};
class Player: public XYkoord
{
public:
    int lives = 3;
    char kryptis;
    int lvl;
};
class Cannon : public XYkoord
{
public:
    int speed=40;
};
class Bullet : public XYkoord
{
public:
    bool exists;
    bool visible;
};

class Gyvybes 
{
public:
    int skaicius;


    Gyvybes( int g) : skaicius(g) {}

    void operator --()
    {
        skaicius = skaicius - 1;
    }
};

Player player;
char c;
char map[60][60];
Gyvybes lives(player.lives);

std::vector <std::pair <int, int > > CannonKoord;
std::vector <std::pair <int, int > > SpikeKoord;
std::vector <std::pair <int, int > > StarKoord;
std::vector <std::pair <int, int > > BlockKoord;

int framesDelay;
int frameCount;
int Cannonsize;
int Spikesize;
int Starsize;
int Blocksize;
bool consoleOff = false;
bool inGame= false;
bool dmg = false;
bool spike = true;
int bulletsk = 0;
char starkryptis[10] = {'d', 'd', 'k', 'd', 'k', 'd', 'k', 'd', 'k', 'k'};
char blockkryptis = 's';
Cannon cannon[60];
Bullet bullet[200];

void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void loadCut(std::string Cutscene)
{
    std::string Eilute;
    std::ifstream openfile(Cutscene.c_str());
    if (openfile.is_open())
    {
        while (!openfile.eof())
        {
            getline(openfile, Eilute);
            std::cout << Eilute << std::endl;
        }
    }
}
void loadmap(std::string level)    //Funkcija nuskaitanti zemelapi is failo
{
    std::string Eilute;
    int eil = 0;
    std::ifstream openfile(level.c_str());
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

    if (openfile.is_open())
    {
        
        while (!openfile.eof())
        {
            getline(openfile, Eilute);
            strcpy_s(map[eil], Eilute.c_str());
            std::cout << Eilute << std::endl;
            eil++;
        }
    }
}
void mapObjects()
{
    for (int i = 0; i < 60; i++)
    {
        for (int j = 0; j < 60; j++)
        {
            if (map[i][j] == '|') 
                CannonKoord.push_back(std::make_pair(i, j));
            
            else if (map[i][j] == '*')
                StarKoord.push_back(std::make_pair(i, j));

            else if (map[i][j] == '^')
                SpikeKoord.push_back(std::make_pair(i, j));

            else if (map[i][j] == 'X')
                BlockKoord.push_back(std::make_pair(i, j));
        }
    }
    bool Cannontrue = true;
    bool Startrue = true;
    bool Spiketrue = true;
    bool Blocktrue = true;

    Cannonsize = CannonKoord.size();
    Starsize = StarKoord.size();
    Spikesize = SpikeKoord.size();
    Blocksize = BlockKoord.size();

    while ((Cannontrue == true) || (Startrue == true) || (Spiketrue == true) || (Blocktrue == true))
    {
        if (CannonKoord.size() == 0)
            Cannontrue = false;
        if (StarKoord.size() == 0)
            Startrue = false;
        if (SpikeKoord.size() == 0)
            Spiketrue = false;
        if (BlockKoord.size() == 0)
            Blocktrue = false;

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);

        for (int i = 0; i < Cannonsize; i++)
        {
            gotoxy(CannonKoord[i].second, CannonKoord[i].first);
            std::cout << "|";
            Cannontrue = false;
        }
        for (int i = 0; i < Starsize; i++)
        {
            gotoxy(StarKoord[i].second, StarKoord[i].first);
            std::cout << "*";
            Startrue = false;
        }
        for (int i = 0; i < Spikesize; i++)
        {
            gotoxy(SpikeKoord[i].second, SpikeKoord[i].first);
            std::cout << "^";
            Spiketrue = false;
        }
        for (int i = 0; i < Blocksize; i++)
        {
            gotoxy(BlockKoord[i].second, BlockKoord[i].first);
            std::cout << "X";
            Blocktrue = false;
        }
    }

}
void clearObjects() {
    CannonKoord.clear();
    StarKoord.clear();
    SpikeKoord.clear();
    BlockKoord.clear();
    for (int i = 0; i < 200; i++)
    {
        bullet[i].exists = false;
    }
}

char getCursorChar()    //funkcija gaunanti zaidejo pozicija
{
    char c = '\0';
    CONSOLE_SCREEN_BUFFER_INFO con;
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hcon != INVALID_HANDLE_VALUE &&
        GetConsoleScreenBufferInfo(hcon, &con))
    {
        DWORD read = 0;
        if (!ReadConsoleOutputCharacterA(hcon, &c, 1,
            con.dwCursorPosition, &read) || read != 1
            )
            c = '\0';
    }
    return c;
}
char read(int x, int y)  //funkcija skaitanti koordinates
{
    gotoxy(x, y);
    char ccccc = getCursorChar();
    return ccccc;
}
void charPlayer()
{
    c = '0';
    if (_kbhit() == 1)
    {
        c = _getch();
    }
}


void meniu()
{
    char c = '0';
    int choice = 2;
    std::system("CLS");
    loadCut("meniu.txt");
    while ((consoleOff == false) && (inGame == false)) {
        while (c != char(13))
        {
            c = _getch();
            if (c == 'a') {
                choice = 1;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                gotoxy(17, 12);
                std::cout << "START";

                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                gotoxy(30, 12);
                std::cout << "QUIT";
            }
            if (c == 'd') {
                choice = 2;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                gotoxy(17, 12);
                std::cout << "START";

                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                gotoxy(30, 12);
                std::cout << "QUIT";
            }
        }
        if (choice == 1)
        {
            inGame = true;
        }
        if (choice == 2)
        {
            inGame = false;
            consoleOff = true;
        }
        c = ' ';
        std::system("CLS");
    }

}
void cutsc(int p, int a)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), p);
    std::system("CLS");
    loadCut("L1.txt");
    Sleep(100);
    std::system("CLS");
    loadCut("L2.txt");
    Sleep(100);
    std::system("CLS");
    loadCut("L3.txt");
    Sleep(100);
    std::system("CLS");
    loadCut("L4.txt");
    Sleep(100);
    std::system("CLS");
    loadCut("L5.txt");
    Sleep(100);
    std::system("CLS");
    loadCut("L6.txt");
    Sleep(100);
    std::system("CLS");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), a);
    loadCut("L6.txt");
    Sleep(100);
    std::system("CLS");
    loadCut("L5.txt");
    Sleep(100);
    std::system("CLS");
    loadCut("L4.txt");
    Sleep(100);
    std::system("CLS");
    loadCut("L3.txt");
    Sleep(100);
    std::system("CLS");
    loadCut("L2.txt");
    Sleep(100);
    std::system("CLS");
    loadCut("L1.txt");
    Sleep(100);
    std::system("CLS");


}
void info()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    gotoxy(53, 4);
    std::cout << "Lives: " << lives.skaicius;
    gotoxy(57, 5);
    std::cout << "  ";
    gotoxy(53, 5);
    std::cout << "x = " << player.x;
    gotoxy(57, 6);
    std::cout << "  ";
    gotoxy(53, 6);
    std::cout << "y = " << player.y;

    gotoxy(53, 8);
    std::cout << "        ";
    gotoxy(53, 8);
    std::cout << frameCount;
}

void restart()
{

    cutsc(15, 15);
    clearObjects();
    if (player.lvl == 1)
    {
        loadmap("level.txt");
    }
    else if (player.lvl == 2)
    {
        loadmap("level2.txt");
    }
    mapObjects();
    info();
    player.x = 3;
    player.y = 25;
    gotoxy(player.x, player.y);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    std::cout << '$';
}

void ZaidimoPradzia() {

    srand(time(NULL));
    player.x = 3;
    player.y = 25;
    frameCount = 0;
    loadmap("level.txt");
    mapObjects();
    info();
    gotoxy(player.x, player.y);
    player.lvl = 1;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    std::cout << '$';
}

void Nextlvl()
{
    cutsc(15, 15);
    clearObjects();
    loadmap("level2.txt");
    mapObjects();
    info();
    player.x = 3;
    player.y = 25;
    gotoxy(player.x, player.y);
    player.lvl = 2;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    std::cout << '$';

}

void lose()
{
    char c = '0';
    int choice = 2;
    std::system("CLS");
    clearObjects();
    loadCut("over.txt");
    player.lvl = 1;
    while ((consoleOff == false) && (inGame == false)) {
        while (c != char(13))
        {
            c = _getch();
            if (c == 'a') {
                choice = 1;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                gotoxy(16, 12);
                std::cout << "RESTART";

                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                gotoxy(30, 12);
                std::cout << "QUIT";
            }
            if (c == 'd') {
                choice = 2;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                gotoxy(16, 12);
                std::cout << "RESTART";

                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                gotoxy(30, 12);
                std::cout << "QUIT";
            }
        }
        if (choice == 1)
        {
            inGame = true;
            ZaidimoPradzia();
        }
        if (choice == 2)
        {
            inGame = false;
            consoleOff = true;
        }
        c = ' ';
        std::system("CLS");
    }


}

void Win()
{
    cutsc(15, 15);
    clearObjects();
    loadCut("win.txt");
    while (c != char(27))
    {
        c = _getch();
    }
    inGame = false;
    consoleOff = true;
    system("CLR");

}

void update()
{
    info(); // kad matyti frameCount
    // Judejimas
    if ((c == 'w') || (c == 'a') || (c == 's') || (c == 'd'))
    {
        info();
        if (c == 'w')
        {
            player.kryptis = 'w';
            
            if ((read(player.x, player.y - 1) != '#') && (read(player.x, player.y - 1) != '|'))
            {
                gotoxy(player.x, player.y);
                std::cout << " ";
                player.y -= 1;
            }
        }
        if (c == 'a')
        {
            player.kryptis = 'a';
            
            if ((read(player.x - 1, player.y) != '#') && (read(player.x - 1, player.y) != '|'))
            {
                gotoxy(player.x, player.y);
                std::cout << " ";
                player.x -= 1;
            }
        }
        if (c == 's')
        {
            player.kryptis = 's';
            
            if ((read(player.x, player.y + 1) != '#') && (read(player.x, player.y + 1) != '|'))
            {
                gotoxy(player.x, player.y);
                std::cout << " ";
                player.y += 1;
            }
        }
        if (c == 'd')
        {
            player.kryptis = 'd';
            
            if ((read(player.x + 1, player.y) != '#') && (read(player.x + 1, player.y) != '|'))
            {
                gotoxy(player.x, player.y);
                std::cout << " ";
                player.x += 1;
            }
        }

        if (read(player.x, player.y) == 'X')
            dmg = true;

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
        gotoxy(player.x, player.y);
        if (player.kryptis == 'w')
        {
            std::cout <<'$';
        }
        if (player.kryptis == 's')
        {
            std::cout << '$';
        }
        if (player.kryptis == 'a')
        {
            std::cout << '$';
        }
        if (player.kryptis == 'd')
        {
            std::cout << '$';
        }
    }

    // Zvaigzdes
    if (frameCount % 10 == 0) // Zvaigzudtes Pradzia 
    {
  
        for (int i = 0; i < Starsize; i++) 
        {
            if ( (read(StarKoord[i].second + 1, StarKoord[i].first) != '#') && starkryptis[i]=='d')
            {
                gotoxy(StarKoord[i].second, StarKoord[i].first);
                std::cout << ' ';
                StarKoord[i].second++;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                gotoxy(StarKoord[i].second, StarKoord[i].first);
                std::cout << '*';
            }
            else if ( (read(StarKoord[i].second - 1, StarKoord[i].first) != '#') && starkryptis[i]=='k')
            {
                gotoxy(StarKoord[i].second, StarKoord[i].first);
                std::cout << ' ';
                StarKoord[i].second--;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                gotoxy(StarKoord[i].second, StarKoord[i].first);
                std::cout << '*';
            }

            if (read(StarKoord[i].second, StarKoord[i].first) == read(player.x, player.y))
            {
                dmg = true;
            }

            if ( (read(StarKoord[i].second + 1, StarKoord[i].first) == '#') && starkryptis[i] == 'd')
            {
                starkryptis[i] = 'k';
            }

            if ( (read(StarKoord[i].second -1, StarKoord[i].first) == '#') && starkryptis[i] == 'k')
            {
                starkryptis[i] = 'd';
            }
        }
    } // Zvaigzudtes pabaiga

    // Spygliai
    if (frameCount % 80 == 0) // Spygliu pradzia
    {
        
            if (spike == true)
            {
                for (int i = 0; i < Spikesize; i++)
                {
                    gotoxy(SpikeKoord[i].second, SpikeKoord[i].first);
                    std::cout << ' ';
                    spike = false;
                }
            }
            else
            {
                for (int i = 0; i < Spikesize; i++)
                {
                    gotoxy(SpikeKoord[i].second, SpikeKoord[i].first);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                    std::cout << '^';
                    spike = true;

                }
            }

    }
    for (int i = 0; i < Spikesize; i++)
    {
        if ((spike == true) && (read(SpikeKoord[i].second, SpikeKoord[i].first) == (read(player.x, player.y))))
            dmg = true;
    }   //Spygliu pabaiga


    // Patrankos
    for (int i = 0; i < Cannonsize; i++) 
    {
        if (frameCount % 180 == 0)
        {
            bulletsk++;
            bullet[bulletsk].exists = true;
            bullet[bulletsk].x = CannonKoord[i].second;
            bullet[bulletsk].y = CannonKoord[i].first;
            bullet[bulletsk].visible = true;
        }

    }
    if (frameCount % 20 == 0)
    {
        for (int i = 0; i < 200; i++)
        {
            if (bullet[i].exists == true)
            {
                bullet[i].exists = false;
                if (bullet[i].visible == false)
                {
                    gotoxy(bullet[i].x, bullet[i].y);
                    std::cout << " ";
                }
                bullet[i].y += 1;
                if (read(bullet[i].x, bullet[i].y) == ' ')
                {
                    bullet[i].exists = true;
                }
                if (read(bullet[i].x, bullet[i].y) == (read(player.x, player.y)))
                {
                    dmg = true;
                }
                bullet[i].visible = false;
            }
        }
    }
        for (int i = 0; i <= 200; i++)
        {
            if (bullet[i].exists == true)
            {

                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                gotoxy(bullet[i].x, bullet[i].y);
                std::cout << "o";

                if (read(bullet[i].x, bullet[i].y) == (read(player.x, player.y)))
                {
                    dmg = true;
                }
            }
        }

    // Blokai

    if (blockkryptis == 's')
    {
        if (frameCount % 1 == 0)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
            for (int i = 0; i < Blocksize; i++)
            {
                if (read(BlockKoord[i].second, BlockKoord[i].first + 1) != '#')
                {
                    if ((read(BlockKoord[i].second, BlockKoord[i].first - 1) != 'X') && (read(BlockKoord[i].second, BlockKoord[i].first - 1) != '#'))
                    {
                        gotoxy(BlockKoord[i].second, BlockKoord[i].first);
                        std::cout << ' ';
                    }
                    
                    BlockKoord[i].first++;
                    gotoxy(BlockKoord[i].second, BlockKoord[i].first);
                    std::cout << 'X';

                }
                else
                {
                    blockkryptis = 'w';
                }
            }
        }
    }

    if (blockkryptis == 'w')
    {
        if (frameCount % 8 == 0)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
            for (int i = 0; i < Blocksize; i++)
            {
                if (read(BlockKoord[i].second, BlockKoord[i].first - 1) != '#')
                {
                    if ((read(BlockKoord[i].second, BlockKoord[i].first + 1) != 'X') && (read(BlockKoord[i].second, BlockKoord[i].first - 1) != '#'))
                    {
                        gotoxy(BlockKoord[i].second, BlockKoord[i].first);
                        std::cout << ' ';
                    }
                    
                    BlockKoord[i].first--;
                    gotoxy(BlockKoord[i].second, BlockKoord[i].first);
                    std::cout << 'X';

                }
                else
                {
                    blockkryptis = 's';
                }
            }
        }
    }

    for (int i = 0; i < Blocksize; i++)
    {
        if (read(BlockKoord[i].second, BlockKoord[i].first) == (read(player.x, player.y)))
        {
            dmg = true;
        }
    }

   
}



int main()
{
    
    framesDelay = 10;

    meniu();
    cutsc(15, 15);
    while (consoleOff == false)
    {
        ZaidimoPradzia();
        while (inGame == true)
        {
            update();
            charPlayer();

            if (dmg == true)
            {
                --lives;
                if (lives.skaicius == 0)
                {
                    inGame = false;
                    lives.skaicius = player.lives;

                }
                dmg = false;
                restart();

            }


            Sleep(framesDelay);
            frameCount += 1;
            
            if ((player.lvl == 1) && (player.x == 50) && (player.y == 25))
            {
                Nextlvl();
            }

            if ((player.lvl == 2) && (player.x == 0) && (player.y == 1))
            {
                Win();
                return 0;
            }

            if (bulletsk > 150)
                bulletsk = 0;
            if (frameCount > 1000000)
                frameCount = 1;
        }
        cutsc(15, 15);
        lose();
    }

	return 0;
}

