//This Code was developed by a group of student at Faculty of Engineering, Cairo University, department of electrical communications and electronics.

#include <iostream>
#include <bits/stdc++.h>
#include <cstdlib>
#include <time.h>
#include <windows.h>
#include <cwchar>
#include<stdlib.h>
#include <chrono> 
#include <limits>
#include <conio.h>


using namespace std;
using namespace std::chrono;

CONSOLE_FONT_INFOEX cfi;
//Global variables
bool win = true, cmplxtyCalc =false; // is the game over , refresh the screen , calc time cmplx.
int drawnCard;
int usize = 4, csize = 4, dusize = 0, dcsize = 0,colours[4];

#define MAX 4 // Governs the stack size and consecutively the number of pawns, THIS PARAMETER CAN BE ALTERED FROM 1~4 TO STUDY TIME COMPLEXITY


//pawn implementation
struct pawn
{
    char s; //symbol
    int x, y; //Coordinates on the board
    bool red, safe;
};

//Queue implementation
struct Queue {
    int front, rear, capacity;
    int* queue;
    Queue(int c)
    {
        front = rear = 0;
        capacity = c;
        queue = new int;
    }
    ~Queue() { delete[] queue; }
    void Enqueue(int data)
    {
        // insert element at the rear
        queue[rear] = data;
        rear++;
    }
    int Dequeue()
    {
        int drawn = queue[front];
        // shift all the elements from index 2 till rear
        // to the left by one
        for (int i = 0; i < rear - 1; i++) {
            queue[i] = queue[i + 1];
        }
        // decrement rear
        rear--;
        return drawn;
    }
    bool isEmpty()
    {
        return (front == rear);
    }
};

//Stack implementation (stack of pawns)
class Stack {
    int top;
public:
    pawn stack[MAX];
    Stack() { top = -1; }

    bool push(pawn x);
    bool isEmpty();
    pawn pop();
    int length()
    {
        return top;
    }
};
bool Stack::push(pawn x)
{
    if (top >= (MAX - 1)) {
        cout << "Stack Overflow";
        return false;
    }
    else {
        stack[++top] = x;
        return true;
    }
}
pawn Stack::pop()
{
    pawn x = stack[top--];
    return x;
}
bool Stack::isEmpty()
{
    return (top < 0);
}

Stack user, computer, DU, DC;
Queue DOC(45);   //Deck of cards (DOC)

void setFont(CONSOLE_FONT_INFOEX* cfi,int size_y){
cfi->nFont = 0;
cfi->dwFontSize.X = 0;                   // Width of each character in the font
cfi->dwFontSize.Y = size_y;              // Height
cfi->FontFamily = FF_DONTCARE;
cfi->FontWeight = FW_NORMAL;
std::wcscpy(cfi->FaceName, L"Consolas"); // Choose your font
SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, cfi);
}
void clearConsole() {
    COORD topLeft  = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
}

//Board implementation
class board
{
public:
    char b[17][17];
    board()
    {
        for (int i = 0; i < 16; i++) {
            b[i][0] = b[i][15] = b[0][i] = b[15][i] = '.';
        }
        for (int i = 1; i < 15; i++)
            for (int j = 1; j < 15; j++)
                b[i][j] = ' ';
        for (int i = 1; i < 7; i++)
        {
            b[i][2] = b[i + 8][13] = '.';
            b[2][15 - i] = b[13][i] = '.';
        }
        b[1][4] = 'U'; // U for user
        b[14][11] = 'C'; // C for computer
        b[0][1] = b[0][9] = '>';
        b[14][0] = b[6][0] = '^';
        b[15][14] = b[15][6] = '<';
        b[9][15] = b[1][15] = 'v';
        b[7][2] = b[8][13] = char(0 + 48);
        b[2][4] = b[13][11] = char(4 + 48);
    }
    void printBoard(char arr[17][17]);
};
void board::printBoard(char arr[17][17])
{
    b[7][2] = char(dusize + 48);
    b[8][13] = char(dcsize + 48);
    b[2][4] = char(usize + 48);
    b[13][11] = char(csize + 48);
    for (int i = 0; i < 16; i++)
    {
        cout << "\t\t\t";
        for (int j = 0; j < 16; j++)
        {
            cout << arr[i][j] << ' ';
            if ((i == 0 && ((j < 4) || (j > 7 && j < 13))) || (i < 8 && (j == 1 || j == 3)))
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colours[0]);
            else if ((j == 14 && ((i != 0 && i < 5) || (i > 8 && i < 14))) || ((i == 2 || i == 4) && (j > 7 && j < 15)))
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colours[1]);
            else if ((i == 15 && ((j > 9 && j < 14) || (j > 0 && j < 6))) || (i > 7 && (j == 12 || j == 10)))
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colours[2]);
            else if ((j == 15 && ((i > 9 && i < 14) || (i > 0 && i < 6))) || ((i == 11 || i == 13) && (j < 7)))
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colours[3]);
            else
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        }
        cout << endl;
    }
        cout << endl;

    //Print the board for >>Online Compiler
    //     for(int i=0; i<16; i++)
    //         {
    //         cout<<"                 ";
    //         for(int j=0; j<16; j++)
    //         cout<< arr[i][j]<<' ';
    //         cout<<endl;}

}

// Shuffle cards
void shuffleCards(int arr[], int n)
{
    // To obtain a time-based seed
    unsigned seed = 0;
    // Shuffling our array
    shuffle(arr, arr + n, default_random_engine(seed));
}

//Deck of cards implementation
class deck
{
public:
    int cards[45];
    int index = 0;
    deck()
    {
        while (index < 5)
        {
            cards[index] = 1;
            index++;
        }

        for (int i = 0; i < 4; i++)
        {
            for (int card = 2; card < 13; card++)
            {

                if (card == 9 || card == 6)
                {
                    continue;
                }
                cards[index] = card;
                index++;
            }
            cards[index] = 0;
            index++;
        }
        //Shuffle the cards in the beginning of the game.
        int n = sizeof(cards) / sizeof(cards[0]);
        srand(time(0));
        int ranTimes = rand() % 10;
        for (int shuffles = 0; shuffles < ranTimes; shuffles++)
        {
            shuffleCards(cards, n);
        }
    }
    void queueDeck();
};
void deck::queueDeck()
{
    for (int i = 0; i < 45; i++)
    {
        DOC.Enqueue(cards[i]);
    }
    cout << endl << "The deck of cards is shuffled, You can draw now!\n" << endl;
}
void logoPrint(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
    cout<<endl<<endl;
cout<<"       \t\t\t\t $$$$$$\\   $$$$$$\\  $$$$$$$\\  $$$$$$$\\ $$\\     $$\\        $$$$$$\\   $$$$$$\\  $$\\      $$\\ $$$$$$$$\\\n" 
<<"       \t\t\t\t$$  __$$\\ $$  __$$\\ $$  __$$\\ $$  __$$\\\\$$\\   $$  |      $$  __$$\\ $$  __$$\\ $$$\\    $$$ |$$  _____|\n"
<<"       \t\t\t\t$$ /  \\__|$$ /  $$ |$$ |  $$ |$$ |  $$ |\\$$\\ $$  /       $$ /  \\__|$$ /  $$ |$$$$\\  $$$$ |$$ |      \n"
<<"       \t\t\t\t\\$$$$$$\\  $$ |  $$ |$$$$$$$  |$$$$$$$  | \\$$$$  /        $$ |$$$$\\ $$$$$$$$ |$$\\$$\\$$ $$ |$$$$$\\    \n"
 <<"       \t\t\t\t \\____$$\\ $$ |  $$ |$$  __$$< $$  __$$<   \\$$  /         $$ |\\_$$ |$$  __$$ |$$ \\$$$  $$ |$$  __|   \n"
<<"       \t\t\t\t$$\\   $$ |$$ |  $$ |$$ |  $$ |$$ |  $$ |   $$ |          $$ |  $$ |$$ |  $$ |$$ |\\$  /$$ |$$ |      \n"
<<"       \t\t\t\t\\$$$$$$  | $$$$$$  |$$ |  $$ |$$ |  $$ |   $$ |          \\$$$$$$  |$$ |  $$ |$$ | \\_/ $$ |$$$$$$$$\\ \n"
 <<"       \t\t\t\t \\______/  \\______/ \\__|  \\__|\\__|  \\__|   \\__|           \\______/ \\__|  \\__|\\__|     \\__|\\________|\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

}

