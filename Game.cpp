#include <iostream>
#include <vector>
#include <string>
#include<ctime>
using namespace std;

// Base class for items placed on the board
class GameItem {
public:
    string name;
    int row;
    int col;
    GameItem(string n, int r, int c) : name(n), row(r), col(c) {}

    // Pure virtual functions
    virtual void interact() = 0; // Used when player interacts or touches object 
    virtual void display() const = 0; // Used to display objects on board
    // Const getters because they only need to get values not to manipulate them
    int getrow() const {
        return row;
    }
    int getcol() const {
        return col;
    }
    string getname() const {
        return name;
    }
    // Virtual destructor to ensure that derived class destructor executes properly
    virtual ~GameItem() {}
};
class Coin : public GameItem {
public:
    int value;
    Coin(string n, int r, int c) : GameItem(n, r, c) {
        // n is name/type of coin 
        if (n == "gold") { // assigning values to coin type
            value = 10;
        }
        else {
            value = 5;
        }
    }
    void interact() {
        cout << "Picked up a " << name << " coin worth " << value << " points!" << endl;
    }
    void display() const { // display coin on board
        if (value == 10) {
            cout << "G";
        }
        else {
            cout << "S";
        }
    }
    int getvalue() const {
        return value;
    }
};
class HelperObject : public GameItem {
public:
    int cost;
    int uses;
    HelperObject(string n, int r, int c, int cst, int u)
        : GameItem(n, r, c), cost(cst), uses(u) {
    }

    void interact() {
        cout << "Found a " << name << "!" << endl;
    }
    void display() const {
        if (name == "Sword") cout << "w";
        else if (name == "Shield") cout << "d";
        else if (name == "Water") cout << "a";
        else if (name == "Key") cout << "k";
        else cout << "H";
    }
    void use() {
        if (uses > 0)
            uses--;
    }
    bool isUsable() const {
        if (uses > 0)
            return true;
        else
            return false;
    }
};
class Hurdle : public GameItem {
    int blockTurns;
    int moveBack;
public:
    Hurdle(string type, int r, int c) : GameItem(type, r, c) {
        if (type == "Fire") {
            blockTurns = 2;
            moveBack = 0;
        }
        else if (type == "Snake") {
            blockTurns = 3;
            moveBack = 3;
        }
        else if (type == "Ghost") {
            blockTurns = 1;
            moveBack = 0;
        }
        else if (type == "Lion") {
            blockTurns = 4;
            moveBack = 0;
        }
        else if (type == "Lock") {
            blockTurns = 999; // until key is used
            moveBack = 0;
        }
    }
    void interact() {
        cout << "Encountered a " << name << " hurdle!" << endl;
    }
    void display() const {
        if (name == "Fire") cout << "f";
        else if (name == "Snake") cout << "s";
        else if (name == "Ghost") cout << "g";
        else if (name == "Lion") cout << "l";
        else if (name == "Lock") cout << "lk";
    }
    int getBlockTurns() const { return blockTurns; }
    int getMoveBack() const { return moveBack; }
};
class Player {
public:
    string name;
    int row;
    int col;
    int points;
    int gold;
    int silver;
    vector<HelperObject*> inventory;
    int blockedTurns;
    bool forward; // direction true if left->right false if right ->left
    bool movingDown; // true when moving to next row
    bool movingup;
    Player(string n, int startRow, int startCol, bool dir)
        : name(n), row(startRow), col(startCol), forward(dir),
        gold(10), silver(20), points(0),  blockedTurns(0) , movingDown(false) {
    }
    void move(int boardsize) {
        //check for blocked turns
        if (blockedTurns > 0) {
            blockedTurns--;
            cout << name << " is blocked for " << blockedTurns << " more turns." << endl;
            return;
        } // if turns are blocked the turn will shoft to next player

        if (name == "P1") {
            if (forward) {
               // if going from left->right
                col++;
                if (col >= boardsize) {// Reached corner move down and reverse direction
                    col = boardsize - 1;
                    row++;
                    forward = false; // changed direction........now next move will be from right->left
                    movingDown = true;
                }
            }
            else {//if going from right->left
                col--;
                if (col < 0) { // Reached corner move down and reverse direction
                    col = 0;
                    row++;
                    forward = true;//direction changed........now moving from left->right
                    movingDown = true;//shift to next row
                }
            }

            if (row >= boardsize) {//when rows are finished on board
                row = boardsize - 1;
                cout << name << " cannot move further. Reached end of board." << endl;
            }

            if (movingDown) {//if the row is shifted down
                cout << name << " is moving down to next row." << endl;
                movingDown = false;//row shift ends
            }
        }
        else if (name == "P2") {
            if (!forward) {//if going from right->left (Player2 starts from bottom-right)
               col--;
                if (col < 0) {    // Reached corner move up and reverse direction
                    col = 0;
                   row--;
                    forward = true;//changed direction........now next move will be from left->right
                    movingDown = true;
                }
            }
            else {//if going from left->right

                col++;
                if (col >= boardsize) {     // Reached corner move up and reverse direction
                    col = boardsize - 1;
                   row--;
                    forward = false;//direction changed........now moving from right->left
                    movingDown = true;//shift to next row (upward for Player2)
                }
            }
            if (row < 0) {//when rows are finished on board
                row = 0;
                cout << name << " cannot move further. Reached end of board." << endl;
            }

            if (movingDown) {//if the row is shifted down
                cout << name << " is moving down to next row." << endl;
                movingDown = false;
            }
        }
    }
    void moveBack(int boardsize) {//turn for player at the end .....Probably player 2 
        if (name == "P1") {
            if (forward) {//if moving from left->right
                col--;
                if (col < 0) {
                    col = 0;
                    row--;
                    forward = false;//shifted move from right->left
                }
            }
            else {
                col++;
                if (col >= boardsize) {
                    col = boardsize - 1;
                    row--;
                    forward = true;
                    movingup = true;
                }
            }
            if (movingup) {  //if the row is shifted down
                cout << name << " is moving down to next row." << endl;
                movingup = false;//tow shift ends
            }
            if (row < 0) {
                row = 0;
            }
		}
		else if (name == "P2") {
			if (!forward) {//if moving from right->left
				col++;
				if (col >= boardsize) {
					col = boardsize - 1;
					row++;
					forward = true;//shifted move from left->right
				}
			}
			else {
				col--;
				if (col < 0) {
					col = 0;
					row++;
					forward = false;
					movingup = true;
				}
			}
			if (movingup) {  //if the row is shifted down
				cout << name << " is moving down to next row." << endl;
				movingup = false;//tow shift ends
			}
			if (row >= boardsize) {
				row = boardsize - 1;
			}
		}
        else {

        }

    }
    