void MaximizeWindow()
{
	keybd_event(VK_MENU,0x38,0,0);
	keybd_event(VK_RETURN,0x1c,0,0);
	keybd_event(VK_RETURN,0x1c,KEYEVENTF_KEYUP,0);
	keybd_event(VK_MENU,0x38,KEYEVENTF_KEYUP,0);
}

//Welcome screen 
void welcomeScreen(bool &autoDraw)
{   
    MaximizeWindow();
    logoPrint();
    //Rules
    cout<<endl<<endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xB);
    cout << " Rules of the game:\n" ;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    cout<< "-------------------------------\n" << " - Each player has 4 pawns.\n";
    cout << " - If it's your first turn and you do not draw a card that lets you start a pawn out, you forfeit(skip) your turn.\n";
    cout << " - To move a pawn from your start out onto the track, you must draw either a 1 or a 2.\n" << " If it is a 2 put a pawn on the main track if it was on start and draw again. Otherwise, move pawn forward 2 steps and draw again. \n";
    cout << " - You may jump over your own or another player's pawn that's in your way, counting it as one space.\n" << " BUT... if you land on a space that's already occupied by an opponent's pawn, BUMP that pawn back to its own START space.\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xB);
    cout << endl << " Notes:\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    cout<< "-------------------------------\n";
    cout << " - Two pawns of the same color may never occupy the same space. If your only possible move makes you land on a space already occupied\n   by another of your own pawns, you forfeit your turn.\n";
    cout << " - If at any time you cannot move, you forfeit your turn.\n";
    cout << " - But if at any time you can move, you must move, even if it's to your disadvantage.\n";
    cout << " - If the pawn is placed on (>), it will move 3 or 4 steps foward (Clarified which is which on the board) on the main track bumping\n   any opponent's pawn/s back to its/their own START space.\n";
    cout << " - Movement of pawns is in clockwise movement.\n";
    cout << " - If the drawn card was Sorry card, then the player can choose any opponent's pawn on the main track to replace its own and bump the opponet's pawn back to start \n";
    cout << " Let's get started!\n";
    cout << "-----------------------------------------------------------------------------------------------\n";
    char ip;
    cout<<"Still confused? Watch a 2 min video tutorial to clarify game rules(Y/N)";
    cin>>ip;
    Beep(250,250);
    while(ip != 'Y' &&ip != 'y'&&ip != 'N'&&ip != 'n' ){
    cout<<"Error: Still confused? Watch a 2 min video tutorial to clarify the rules of the game(Y/N)";
    Beep(250,250);
    cin>>ip;
    }
    if(ip == 'Y' || ip == 'y'){
    std::string op = std::string("start ").append("https://www.youtube.com/watch?v=XMYIi17QrC0");
    system(op.c_str());
    }
    cout<<"Do you want the game to draw cards automatically?(Y/N)";
    cin>>ip;
    Beep(250,250);
    while(ip != 'Y' &&ip != 'y'&&ip != 'N'&&ip != 'n' ){
    cout<<"Error: Do you want the game to draw cards automatically?(Y/N)";
    Beep(250,250);
    cin>>ip;
    }
    if(ip == 'Y' || ip == 'y')
    autoDraw = true;
    cout<<"Choose the colour you want to play with(R/Y/G/B)";
    cin>>ip;
    Beep(250,250);
    while(ip != 'R' &&ip != 'r'&&ip != 'Y'&&ip != 'y' &&ip != 'G'&&ip != 'g'&&ip != 'B'&&ip != 'b'){
    Beep(250,250);
    cout<<"Error: Choose the colour you want to play with(R/Y/G/B)";
    cin>>ip;
    }
if(ip == 'R'|| ip =='r'){
        colours[0]=4;
        colours[1]=1;
        colours[2]=6;
        colours[3]=2;
        cout<<"You have chosen Red, computer has choosen Yellow\n"; }
else if(ip == 'Y'|| ip =='y'){
        colours[0]=6;
        colours[1]=1;
        colours[2]=4;
        colours[3]=2;
        cout<<"You have chosen Yellow, computer has choosen Red\n";}
else if(ip == 'G'|| ip =='g'){
        colours[0]=2;
        colours[1]=1;
        colours[2]=6;
        colours[3]=4;
        cout<<"You have chosen Green, computer has choosen Yellow\n"; }       
else
{
        colours[0]=1;
        colours[1]=4;
        colours[2]=6;
        colours[3]=2;
        cout<<"You have chosen Blue, computer has choosen Yellow\n"; ;
}
    cout<<"Calculate the Time Complexity?(Y/N)";
    cin>>ip;
    Beep(250,250);
    while(ip != 'Y' &&ip != 'y'&&ip != 'N'&&ip != 'n' ){
    Beep(250,250);
    cout<<"Error: Calculate the Time Complexity?(Y/N)";
    cin>>ip;
    }
    if(ip == 'Y' || ip == 'y')
    cmplxtyCalc = true;
    clearConsole();
}

//Game functions 
//--------------------------------------------------------------------------------------------------
//UOC stands for (User Or Computer)
//Function to Draw cards and check the deck queue(if empty shuffle the cards and queue it again)
void Draw(deck deckOfcards)
{
    drawnCard = DOC.Dequeue();
    if (DOC.isEmpty())
    {
        DOC.front = DOC.rear = 0;
        int n = sizeof(deckOfcards.cards) / sizeof(deckOfcards.cards[0]);
        shuffleCards(deckOfcards.cards, n);
        deckOfcards.queueDeck();
    }
}

//Function to place pawn on the main track
void placeonTrack(pawn* placer, bool selector, board* brd)
{
    if (selector)
    {
        placer->x = 0;
        placer->y = 4;
        brd->b[placer->x][placer->y] = placer->s;
        cout << endl;
    }
    else
    {
        placer->x = 15;
        placer->y = 11;
        brd->b[placer->x][placer->y] = placer->s;
        cout << endl;
    }
}

// delete a pawn from the active pawn after pushing in stack
void delPawnfrmArr(char ch, pawn sentPawns[8]) {
    int i;
    for (i = 0; i < 8; i++)
        if (sentPawns[i].s == ch)
            break;
    sentPawns[i] = { '\0',-1,-1,false,false };
    return;
}

//Function to check whether there is a pawn on a particuDlar slot or not
//Return true if there is a slot is empty
bool slotChecker(board brd, int x, int y)
{
    if (brd.b[x][y] == '.' || brd.b[x][y] == '>' || brd.b[x][y] == '<' || brd.b[x][y] == '^' || brd.b[x][y] == 'v')
        return true;
    else
        return false;
}

//Function to return the index of pawn that will move
int whichtoMove(char diff, pawn active[8])
{
    while (true)
    {
        for (int i = 0; i < 8; i++)
        {
            if (active[i].s == diff)
                return i;
        }
        cout << "Please Enter a correct letter!\n";
        cin >> diff;
    }
}

// Function to bump the opponent's pawn to its start
bool trivialBump(pawn* check, board* brd, pawn sentPawns[8])
{
    if (slotChecker(*brd, check->x, check->y)) //do nothing if theres no char
        return false;
    char ch = brd->b[check->x][check->y]; // the char of the pawn we will bump into
    int i;
    for (i = 0; i < 8; i++)
        if (sentPawns[i].s == ch)
            break;
    if ((ch == 'A' || ch == 'B' || ch == 'C' || ch == 'D') && !check->red)// user pawn will return to base
    {
        sentPawns[i]= {.s=sentPawns[i].s,.x=1, .y=4, .red=true, .safe =false };
        user.push(sentPawns[i]);
        usize++;
    }
    else if ((ch == 'Z' || ch == 'X' || ch == 'Y' || ch == 'W') && check->red)// computer pawn will return to base
    {
        sentPawns[i]= {.s=sentPawns[i].s,.x=14, .y=11, .red=false, .safe =false };
        computer.push(sentPawns[i]);
        csize++;
        Beep(200,250);
    }
    else
        return true;
    cout << "Pawn " << ch << " was sent home\n";
    //brd.b[sentPawns[i].x][sentPawns[i].y] = '.';
    delPawnfrmArr(ch, sentPawns);
    return false;
}
void slideBump(pawn* check, board *brd, pawn sentPawns[8], bool sliding) {
    if (slotChecker(*brd, check->x, check->y)) //do nothing if theres no char
        return ;
    //Char of the pawn that should be bumped
    char ch = brd->b[check->x][check->y];
    //Get the index of the symbol
    int i;
    for (i = 0; i < 8; i++)
        {if (sentPawns[i].s == ch)
            {break;}}
    if ((ch == 'A' || ch == 'B' || ch == 'C' || ch == 'D'))// user pawn will return to base
    {
        user.push(sentPawns[i]);
        usize++;
    }
    else if ((ch == 'Z' || ch == 'X' || ch == 'Y' || ch == 'W'))// computer pawn will return to base
    {
        computer.push(sentPawns[i]);
        csize++;
        Beep(200,250);
        }
    brd->b[sentPawns[i].x][sentPawns[i].y] = '.';
    cout << "Pawn " << ch << " was sent home\n";
    delPawnfrmArr(ch, sentPawns);
    return;
}

void slide(pawn check, board* brd, pawn sentPawns[8], bool sm) {
    for (int i = 1; i <= 4 - sm; i++)
    {
        pawn temp = check;
        if (check.x == 15) // slide left
            {temp.y -= i;}
        else if (check.x == 0)
            {temp.y += i;} //slide right
        else if (check.y == 15)
            {temp.x += i;} // slide down
        else
            {temp.x -= i;} // slide up

        slideBump(&temp, brd, sentPawns,true);
        
    }
}

void movePawn(pawn*, int, board*, pawn[]);
bool checker(char toCheck, pawn sentPawns[8]);

//To check whether the pawn is movable or not
bool isMovable(pawn mover, int steps, board* brd)
{
    if (mover.safe)
    {
            if ((mover.x + steps <= 6 && mover.red && slotChecker(*brd, mover.x + steps, mover.y)) || (mover.x - steps >= 9 && !mover.red) && slotChecker(*brd, mover.x - steps, mover.y)) 
                return true;
            else 
                return false;
    }
    if (mover.red && mover.x < 10 && mover.y < 3 && ((-mover.x + mover.y + steps - 2) > -1) && ((-mover.x + mover.y + steps - 2) < 7) ) {
        mover.x = -mover.x + mover.y + steps - 2;
        mover.y = 2;
        }
    else if (!mover.red && mover.x > 5 && mover.y > 12 && ((17 + mover.y  - steps -  mover.x) < 16) && ((17 + mover.y  - steps -  mover.x) > 8)) {
        mover.x = 17 + mover.y  - steps -  mover.x;
        mover.y = 13;
        }    
    else if (!mover.red && mover.x > 5 && mover.y > 12 && ((17 + mover.y  - steps -  mover.x) <= 8) ) { // if its getting out of the board then it cannot be moved
        return false;
        }
    else if (mover.red && mover.x < 10 && mover.y < 3  && ((-mover.x + mover.y + steps - 2) >= 7) ) {
        return false;
        }

    else if (mover.x == 0 && ((mover.y + steps) <= 15)) // upper horizontal
    {
        mover.y += steps;
    }
    else if (mover.x == 0 && ((mover.y + steps) > 15))
    {
        mover.x = (mover.y + steps - 15);
        mover.y = 15;
    }
    else if ((mover.x + steps) <= 15 && mover.y == 15) // right vertical
    {
        mover.x += steps;
    }
    else if ((mover.x + steps) > 15 && mover.y == 15)
    {
        mover.y = 15 - (mover.x + steps - 15);
        mover.x = 15;
    }
    else if (mover.x == 15 && ((mover.y - steps) >= 0)) // lower horizontal
    {
        mover.y -= steps;
    }
    else if (mover.x == 15 && ((mover.y - steps) < 0))
    {
        mover.x = 15 + (mover.y - steps);
        mover.y = 0;
    }
    else if ((mover.x - steps) >= 0 && mover.y == 0) // left vertical
    {
        mover.x -= steps;
    }
    else if ((mover.x - steps) < 0 && mover.y == 0)
    {
        mover.y = steps - mover.x;
        mover.x = 0;
    }
   char ch = brd->b[mover.x][mover.y];
   if(slotChecker(*brd, mover.x, mover.y) || (( ch == 'A' || ch == 'B' || ch == 'C' || ch == 'D') && !mover.red) || ((ch == 'Z' || ch == 'X' || ch == 'Y' || ch == 'W') && mover.red))
   {
       return true;
   }
   else
   {
       return false;
   }
}

//Return number of movable pawns in the active pawns
int possibleMove(bool UOC, int steps, board* brd, pawn sentPawns[8])
{
    int num = 0;
    if (UOC)
    {
        for (int i = 0; i < 4; i++)
        {   if(sentPawns[i].s !='\0')
            {
                if (isMovable(sentPawns[i], steps, brd))
                {
                    num++;
                }
            }
            {continue;}
        }
    }
    else 
    {
        for (int i = 4; i < 8; i++) //return false if any computer pawn is not in safe road
        {   if(sentPawns[i].s !='\0')
            {   
                if (isMovable(sentPawns[i], steps, brd))
                {
                    num++;
                }
            }
        }    
    }
    return num;
}