    void pickItem(GameItem* item) {
        if (Coin* coin = dynamic_cast<Coin*>(item)) { // if it is of coin type 
            points =points + coin->getvalue();
            if (coin->getname() == "gold") gold++;
            else silver++;
            cout << name << " picked up a " << coin->getname() << " coin (" << coin->getvalue() << " points)" << endl;
        }
        else if (HelperObject* helper = dynamic_cast<HelperObject*>(item)) {  // if it is of helper object 
            inventory.push_back(new HelperObject(*helper));
            cout << name << " picked up a " << helper->getname() << endl;
        }
    }
    void useObject(string objectName) {
        for (int i = 0; i < inventory.size(); i++) {
            if (inventory[i]->getname() == objectName && inventory[i]->isUsable()) {
                inventory[i]->use();
                cout << name << " used " << objectName << endl;
                if (!inventory[i]->isUsable()) {
                    delete inventory[i];
                    inventory.erase(inventory.begin() + i);
                }
                return;
            }
        }
        cout << "No usable " << objectName << " found in inventory." << endl;
    }
    bool hasObject(string objectName) const {
        for (const HelperObject* obj : inventory) {
            if (obj->getname() == objectName && obj->isUsable()) {
                return true;
            }
        }
        return false;
    }
    void buyObject(string objectName) {
        if (objectName == "Sword" && gold >= 4) {
            gold -= 4;
            points -= 40;
			//////// -1 -1 menas no index only object is buyed and stored in inventory
            inventory.push_back(new HelperObject("Sword", -1, -1, 40, 2));
            cout << name << " bought a Sword (2 uses)" << endl;
        }
        else if (objectName == "Shield" && silver >= 3) {
            silver -= 3;
            points -= 30;
            inventory.push_back(new HelperObject("Shield", -1, -1, 30, 1));
            cout << name << " bought a Shield (1 use)" << endl;
        }
        else if (objectName == "Water" && (gold + silver) >= 5) {
            if (gold >= 5) gold -= 5;
            else {
                silver -= (5 - gold);
                gold = 0;
            }
            points -= 50;
            inventory.push_back(new HelperObject("Water", -1, -1, 50, 1));
            cout << name << " bought Water (1 use)" << endl;
        }
        else if (objectName == "Key" && silver >= 7) {
            silver -= 7;
            points -= 70;
            inventory.push_back(new HelperObject("Key", -1, -1, 70, 1));
            cout << name << " bought a Key (1 use)" << endl;
        }
        else {
            cout << "Not enough coins to buy " << objectName << endl;
        }
    }