void moveAnotherPawn(pawn* mover, int steps, board* brd, pawn sentPawns[8]) {
    
    if (possibleMove(mover->red, steps, brd, sentPawns) ==0)
        return;

    if (mover->red) 
    {
        if (user.length() >= 2)
            return; // no pawns to move
        else if (user.length() == 1) // two pawns
        {
            //Find another pawn to move
            int i;
            for (i = 0; i < 4; i++)
                if (sentPawns[i].s != mover->s)
                    break;
            //infinte loop when ther is two cons. pawns on the safe road****
            if(!isMovable(sentPawns[i],steps,brd))
                return;
            cout << "Pawn " << sentPawns[i].s << " was moved by " << steps<<endl;
            movePawn(&sentPawns[i], steps, brd, sentPawns);
        }
        else
        {
            cout << "Which Pawn will you move? \n";
            char ch;
            bool break1 = false;
            while (true)
            {
                cin >> ch;
                for (int i = 0; i < 4; i++)
                {
                    if (sentPawns[i].s == ch && sentPawns[i].s != mover->s && isMovable(sentPawns[i],steps,brd))
                        {
                            break1 = true;
                            break;
                        }
                }
                if(true)
                break;
                
                cout << "Please choonse another pawn \n";
            }
            movePawn(&sentPawns[whichtoMove(ch, sentPawns)], steps, brd, sentPawns);
            cout << " Pawn " << ch << " was moved by " << steps<<endl;                                                                                
            return;
        }
    }
    else
    {
        if (computer.length() >= 2)
            return; // no pawns to move
        else if (computer.length() == 1) // two pawns
        {
            //Find another pawn to move
            int i;
            for (i = 4; i < 8; i++)
                if (sentPawns[i].s != mover->s)
                    break;
            if(!isMovable(sentPawns[i],steps,brd))
                return;
            movePawn(&sentPawns[i], steps, brd, sentPawns);
            cout << " Pawn " << sentPawns[i].s << " was moved by " << steps<<endl;
        }
        else
        {
            //More than two pawns
            //Find next pawn in the array
            int i;
            for (i = 4; i < 8; i++)
                {
                    if (sentPawns[i].s != mover->s && sentPawns[i].s != '\0' &&isMovable(sentPawns[i], steps,brd))
                        break;
                    if(i ==7)
                        return;
                }
            movePawn(&sentPawns[i], steps, brd, sentPawns);
            cout << "Pawn " << sentPawns[i].s << " was moved by " << steps<<endl;
        }
    }
}

//The sequence of steps that reverts the coordinates of a given pawn if the move is trivial 
bool trivialSeq(pawn* check, board *brd, pawn sentPawns[8], int x,int y ,int steps,bool isSliding) {
//since the sequence is checked in every stage, let the old indicies x,y fix  the '<' prob.
    if (x == 15 && y == 14  )
        brd->b[x][y] = '<';
    else if (x == 0 && y == 1  )
        brd->b[x][y] = '>';
    else if (x == 14 && y == 0  )
        brd->b[x][y] = '^';
    else if (x == 1 && y == 15  )
        brd->b[1][15] = 'v';
    else
        brd->b[x][y] = '.';
   if(isSliding)
        {return false;}
    if (trivialBump(check, brd, sentPawns))
    {
        check->y = y;
        check->x = x;
        brd->b[check->x][check->y] = check->s; // new indicies
        if (check->red)cout << "Trivial move\n";
        moveAnotherPawn(check, steps, brd, sentPawns);
        return true;
    }
    return false;
}

//Function to move pawn on the board 
void movePawn(pawn* mover, int steps, board* brd, pawn sentPawns[8])
{ 
    bool isSliding = false;
    int x_cor = mover->x, y_cor = mover->y;
    //move inside the safe zone
    if (mover->safe)
    {
        if (mover->red) {
            if (mover->x + steps == 6)
            {
                brd->b[mover->x][2] = '.';
                DU.push(*mover);
                dusize++;
                Beep(350,250);
                Beep(350,250);
                delPawnfrmArr(mover->s, sentPawns);
            }
            else if ((mover->x + steps) < 6)
            {
                mover->x += steps;
                if (trivialSeq(mover, brd, sentPawns, x_cor, y_cor, steps,false))
                    return;
                brd->b[mover->x][2] = mover->s;
            }
            else
                moveAnotherPawn(mover, steps, brd, sentPawns);
        }
        else {
            if (mover->x - steps == 9)
            {
                brd->b[mover->x][13] = '.';
                DC.push(*mover);
                dcsize++;
                Beep(200,250);
                Beep(200,250);
                delPawnfrmArr(mover->s, sentPawns);
            }
            else if ((mover->x - steps) > 9)
            {
                mover->x -= steps;
            if (trivialSeq(mover, brd, sentPawns, x_cor, y_cor, steps,false))
                    return;
                brd->b[mover->x][13] = mover->s;
            }
            else
                moveAnotherPawn(mover, steps, brd, sentPawns);
        }
        return;
    
    }
    //***********enter the safe zone************
    //for user:
    if (mover->red && mover->x < 10 && mover->y < 3 && ((-mover->x + mover->y + steps - 2) > -1) && ((-mover->x + mover->y + steps - 2) < 7) && !mover->safe) {
        mover->x = -mover->x + mover->y + steps - 2;
        mover->y = 2;
        if (trivialSeq(mover, brd, sentPawns, x_cor, y_cor, steps,false))
        return;
        brd->b[mover->x][mover->y] = mover->s;
        if(mover->x != 0){ 
        mover->safe = true;}
        if (mover->x == 6) {
            movePawn(mover, 0, brd, sentPawns);

        }
        return;
    }
    //for computer
    else if (!mover->red && mover->x > 5 && mover->y > 12 && ((17 + mover->y  - steps -  mover->x) < 16) && ((17 + mover->y  - steps -  mover->x) > 8) && !mover->safe) {
        mover->x = 17 + mover->y  - steps -  mover->x;
        mover->y = 13;
        if (trivialSeq(mover, brd, sentPawns, x_cor, y_cor, steps,false))
        return;
        brd->b[mover->x][mover->y] = mover->s;
        if(mover->x != 15){
        mover->safe = true;}
        if (mover->x == 9) {
            movePawn(mover, 0, brd, sentPawns);
        }
        return;
    }
    // pawns cannot make more than one turn around the board
    else if ((!mover->red && mover->x > 5 && mover->y > 12) || (mover->red && mover->x < 10 && mover->y < 3)) {
        if ((-mover->x + mover->y + steps -2 > 6) && mover->red)
        {
            moveAnotherPawn(mover, steps, brd, sentPawns);
            return;
        }
        else if ((17 + mover->y  - steps -  mover->x<9) && !mover->red) {
            moveAnotherPawn(mover, steps, brd, sentPawns);
            return;
        }
    }

    //move on board boundaries
    if (mover->x == 0 && ((mover->y + steps) <= 15)) // upper horizontal
    {
        mover->y += steps;
    }
    else if (mover->x == 0 && ((mover->y + steps) > 15))
    {
        mover->x = (mover->y + steps - 15);
        mover->y = 15;
    }
    else if ((mover->x + steps) <= 15 && mover->y == 15) // right vertical
    {
        mover->x += steps;
    }
    else if ((mover->x + steps) > 15 && mover->y == 15)
    {
        mover->y = 15 - (mover->x + steps - 15);
        mover->x = 15;
    }
    else if (mover->x == 15 && ((mover->y - steps) >= 0)) // lower horizontal
    {
        mover->y -= steps;
    }
    else if (mover->x == 15 && ((mover->y - steps) < 0))
    {
        mover->x = 15 + (mover->y - steps);
        mover->y = 0;
    }
    else if ((mover->x - steps) >= 0 && mover->y == 0) // left vertical
    {
        mover->x -= steps;
    }
    else if ((mover->x - steps) < 0 && mover->y == 0)
    {
        mover->y = steps - mover->x;
        mover->x = 0;
    }

    
    //slider
    //long slider (slide by 4)
    if ((mover->x == 0 && mover->y == 9) || (mover->x == 9 && mover->y == 15) || (mover->x == 15 && mover->y == 6) || (mover->x == 6 && mover->y == 0))
    {
        isSliding = true;
        slide(*mover, brd, sentPawns, 0);
        movePawn(mover, 4, brd, sentPawns);
        brd->b[0][9] = '>';
        brd->b[6][0] = '^';
        brd->b[15][6] = '<';
        brd->b[9][15] = 'v';
    }
    //short slider (slide by 3)
    if (((mover->x == 0 && mover->y == 1) && !mover->red) || (mover->x == 1 && mover->y == 15) || ((mover->x == 15 && mover->y == 14) && mover->red) || (mover->x == 14 && mover->y == 0))
    {
        isSliding = true;
        slide(*mover, brd, sentPawns, 1);
        movePawn(mover, 3, brd, sentPawns);
    }
    if(!trivialSeq(mover, brd, sentPawns, x_cor, y_cor, steps,isSliding))
    brd->b[mover->x][mover->y] = mover->s; // new indicies

}