    void placeHurdle(string hurdleName, int row, int col) {
        if (hurdleName == "Fire" && (gold + silver) >= 5) {
            if (gold >= 5) gold = gold- 5;
            else {
                silver =silver- (5 - gold);
                gold = 0;
            }
            points = points - 50;
            cout << name << " placed Fire at (" << row << "," << col << ")" << endl;
        }
        else if (hurdleName == "Snake" && (gold+silver) >= 3) {
            if (gold >= 3) gold = gold - 3;
            else {
                silver = silver - (3 - gold);
                gold = 0;
            }
            points = points - 30;
            cout << name << " placed Snake at (" << row << "," << col << ")" << endl;
        }
        else if (hurdleName == "Ghost" &&( gold+silver >= 2)) {
            if (gold >= 2) gold = gold - 2;
            else {
                silver = silver - (2 - gold);
                gold = 0;
            }
            points = points - 20;
            cout << name << " placed Ghost at (" << row << "," << col << ")" << endl;
        }
        else if (hurdleName == "Lion" && gold >= 5) {
            gold =gold- 5;
            points =points- 50;
            cout << name << " placed Lion at (" << row << "," << col << ")" << endl;
        }
        else if (hurdleName == "Lock" && silver >= 6) {
            silver -= 6;
            points = points - 60;
            cout << name << " placed Lock at (" << row << "," << col << ")" << endl;
        }
        else {
            cout << "Not enough coins to place " << hurdleName << endl;
        }
    }
    void displayStatus() const {
        cout << name << "  Pos: (" << row << "," << col << ")  Points: "
            << points << "  Gold: " << gold << "  Silver: " << silver << endl;
        cout << "Inventory: ";
        for (const HelperObject* obj : inventory) {//iterates from all the helper objects stored in inventory
            cout << obj->getname() << "(" << obj->uses << "), ";//prints all helper objects
        }
        cout << endl;
    }
    ~Player() {
        for (HelperObject* obj : inventory) {
            delete obj;
        }
    }
};
class Board {
public:
    vector<vector<GameItem*>> grid;
    int size;

    Board(int s) : size(s) {
        // Resizing board
        grid.resize(size, vector<GameItem*>(size, nullptr));
    }

    void placeItem(GameItem* item) {
        //check if item is within size means the row and col generated to be placed on 
        if (item->getrow() >= 0 && item->getrow() < size && item->getcol() >= 0 && item->getcol() < size) {
            grid[item->getrow()][item->getcol()] = item;   //placing item on grid
        }
    }

    void placeRandomCoins(int minCoins, int maxCoins) {
        int total = minCoins + (rand() % (maxCoins - minCoins + 1));
        int placed = 0;
        while (placed < total) {
            int r = rand() % size;//on which number or row
            int c = rand() % size;//on which number of col

            // Skip center cell or occupied cells
            if ((r == size / 2 && c == size / 2) || grid[r][c]) {
                continue;
            }
            //deciding coitype to place
            string coinType;
            if ((rand() + r + c) % 2 == 0)//generating random number if even then gold will be placed if odd silver will be placed
            {
				coinType = "gold";
			}
            else {
                coinType = "silver";
            }
			//placing item on randomly generated row and col
            placeItem(new Coin(coinType, r, c));
            placed++;
        }
    }