//Function to check active pawns [among computer pawns]
bool checker(char toCheck, pawn sentPawns[8])
{
    for (int i = 4; i < 8; i++)
    {
        if (sentPawns[i].s == toCheck)
            return true;
    }
    return false;
}

//Funcion that returns computer decision to move which pawn
char computerdesicison(board brd, pawn sentPawns[8])
{
    bool returner;
    while (true)
    {
        srand(time(NULL));              //Randomize seed initialization
        int randNum = (rand() % 40) % 4; // Generate a random number between 0 and 3

        if (randNum == 0)
        {
            returner = checker('W', sentPawns);
            if (returner)
            return 'W';
        }
        else if (randNum == 1)
        {
            returner = checker('X', sentPawns);
            if (returner)
            return 'X';
        }
        else if (randNum == 2)
        {
            returner = checker('Y', sentPawns);
            if (returner)
                return 'Y';
        }
        else
        {
            returner = checker('Z', sentPawns);
            if (returner)
                return 'Z';
        }
    }
}

//Function to get the only active pawn's index
int getActivepawnindex(bool UOC, pawn sentPawns[8])
{
    int onlyActivepawn;
    if(UOC)
    {
        for (int i = 0; i < 4; i++)
            {
                if (sentPawns[i].s != '\0')
                    {
                        onlyActivepawn = i;
                        break;} 
            }
    }
    else
    {
        for (int i = 4; i < 7; i++)
            {
                if (sentPawns[i].s != '\0')
                    {
                        onlyActivepawn = i;
                        break;}
            }
    }
    return onlyActivepawn;
}

//Function to return the number of active pawns
int numberofactivePawns(pawn sentPawns[8], bool UOC)
{
    int NOAP = 0; //Number of active pawns
    if (UOC)
    {
        for (int i = 4; i < 8; i++)
        {
            if (sentPawns[i].s == 'W' || sentPawns[i].s == 'X' || sentPawns[i].s == 'Y' || sentPawns[i].s == 'Z')
            {
                NOAP++;
            }
        }
        return NOAP;
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            if (sentPawns[i].s == 'A' || sentPawns[i].s == 'B' || sentPawns[i].s == 'C' || sentPawns[i].s == 'D')
            {
                NOAP++;
            }
        }
        return NOAP;
    }
}

//Function to return the index of nearest replacable user pawn 
int moveACW(board brd, pawn sentPawns[8], bool UOC)
{
    if(UOC)
    {
        for (int i = 0; i < 4; i++)
            {
                for (int j = 15; j >=0; j--)
                {
                if(i == 0)      //Right vertical
                    {if (brd.b[j][15] == 'W' || brd.b[j][15] == 'X'|| brd.b[j][15] == 'Y'|| brd.b[j][15] == 'Z')
                        {return 1;}
                        continue;}
                else if(i == 1) //Upper horizontal
                        {if ((15 - j) != 3)
                            {
                                if (brd.b[0][j] == 'W' || brd.b[0][j] == 'X'|| brd.b[0][j] == 'Y'|| brd.b[0][j] == 'Z')
                                    {return 2;}
                                continue;
                            }
                            continue;}
                else if(i == 2) //Left vertical
                        {
                            if (brd.b[15- j][0] == 'W' || brd.b[15- j][0] == 'X'|| brd.b[15- j][0] == 'Y'|| brd.b[15- j][0] == 'Z')
                            {return 3;}
                            continue;}
                else            //Lower horizontal
                   {
                       if(brd.b[15][15- j] == 'W' || brd.b[15][15- j] == 'X'|| brd.b[15][15- j] == 'Y'|| brd.b[15][15- j] == 'Z')
                        {return 4;}
                        continue;}               
                }
            }
    }
    else
    { 
        for (int i = 0; i < 4; i++)
        {for (int j = 15; j >=0; j--)
            {
                if(i == 0)      //Right vertical
                    {
                        if (brd.b[j][15] == 'A' || brd.b[j][15] == 'B'|| brd.b[j][15] == 'C'|| brd.b[j][15] == 'D')
                            {return whichtoMove(brd.b[j][15],sentPawns);}
                            continue;}
                else if(i == 1) //Upper horizontal
                    {
                        if (brd.b[0][j] == 'A' || brd.b[0][j] == 'B'|| brd.b[0][j] == 'C'|| brd.b[0][j] == 'D')
                        {return whichtoMove(brd.b[0][j],sentPawns);}
                        continue;}
                else if(i == 2) //Left vertical
                    {
                        if (brd.b[15- j][0] == 'A' || brd.b[15- j][0] == 'B'|| brd.b[15- j][0] == 'C'|| brd.b[15- j][0] == 'D')
                        {return whichtoMove(brd.b[15- j][0],sentPawns);}
                        continue;} 
                else            //Lower horizontal
                    {
                        if (15- j != 12)
                        {
                            if(brd.b[15][15- j] == 'A' || brd.b[15][15- j] == 'B'|| brd.b[15][15- j] == 'C'|| brd.b[15][15- j] == 'D')
                                {return whichtoMove(brd.b[15][15- j],sentPawns);}
                                continue;
                        }
                        continue;}
            }               
        }
    }
    return -1;
}

//Move the chosen pawn
void movewhichPawn(board* brd, bool UOC, pawn sentPawns[8],int steps)
{
    if (UOC)
    {
        int i;        
        if(possibleMove(UOC,steps,brd,sentPawns)==1)
        {
            for(i=0;i<4;i++)
            {
                if(isMovable(sentPawns[i],steps,brd))
                {break;}
            }
        movePawn(&sentPawns[i], drawnCard, brd, sentPawns);
        cout<< "You have moved "<<sentPawns[i].s <<endl;
        }
        else if(possibleMove(UOC,steps,brd,sentPawns)==0)
        {
            return; //Skip the move
        }
        else
        {
        char selector;
        int index;
        cout << "Which Pawn will you move? \n";
        while(true)
        {   cin >> selector;
            if(selector=='A'||selector=='B' || selector=='C' ||selector=='D')
            {   index = whichtoMove(selector, sentPawns);
                if(isMovable(sentPawns[index],steps,brd))
                {break;}
                else
                {cout<< "Please enter a movable pawn: \n";}
            }
            else
            {cout<< "Please enter a correct pawn: \n";
                continue;}
        }
        movePawn(&sentPawns[index], drawnCard, brd, sentPawns);
        cout<< "You have moved "<<sentPawns[index].s <<endl;}
    } 
    else
    {
        int i;
        if(possibleMove(UOC,steps,brd,sentPawns)==1)
            {
                for(i=4;i<8;i++)
                {
                    if(isMovable(sentPawns[i],steps,brd))
                    {break;}
                }
                movePawn(&sentPawns[i], drawnCard, brd, sentPawns);
                cout<<"The computer has moved " <<sentPawns[i].s <<endl;
            }
        else if(possibleMove(UOC,steps,brd,sentPawns)==0)
            {
                return; //Skip the move
            }
        else
        {
            char selector;
            int index;
        while(true)
        {
            selector = computerdesicison(*brd, sentPawns);
            index = whichtoMove(selector, sentPawns);
            if(isMovable(sentPawns[index],steps,brd))
            {break;}
        }
        movePawn(&sentPawns[index], drawnCard, brd, sentPawns);
        cout << "The computer moved " << sentPawns[index].s << endl;}
    }

}

//Function to return the first free index of the activePawns array
int nearfreeindex(bool UOC, pawn sentPawns[8])
{   int index;
    if(UOC)
    {
        for(int i=0; i<4; i++)
        {
            if(sentPawns[i].s == '\0')
            {index=i;
            break;}
        }
    }
    else
    for(int i=4; i<8; i++)
        {
            if(sentPawns[i].s == '\0')
            {index=i;
            break;}
        }
    return index;
}

//Function that check if the number of pawns is either 3 or 2 and are in the safe road
bool checkPawnsonsafe(int NOAP,pawn sentPawns[8])
{
    int terminator= 0; //Terminates the sorry card if all three are in safe road
    for(int i=0; i<4;i++)
    {
        if(sentPawns[i].safe)
            terminator++;
    }
    if(terminator == NOAP)
        return true;
    else 
        return false;
}

//Function of sorry card
void sorryCard(bool UOC, pawn sentPawns[8],board* brd)
{   bool continuer = true;
    bool allSafe = true;
    if (UOC)//Cases of skipping
    {   //Cases of skipping 
        for(int i =0; i<=4; i++)
            {
                if(i==4)
                break;
                if(!sentPawns[i].safe)
                {
                    allSafe = false;
                    break;
                }
            }
    
        if (user.length() == -1)
            {
                continuer = false;
                cout<<"The card is useless\n";
                return;
            }
        //Skip if there is no active opponent pawn (They are either unpoped from computer stack or they are all 4 in safe road) 
        else if (computer.length() == 3 || allSafe || moveACW(*brd,sentPawns,true) == -1 )
            {
                continuer = false;
                cout<<"The card is useless\n";
                return;
            }
        //Execute the sorry card rule
        if (continuer)
            {   int replacedPawnindex;
                bool pawnIsinsaferoad;
                int NOAP = 0; //Number of active pawns
                int Pawnsonboarder=0;
                for(int i=4;i<8;i++)
                {
                    if((sentPawns[i].safe == false) && sentPawns[i].s != '\0')
                    {
                        Pawnsonboarder++;
                    }
                }
                NOAP = numberofactivePawns(sentPawns, true);
                if ((NOAP == 4 || NOAP == 3 || NOAP == 2) && Pawnsonboarder != 1 )
                    {
                        //Handling the corner case of if the number of pawns is either 3 or 2 and are in the safe road
                        if(NOAP == 3)
                        {
                            if(checkPawnsonsafe(NOAP,sentPawns))
                            {cout<<"The sorry card is useless Sorry!\n";
                                return;} 
                        }
                        else if(NOAP == 2)
                        {
                            if(checkPawnsonsafe(NOAP,sentPawns))
                            {cout<<"The sorry card is useless Sorry!\n";
                                return;} 
                        }
                        //Read the pawn that will be replaced from the user and check if he enters a proper one
                        char replaced;
                        cout << "Which pawn would you like to replace?\n";
                        //Read replaced pawn and check if valid
                        while (true)
                            {
                                cin >> replaced;
                                if (checker(replaced, sentPawns))
                                {
                                    replacedPawnindex = whichtoMove(replaced, sentPawns);
                                    pawnIsinsaferoad = sentPawns[replacedPawnindex].safe; 
                                    if(!pawnIsinsaferoad)
                                        break;
                                    else
                                    cout << "Please enter a correct pawn\n";
                                }
                                else
                                    cout << "Please enter a correct pawn\n";
                            }
                    }
                else
                    {   //Find the only active pawn
                        char OAP; //Only Active Pawn(OAP)
                        replacedPawnindex = getActivepawnindex(false, sentPawns);
                        OAP = sentPawns[replacedPawnindex].s;
                        pawnIsinsaferoad = sentPawns[replacedPawnindex].safe; 
                    }
                if(!pawnIsinsaferoad)
                    {   //Pop user stack
                        int firstFreeindex;
                        firstFreeindex = nearfreeindex(true, sentPawns);
                        sentPawns[firstFreeindex] = user.pop();
                        usize--;
                        Beep(200,250);
                        //Assign user x and y coordinates and place on the board 
                        sentPawns[firstFreeindex].x = sentPawns[replacedPawnindex].x;
                        sentPawns[firstFreeindex].y = sentPawns[replacedPawnindex].y;
                        brd->b[sentPawns[replacedPawnindex].x][sentPawns[replacedPawnindex].y] = sentPawns[firstFreeindex].s;
                        //push computer
                        sentPawns[replacedPawnindex]= {.s=sentPawns[replacedPawnindex].s,.x=14, .y=11, .red=false, .safe =false };
                        computer.push(sentPawns[replacedPawnindex]);
                        csize++;
                        Beep(200,250);
                        cout<<"You have replaced "<<sentPawns[replacedPawnindex].s<<" by "<< sentPawns[firstFreeindex].s<<" pawn\n";
                        cout<<"Pawn "<<sentPawns[replacedPawnindex].s << " was sent home.\n";
                        //delete replaced from the activePawns array.
                        sentPawns[replacedPawnindex] = {'\0',-1,-1,false,false};
                        movePawn(&sentPawns[firstFreeindex],0,brd,sentPawns);
                        brd->printBoard(brd->b);
                        return;
                    }
                    cout<<"Sorry card doesn't replace a pawn in the safe road\n";
                    return;
            }
        }
    else
    {
        //Computer sorry card
        //Cases of skipping 
        for(int i =4; i<=8; i++)
            {
                if(i==8)
                break;
                if(!sentPawns[i].safe)
                {
                    allSafe = false;
                    break;
                }
            }

        if (computer.length() == -1)
            {
                continuer = false;
                cout<<"The card is useless\n";
            }
            //Skip if there is no active opponent pawn (They are either unpoped from computer stack or they are all 4 in safe road)
        else if (user.length() == 3 || allSafe || moveACW(*brd,sentPawns,false)== -1)
            {
                continuer = false;
                cout<<"The card is useless\n";
            }
        //Execute the sorry card rule
        if (continuer)
        {
            int replacedPawnindex = moveACW(*brd,sentPawns,false);
            //cout<<"The computer replaced: \n"<<sentPawns[replacedPawnindex].s<<endl<<endl;
            //Pop user stack
            int firstFreeindex;
            firstFreeindex = nearfreeindex(false, sentPawns);
            sentPawns[firstFreeindex] = computer.pop();
            csize--;
            Beep(200,250);
            //Assign user x and y coordinates and place on the board 
            sentPawns[firstFreeindex].x = sentPawns[replacedPawnindex].x;
            sentPawns[firstFreeindex].y = sentPawns[replacedPawnindex].y;
            brd->b[sentPawns[replacedPawnindex].x][sentPawns[replacedPawnindex].y] = sentPawns[firstFreeindex].s;
            //push User 
            sentPawns[replacedPawnindex]= {.s=sentPawns[replacedPawnindex].s,.x=1, .y=4, .red=true, .safe =false };
            user.push(sentPawns[replacedPawnindex]);
            usize++;
            Beep(350,250);
            cout<<"The computer replaced "<<sentPawns[replacedPawnindex].s<<" by "<< sentPawns[firstFreeindex].s<<" pawn\n"<<endl;
            cout<<"Pawn "<<sentPawns[replacedPawnindex].s << " was sent home.\n";
            //delete replaced from the activePawns array.
            sentPawns[replacedPawnindex] = {'\0',-1,-1,false,false};
            movePawn(&sentPawns[firstFreeindex],0,brd,sentPawns);
            brd->printBoard(brd->b);
            return;

        }
        
    }
}