    void placeRandomHurdles(int count) {
        int placed = 0;
        while (placed < count) {
            int r = rand() % size;
            int c = rand()  % size;

            // Skip center cell or occupied cells
            if ((r == size / 2 && c == size / 2) || grid[r][c]) {
                continue;
            }

            string hurdleTypes[] = { "Fire", "Snake", "Ghost", "Lion", "Lock" };
            string type = hurdleTypes[(rand() + r + c) % 5];//generating random index from the array

            placeItem(new Hurdle(type, r, c));//place hurdle 
            placed++;
        }
    }

    void removeItem(int r, int c) {
        delete grid[r][c]; // index will be deleted means null
        grid[r][c] = nullptr; // free dynamic memory
    }

    GameItem* getItem(int r, int c) {
        //within cell
        if (r >= 0 && r < size && c >= 0 && c < size) {
            return grid[r][c];
        }
        return nullptr;//null if no item exist
    }

    // Shows the game state, players, and objects on board
    void renderBoard(const Player* p1, const Player* p2) {
        int goalRow = size / 2;
        int goalCol = size / 2;

        // Print column numbers
		cout << "    ";
        for (int j = 0; j < size; j++) {
            cout << j << "      ";
        }
        cout << endl << endl;

        // Print each row with row number
        for (int i = 0; i < size; i++) {
            cout << i << "  ||  ";
            for (int j = 0; j < size; j++) {
                // Check player positions
                if (p1->row == i && p1->col == j) {
                    cout << "1 ||  ";
                }
                else if (p2->row == i && p2->col == j) {
                    cout << "2 ||  ";
                }
                // Goal cell
                else if (i == goalRow && j == goalCol) {
                    cout << "X ||  ";
                }
                // Game items
                else if (grid[i][j]) {//if something is there menas not empty index or existing index
                    grid[i][j]->display();
                    cout << " ||  ";
                }
                // Empty cell
                else {
                    cout << "  ||  ";
                }
            }
            cout << endl ;
        }
        cout << endl;
    }

    ~Board() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (grid[i][j]) {
                    delete grid[i][j];
                }
            }
        }
    }
};

class AdventureQuest {
    Board board;
    Player player1;
    Player player2;
    int turn;

public:
    AdventureQuest(int n) : board(n), player1("P1", 0, 0, true),
        player2("P2", n - 1, n - 1, false), turn(0) {

        // Place initial coins and hurdles
        board.placeRandomCoins(5, 10);//placing random coins on board with max and min limit
        board.placeRandomHurdles(3);//placing random hurdles on board with how much needed to be placed
    }

    void startGame() {
        bool gameOver = false;
        int goalRow = board.size / 2;
        int goalCol = board.size / 2;

        while (!gameOver) {
            system("cls");
            board.renderBoard(&player1, &player2);
            player1.displayStatus();
            player2.displayStatus();

            Player* currentPlayer = (turn % 2 == 0) ? &player1 : &player2;
            Player* otherPlayer = (turn % 2 == 0) ? &player2 : &player1;

            if (currentPlayer->blockedTurns > 0) {
                cout << currentPlayer->name << " is blocked for " << currentPlayer->blockedTurns << " more turns." << endl;
                currentPlayer->blockedTurns--;  // Decrement blockedTurns here
                turn++;  // Move to the next player's turn
                continue;
            }
           
            cout << currentPlayer->name << "'s turn. Choose action:" << endl;
            cout << "1. Move forward" << endl;
            cout << "2. Buy helper object" << endl;
            cout << "3. Place hurdle" << endl;
            cout << "Enter choice: ";

            int choice;
            cin >> choice;
           
            switch (choice) {

            case 1: { // Move forward
                // Check if player has already reached the goal before moving
                if (currentPlayer->row == goalRow && currentPlayer->col == goalCol) {
                    cout << currentPlayer->name << " has already reached the goal and cannot move further." << endl;
                    break; // Exit the case block to prevent further movement
                }

                currentPlayer->move(board.size);

                // Check if player reached the goal after movement
                if (currentPlayer->row == goalRow && currentPlayer->col == goalCol) {
                    cout << currentPlayer->name << " reached the goal!" << endl;

                    // Check if the other player is also at the goal
                    if (otherPlayer->row == goalRow && otherPlayer->col == goalCol) {
                        // Both players at the goal - compare points
                        if (currentPlayer->points > otherPlayer->points) {
                            cout << currentPlayer->name << " wins with more points!" << endl;
                        }
                        else if (otherPlayer->points > currentPlayer->points) {
                            cout << otherPlayer->name << " wins with more points!" << endl;
                        }
                        else {
                            cout << "It's a draw! Both players have the same points." << endl;
                        }
                        gameOver = true;
                    }
                    else {
                        // Only the current player reached the goal
                        if (currentPlayer->points > otherPlayer->points) {
                            cout << currentPlayer->name << " wins by reaching the goal with more points!" << endl;
                            gameOver = true;
                        }
                        else {
                            cout << currentPlayer->name << " reached the goal but needs more points to win!" << endl;
                            cout << "Waiting for " << otherPlayer->name << " to reach the goal..." << endl;
                        }
                    }
                }
                // If game isn't over, check for items in the new cell
                if (!gameOver) {
                    GameItem* item = board.getItem(currentPlayer->row, currentPlayer->col);
                    if (item) {
                        item->interact();

                        if (Coin* coin = dynamic_cast<Coin*>(item)) {
                            currentPlayer->pickItem(coin);
                            board.removeItem(currentPlayer->row, currentPlayer->col);
                        }
                        else if (Hurdle* hurdle = dynamic_cast<Hurdle*>(item)) {
                            bool usedHelper = false;
                            string requiredObject;

                            // Determine required helper object
                            if (hurdle->getname() == "Snake" || hurdle->getname() == "Lion") {
                                requiredObject = "Sword";
                            }
                            else if (hurdle->getname() == "Ghost") {
                                requiredObject = "Shield";
                            }
                            else if (hurdle->getname() == "Fire") {
                                requiredObject = "Water";
                            }
                            else if (hurdle->getname() == "Lock") {
                                requiredObject = "Key";
                            }

                            // Check if player has the required object
                            if (currentPlayer->hasObject(requiredObject)) {
                                cout << "Use " << requiredObject << " to overcome " << hurdle->getname() << "? (y/n): ";
                                char answer;
                                cin >> answer;
                                if (answer == 'y' || answer == 'Y') {
                                    currentPlayer->useObject(requiredObject);
                                    usedHelper = true;
                                    cout << hurdle->getname() << " overcome with " << requiredObject << "!" << endl;
                                }
                            }
                            else {
                                cout << "You don't have a " << requiredObject << "! Would you like to buy one? (y/n): ";
                                char answer;
                                cin >> answer;
                                if (answer == 'y' || answer == 'Y') {
                                    currentPlayer->buyObject(requiredObject);
                                    if (currentPlayer->hasObject(requiredObject)) {
                                        cout << "Use " << requiredObject << " to overcome " << hurdle->getname() << "? (y/n): ";
                                        cin >> answer;
                                        if (answer == 'y' || answer == 'Y') {
                                            currentPlayer->useObject(requiredObject);
                                            usedHelper = true;
                                            cout << hurdle->getname() << " overcome with " << requiredObject << "!" << endl;
                                        }
                                    }
                                }
                            }

                            // Apply hurdle effects if no helper was used
                            if (!usedHelper) {
                                currentPlayer->blockedTurns = hurdle->getBlockTurns();
                                if (hurdle->getMoveBack() > 0) {
                                    for (int i = 0; i < hurdle->getMoveBack(); i++) {
                                        currentPlayer->moveBack(board.size);
                                    }
                                    cout << currentPlayer->name << " was moved back by " << hurdle->getMoveBack() << " cells!" << endl;
                                }
                            }

                            board.removeItem(currentPlayer->row, currentPlayer->col);
                        }
                    }
                }
                break;
            }
            case 2: {
                cout << "Available helper objects:" << endl;
                cout << "1. Sword (4 gold, 40 points, 2 uses)" << endl;
                cout << "2. Shield (3 silver, 30 points, 1 use)" << endl;
                cout << "3. Water (5 coins, 50 points, 1 use)" << endl;
                cout << "4. Key (7 silver, 70 points, 1 use)" << endl;
                cout << "Enter choice: ";
                int objChoice;
                cin >> objChoice;

                switch (objChoice) {
                case 1: currentPlayer->buyObject("Sword"); break;
                case 2: currentPlayer->buyObject("Shield"); break;
                case 3: currentPlayer->buyObject("Water"); break;
                case 4: currentPlayer->buyObject("Key"); break;
                default: cout << "Invalid choice!" << endl;
                }
                break;
            }
            case 3: {
                cout << "Available hurdles:" << endl;
                cout << "1. Fire (5 coins, 50 points)" << endl;
                cout << "2. Snake (3 coins, 30 points)" << endl;
                cout << "3. Ghost (2 coins, 20 points)" << endl;
                cout << "4. Lion (5 gold, 50 points)" << endl;
                cout << "5. Lock (6 silver, 60 points)" << endl;
                cout << "Enter choice: ";
                int hurdleChoice;
                cin >> hurdleChoice;

                string hurdleName;
                switch (hurdleChoice) {
                case 1: hurdleName = "Fire"; break;
                case 2: hurdleName = "Snake"; break;
                case 3: hurdleName = "Ghost"; break;
                case 4: hurdleName = "Lion"; break;
                case 5: hurdleName = "Lock"; break;
                default:
                    cout << "Invalid choice!" << endl;
                    break;
                }

                if (!hurdleName.empty()) {
                    cout << "Enter row and column to place hurdle (0-" << board.size - 1 << "): ";
                    int r, c;
                    cin >> r >> c;

                    if (r >= 0 && r < board.size && c >= 0 && c < board.size &&
                        !(r == goalRow && c == goalCol) && !board.getItem(r, c)) {
                        currentPlayer->placeHurdle(hurdleName, r, c);
                        board.placeItem(new Hurdle(hurdleName, r, c));
                    }
                    else {
                        cout << "Invalid position!" << endl;
                    }
                }
                break;
            }
            default:
                cout << "Invalid choice!" << endl;
            }
       
            turn++;
 
        } 
    }
};
int main() {
    srand(time(0));
    cout << "^^^^^^^^^^^^^^^^^Welcome to Adventure Quest!^^^^^^^^^^^^^^^^^^^" << endl;
    cout << "_________________________________________________________________" << endl;
    cout << "|                        Game Rules                              |" << endl;
    cout << "|  1. Both players will move in zig zag direction                |" << endl;
    cout << "|  2. The Player that will reach the goal with more points       |" << endl;
    cout<<  "|     then other will win the game                               |" << endl;
    cout << "|  3. Player will get blocked if encounter a hurdle or move back |" << endl;
    cout << "|  4. Player can use helping object to cross the hurdle          |" << endl;
    cout << "|  5. Opponent player can place hurdle in front of other player  | " << endl;
    cout << "_________________________________________________________________" << endl;
    cout << endl;
    cout << "-------------GAME INFO---------------" << endl;
    cout << " 1 on board represent Player 1 " << endl;
    cout << " 2 on board represent Player 2 " << endl;
    cout << " G represent gold coins on board it increase 10 points " << endl;
    cout << " S represent silver coins on board it increase 5 points " << endl;
    cout << "Hurdles : " << endl;
    cout << " lk is Lock that block player for 999 turns means it will almost impossible to unblock without key 'Cost :6 silver ,60 points'" << endl;
    cout << " f is Fire that blocks player for 2 turns 'Cost:5 coins ,50 points'" << endl;
    cout << " s is Snake that block player for 3 turns and sent him 3 cells back 'Cost:3 coins, 30 points'" << endl;
    cout << " g is Ghost that blocks only1 turn 'Cost:2 coins,20 points '" << endl;
    cout << " l is Lion that blocks 4 tunrs 'Cost:5 gold, 50 points ' " << endl;
    cout << "Helping objects :" << endl;
    cout << "1. Sword (4 gold, 40 points, 2 use on 1 buy ) can be used to fight Snake or Lion" << endl;
    cout << "2. Shield(3 silver, 30 points, 1 use on 1 buy) can be used to fight Ghost" << endl;
    cout << "3. Water(5 coins, 50 points, 1 use on 1 buy)can be used to protect from Fire" << endl;
    cout << "4. Key(7 silver, 70 points, 1 use on 1 buy) can be used to open a Lock" << endl;
    cout << endl;
    cout << "Enter board size (odd number between 5 and 11): ";
    int size;
    cin >> size;

    if (size < 5 || size > 11 || size % 2 == 0) {
        cout << "Invalid size! Using default size 5." << endl;
        size = 5;
    }

    AdventureQuest game(size);
    game.startGame();
    return 0;
}