//Function to check whether the pawn is opponent's or not.
bool isOpponentchar(char toCheck, bool UOC)
{
    char LUC; //Last user computer (LUC)
    if(UOC)
    {LUC = 'Z';}
    else
    {LUC = 'D';}
    for(int i=0; i<4; i++)
    {
        if(toCheck == LUC)
            {return true;}
        LUC--;
    }
    return false;
}

void numberoneCard (bool UOC,pawn sentPawns[8],board* brd,int takenCard);
//Function to handle if the drawn card is 2
void numbertwoCard (bool UOC,pawn sentPawns[8],board* brd, deck deckOfcards)
{
    cout<<"Drawing again since the drawn card was 2. \n";
    Draw(deckOfcards);
    if(UOC)
    {
        cout << "You have drawn ";}
    else
    {
        cout << "The computer has drawn ";
    }
    
    if(!drawnCard)
    cout<<"Sorry card\n";
    else
    cout<<drawnCard<<endl;

    if((drawnCard != 0) && (drawnCard != 2) && (drawnCard !=1)) //Move the pawn function
        {
                if((usize == 3 && UOC)  || (csize == 3 && !UOC) )
                {
                    int onlyActivepawn;
                    onlyActivepawn=getActivepawnindex(UOC,sentPawns);
                    if(possibleMove(UOC,drawnCard,brd,sentPawns) == 0)
                    return;
                    movePawn(&sentPawns[onlyActivepawn], drawnCard, brd, sentPawns);
                    if(UOC)
                    {
                        cout<<"You have moved " << sentPawns[onlyActivepawn].s <<endl;}
                    else
                    {
                        cout<<"The computer has moved " << sentPawns[onlyActivepawn].s <<endl;
                    }
                }
                else
                {
                    movewhichPawn(brd, UOC, sentPawns,drawnCard);
                }
                brd->printBoard(brd->b);
                return;
        }
    else if(drawnCard == 0)
        {
            sorryCard(UOC,sentPawns,brd);
            return;
        }
    else if(drawnCard == 2)
        {   numberoneCard (UOC,sentPawns,brd,drawnCard);
            numbertwoCard(UOC,sentPawns,brd,deckOfcards);
            return;
        }
    else
    {   numberoneCard (UOC,sentPawns,brd,drawnCard);
            return;
    }
}

//Function to handle if the drawn card is 1
void numberoneCard (bool UOC,pawn sentPawns[8],board* brd,int takenCard)
{
    bool movePlace = false;
    //Either read from the user to decide to move a pawn or to add a pawn to the board or get the computer decicion if possible 
    if ( ((((brd->b[0][4] == '.') || isOpponentchar((brd->b[0][4]),UOC)) && (user.length() != -1)) && UOC) || ((((brd->b[15][11] == '.') || isOpponentchar((brd->b[15][11]),UOC)) && (computer.length() != -1)) && !UOC))
    {   if(UOC && (possibleMove(UOC,takenCard,brd,sentPawns) != 0))
            {
                cout << "Do you want to move a pawn (Enter 1)  or to place a pawn(Enter 0)?\n";
                int stopper;
                char dummy;
                cin >> dummy;
                 while (dummy != 48 && dummy != 49) 
                    {
                        cout << "Please enter either 1 (to move) or 0 (to place) : ";
                        cin >> dummy;
                    }
                stopper = (int)dummy - 48;
                if(stopper == 1)
                {movePlace = true;}
                else
                {movePlace = false;}
            }
        else
            {
                if((possibleMove(UOC,takenCard,brd,sentPawns) != 0))
                {
                    srand(time(NULL)); //Randomize seed initialization
                    movePlace = rand() % 2; // Generate a random number between 0 and 1
                }

            }
    
        //If the user or the computer decided to move a pawn
        if(movePlace)
            {   //Move the pawn function;
                //Check if there is more than a pawn on board and ask for which to move(for user) and (decide for computer)
                if ((user.length() < 2 && UOC) || (computer.length() < 2 && UOC)) //More than two pawns on board
                    {
                            movewhichPawn(brd, UOC, sentPawns, drawnCard);
                            brd->printBoard(brd->b);
                    }
                else
                    {   int onlyActivepawn;
                        onlyActivepawn=getActivepawnindex(UOC,sentPawns);
                        movePawn(&sentPawns[onlyActivepawn], drawnCard, brd, sentPawns);
                        if(UOC)
                            {
                                cout<<"You have moved " <<sentPawns[onlyActivepawn].s<<endl;
                            }
                        else
                            {
                                cout<<"The computer has moved " <<sentPawns[onlyActivepawn].s <<endl;
                            }
                        brd->printBoard(brd->b);
                    }
            }        
        else
            {
                //find the index of the first free place in the array of either user or computer to pop a new pawn in it. 
                int firstFreeindex;
                firstFreeindex = nearfreeindex(UOC, sentPawns);
                pawn dummy;
                if(UOC)
                    {   //pop in the first free array from 0 to 3
                        sentPawns[firstFreeindex] = user.pop();
                        usize--;
                        Beep(350,250);
                        dummy = { sentPawns[firstFreeindex].s,  0,  4, true, false };
                        cout<<"You have added " <<sentPawns[firstFreeindex].s<<" to the board\n"<<endl;
                    }
                else
                    {   //pop in the first free array from 4 to 7
                        sentPawns[firstFreeindex] = computer.pop();
                        csize--;
                        Beep(200,250);
                        dummy = { sentPawns[firstFreeindex].s,  15, 11, false, false };
                        cout<<"The computer has added " <<sentPawns[firstFreeindex].s<<" to the board\n"<<endl;
                    }
                trivialBump(&dummy,brd,sentPawns);
                placeonTrack(&sentPawns[firstFreeindex], UOC, brd);
                brd->printBoard(brd->b);
            }
    }
    else if (((user.length() == 2 && brd->b[0][4] != '.')&& UOC) ||((computer.length() == 2 && brd->b[15][11] != '.')&& !UOC))
                {   int onlyActivepawn;
                    onlyActivepawn=getActivepawnindex(UOC,sentPawns);
                    movePawn(&sentPawns[onlyActivepawn], drawnCard, brd, sentPawns);
                    if(UOC)
                        {cout<<"You have moved" <<sentPawns[onlyActivepawn].s<<endl;}
                    else
                        {cout<<"The computer has moved " <<sentPawns[onlyActivepawn].s <<endl;}
                        brd->printBoard(brd->b);
                }
    else
        {
            movewhichPawn(brd, UOC, sentPawns,drawnCard);
            brd->printBoard(brd->b);
        }
}

//Driver code
int main() 
{   bool autoDraw = false;
    int tCmplxty = 0;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0; 
    setFont(&cfi,19);
    welcomeScreen(autoDraw);
    
    //Initlaizing Board, the deck of cards and 4 pawns for each player.
    board brd;
    deck deckOfcards;
    deckOfcards.queueDeck();
    brd.printBoard(brd.b);
    pawn activePawns[8] = { NULL };
    char uc = 'D';
    char cc = 'Z';
    usize = 0, csize = 0;
    for (int i = 0; i < 4; i++)
    {
        pawn u = { uc--,  1,  4, true, false };
        pawn c = { cc--,  14, 11, false, false };
        user.push(u);
        computer.push(c);
        usize++;
        csize++;
    }
    
    //Start playing
    bool winner = true;
    while (win)
    {
//Player turn
        //Asks the user to press sth to draw
        if(!autoDraw){
        cout << "Press enter to draw\n"<<endl;
          getch();}

        Draw(deckOfcards);
        
        cout << "You have drawn ";
        if(!drawnCard)
        cout<<"Sorry card\n";
        else
        cout<<drawnCard<<endl;
        
        if (user.length() == 3 && (drawnCard == 1 || drawnCard == 2))
        {
            activePawns[0] = user.pop();
            usize--;
            Beep(350,250);

            pawn dummy = { activePawns[0].s,  0,  4, true, false };
            trivialBump(&dummy,&brd,activePawns);
            cout<<"You have added " <<activePawns[0].s<<" to the board\n"<<endl; 
            placeonTrack(&activePawns[0], true, &brd);
            brd.printBoard(brd.b);
            if (drawnCard == 2)
            {numbertwoCard(true,activePawns,&brd,deckOfcards);}
        }
        else
        {
            if (drawnCard == 1 || drawnCard == 2)
            {
                numberoneCard(true,activePawns,&brd, drawnCard);
                if(drawnCard == 2)
                    {
                        numbertwoCard(true,activePawns,&brd,deckOfcards);
                    }
            }
            else if ((drawnCard > 0))
            {
                if (possibleMove(true,drawnCard,&brd,activePawns) != 0)
                    {   
                        if (user.length() < 2) //More than two pawns
                        {
                            movewhichPawn(&brd, true, activePawns,drawnCard);
                            brd.printBoard(brd.b);
                        }
                        else
                        {
                            if (user.length() == 2)
                            {
                                int onlyActivepawn;
                                onlyActivepawn=getActivepawnindex(true,activePawns);
                                if(isMovable(activePawns[onlyActivepawn],drawnCard,&brd))
                                    {cout<<"You have moved "<<activePawns[onlyActivepawn].s<<endl;
                                    movePawn(&activePawns[onlyActivepawn], drawnCard, &brd, activePawns);}
                                else {cout<<activePawns[onlyActivepawn].s<<" cannot be moved\n"<<endl;}
                                brd.printBoard(brd.b);
                            }
                        }
                    }
                else
                cout<<"The card is useless\n";
            }
            else
            { 
                sorryCard(true, activePawns,&brd);
            }
        }
    
//To check if all pawns of the user are pushed to the destination stack
if (DU.length() == MAX-1)
{

    winner = true;
    break;
}

//Computer turn
    auto start = high_resolution_clock::now();  
        Draw(deckOfcards);
        cout << "The computer has drawn ";
        if(!drawnCard)
        cout<<"Sorry card\n";
        else
        cout<<drawnCard<<endl;
        if (computer.length() == 3 && (drawnCard == 1 || drawnCard == 2))
        {
            activePawns[4] = computer.pop();
            activePawns[4].x =15;
            activePawns[4].y =11;
            csize--;
            Beep(200,250);
            pawn dummy = { activePawns[4].s,  15,  14, false, false };
            trivialBump(&activePawns[4],&brd,activePawns);
            cout<<"The computer has added " <<activePawns[4].s<<" to the board\n"<<endl; 
            placeonTrack(&activePawns[4], false, &brd);
            //Bump user if exist**************************************************
            //Test if there is a user pawn on the start of the computer 
            brd.printBoard(brd.b);
            if (drawnCard == 2)
            {
                numbertwoCard(false,activePawns,&brd,deckOfcards);
            }
        }
        else
        {
            if (drawnCard == 1 || drawnCard == 2)
            {
               numberoneCard(false,activePawns,&brd,drawnCard); 
                if(drawnCard == 2)
                    {
                        numbertwoCard(false,activePawns,&brd,deckOfcards);
                    }
            }
            else if ((drawnCard > 0))
            {
                cout<<"**"<<possibleMove(false,drawnCard,&brd,activePawns)<<"***\n";
                if(possibleMove(false,drawnCard,&brd,activePawns) != 0)
                    {    
                        if (csize < 3)
                            {
                                movewhichPawn(&brd, false, activePawns,drawnCard);
                                brd.printBoard(brd.b);
                            }
                            else
                            {
                                if (computer.length() == 2)
                                {
                                    int onlyActivepawn;
                                    onlyActivepawn=getActivepawnindex(false,activePawns);
                                    cout<<"The computer has moved " <<activePawns[onlyActivepawn].s <<endl;
                                    movePawn(&activePawns[onlyActivepawn], drawnCard, &brd, activePawns);
                                    brd.printBoard(brd.b);
                                }
                            }
                    }
                else
                cout<<"The card is useless\n";
            }
            else
            {
                sorryCard(false, activePawns,&brd);
            }
        }
        auto stop = high_resolution_clock::now(); 
        auto duration = duration_cast<microseconds>(stop - start); 
        tCmplxty+=duration.count();
        //To check if all pawns of the computer are pushed to the destination stack
        if (DC.length() == MAX-1)
        {
            winner = false;
            break;
        }
    }
    //Annoucing the winner
    if (winner)
        {
            cout<<endl<<endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
            cout<< " $$$$$$\\   $$$$$$\\  $$\\   $$\\  $$$$$$\\  $$$$$$$\\   $$$$$$\\ $$$$$$$$\\  $$$$$$\\            $$\\     $$\\  $$$$$$\\  $$\\   $$\\       $$\\      $$\\  $$$$$$\\  $$\\   $$\\ $$\\\n" 
            <<"$$  _$$\\ $$  _$$\\ $$$\\  $$ |$$  _$$\\ $$  _$$\\ $$  _$$\\\\$$  _|$$  _$$\\           \\$$\\   $$  |$$  _$$\\ $$ |  $$ |      $$ | $\\  $$ |$$  __$$\\ $$$\\  $$ |$$ |\n"
            <<"$$ /  \\_|$$ /  $$ |$$$$\\ $$ |$$ /  \\|$$ |  $$ |$$ /  $$ |  $$ |   $$ /  \\_|           \\$$\\ $$  / $$ /  $$ |$$ |  $$ |      $$ |$$$\\ $$ |$$ /  $$ |$$$$\\ $$ |$$ |\n"
            <<"$$ |      $$ |  $$ |$$ $$\\$$ |$$ |$$$$\\ $$$$$$$  |$$$$$$$$ |  $$ |   \\$$$$$$\\              \\$$$$  /  $$ |  $$ |$$ |  $$ |      $$ $$ $$\\$$ |$$ |  $$ |$$ $$\\$$ |$$ |\n"
            <<"$$ |      $$ |  $$ |$$ \\$$$$ |$$ |\\_$$ |$$  _$$< $$  _$$ |  $$ |    \\__$$\\              \\$$  /   $$ |  $$ |$$ |  $$ |      $$$$  $$$$ |$$ |  $$ |$$ \\$$$$ |\\_|\n"
            <<"$$ |  $$\\ $$ |  $$ |$$ |\\$$$ |$$ |  $$ |$$ |  $$ |$$ |  $$ |  $$ |   $$\\   $$ |              $$ |    $$ |  $$ |$$ |  $$ |      $$$  / \\$$$ |$$ |  $$ |$$ |\\$$$ |    \n"
            <<"\\$$$$$$  | $$$$$$  |$$ | \\$$ |\\$$$$$$  |$$ |  $$ |$$ |  $$ |  $$ |   \\$$$$$$  |$$\\           $$ |     $$$$$$  |\\$$$$$$  |      $$  /   \\$$ | $$$$$$  |$$ | \\$$ |$$\\\n" 
            <<" \\___/  \\__/ \\|  \\| \\__/ \\|  \\|\\|  \\|  \\|    \\__/ $  |          \\|     \\__/  \\__/       \\/     \\| \\__/ \\|  \\|\\_|\n"
            <<"                                                                               \\_/                                                                                  \n\n";
        }
    else
        {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
        cout << "Hard luck, the computer won\n\n";
            }
            // code to calculate time taken to execute the algorithm:
        if(cmplxtyCalc)
        cout << "The time taken to execute the algorithm = "<<tCmplxty<<" us" << endl;
}
