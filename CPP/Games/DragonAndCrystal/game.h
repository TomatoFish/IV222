//#ifndef GAME_H
//#define GAME_H
#pragma once
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <chrono>

using namespace std;

#define SIDE 4
#define CLRSCR "\E[H\E[2J"
#define FONT_COLOR_BLACK "\E[30m"
#define FONT_COLOR_RED "\E[31m"
#define FONT_COLOR_GREEN "\E[32m"
#define FONT_COLOR_BLUE "\E[34m"
#define FONT_COLOR_YELLOW "\E[33m"
#define FONT_COLOR_WHITE "\E[39m"
#define FONT_COLOR_SYS "\E[39m"
#define BG_COLOR_BLACK "\E[40m" 
#define BG_COLOR_RED "\E[41m"
#define BG_COLOR_GREEN "\E[42m"
#define BG_COLOR_BLUE "\E[44m"
#define BG_COLOR_YELLOW "\E[43m"
#define BG_COLOR_WHITE "\E[47m"
#define BG_COLOR_SYS "\E[49m"

enum itemsID {Key, Sword, Torchlight, Egg, RoastLeg, DragonEgg, Amulet, RedCrystal, BlueCrystal, GreenCrystal, BlackCrystal, Gold, None = -1};
enum side {N, E, S, W};
enum mobID {UglyTroll, BigSpider, Dragon};

class Item {
	public:
		Item (): id (None){
			int r = rand()%100;
			if (r < probabilitySword) {
				id = Sword;
				return;
			}
			r -= probabilitySword;
			if (r < probabilityTorchlight) {
				id = Torchlight;
				return;
			}
			r -= probabilityTorchlight;
			if (r < probabilityEgg) {
				id = Egg;
				return;
			}
			r -= probabilityEgg;
			if (r < probabilityRoastLeg) {
				id = RoastLeg;
				return;
			}
			r -= probabilityRoastLeg;
			if (r < probabilityDragonEgg) {
				id = DragonEgg;
				return;
			}
			r -= probabilityDragonEgg;
			if (r < probabilityOther) {
				switch (rand()%5) {
					case 0: {
						id = Amulet;
					} break;
					case 1: {
						id = RedCrystal;
					} break;
					case 2: {
						id = BlueCrystal;
					} break;
					case 3: {
						id = GreenCrystal;
					} break;
					case 4: {
						id = BlackCrystal;
					} break;
				}
				return;
			}
			return;		
		}
		int set (itemsID ID) {
			id = ID;
			gold = 0;
		}
		itemsID check(){
			return id;
		}
		itemsID addGold(){
			id = Gold;
			gold = rand()%200 - 100;
			if (gold <= 0) {
				gold = 0;
				id = None;
			}
			gold *= 10;
		}
		void addKey () {
			id = Key;
		}
		int checkGold () {
			if (id == Gold)
				return gold;
			return 0;
		}
		itemsID criateKey(){
			id = Key;
			return id;
		}
	private:
		itemsID id;
		int gold;
		static const int probabilitySword = 2;
		static const int probabilityTorchlight = 1;
		static const int probabilityEgg = 5;
		static const int probabilityRoastLeg = 3;
		static const int probabilityDragonEgg = 1;
		static const int probabilityOther = 15;
};

class Room {
	public:
		Room (){
			dark = false;
			monster = false;
			chest = false;
			exists = false;
			for (int i = 0; i < SIDE; ++i)
				doors[i] = false;
		}
		void addItem () {
			Item a;
			if (a.check() != None)
				items.push_back(a);
		}
		void addItemID (itemsID id) {
			Item a;
			a.set(id);
			items.push_back(a);
		}
		void addGold () {
			Item a;
			a.addGold();
			if (a.check() != None)
				items.push_back(a);
		}
		void addKey () {
			Item a;
			a.addKey();
			items.push_back(a);
		}
		void addChest () {
			chest = true;
		}
		void addDark () {
			dark = true;
		}
		void addMonster () {
			monster = true;
			int r = rand()%100;
			if (r < probabilityTroll) {
				name = UglyTroll;
				return;
			}
			r -= probabilityTroll;
			if (r < probabilitySpider) {
				name = BigSpider;
				return;
			}
			name = Dragon;
		}
		void buldDoor(side d) {
			doors[d] = true;
		}
		void buld () {
			exists = true;
		}
		void getItem(itemsID id) {
			bool f = false;
			for (int i = 0; i < items.size() && !f; ++i)
				if (items[i].check() == id) {
					f = true;
					items.erase(items.begin()+i);
				}					
		}
		void killMonster () {
			monster = false;
		}
		int check () {
			if (exists)
				return 1;
			else
				return 0;
		}
		bool checkItem (itemsID id) {
			bool f = false;
			for (int i = 0; i < items.size(); ++i)
				if (items[i].check() == id)
					f = true;
			return f;
		}
		bool checkChest () {
			return chest;
		}
		bool checkMonster () {
			return monster;
		}
		bool checkDark (bool torch) {
			if (torch)
				return false;
			for (int i = 0; i < items.size(); ++i)
				if (items[i].check() == Torchlight)
					return false;
			return dark;
		}
		bool checkKey () {
			for (int i = 0; i < items.size(); ++i)
			if (items[i].check() == Key)
				return true;
		}
		bool checkDoor (int d) {
			return doors[d];
		}
		int checkGold () {
			int gold = 0;
			for (int i = 0; i < items.size(); ++i)
				gold += items[i].checkGold();
			return gold;
		}
		void printName () {
			switch (name) {
				case UglyTroll: cout << FONT_COLOR_BLUE << "Ugly troll"; break;
				case BigSpider: cout << FONT_COLOR_GREEN << "Big spider"; break;
				case Dragon: cout << FONT_COLOR_RED << "Dragon"; break;
			}
			cout << FONT_COLOR_WHITE; 
		}
		void print () {
			bool f = false;
			for (int i = 0; i < items.size(); ++i)
				if (items[i].check() != None) {
					f = true;
					switch (items[i].check()) {
						case Key:{
							cout << BG_COLOR_BLACK << FONT_COLOR_YELLOW;
							cout << "Key"; 
						} break;
						case Sword: cout << FONT_COLOR_BLUE << "Sword" << FONT_COLOR_WHITE; break;
						case Torchlight: cout << FONT_COLOR_RED << "Torchlight" << FONT_COLOR_WHITE; break;
						case Egg: cout << FONT_COLOR_GREEN << "Egg" << FONT_COLOR_WHITE; break;
						case RoastLeg: cout << FONT_COLOR_GREEN << "Roast leg" << FONT_COLOR_WHITE; break;
						case DragonEgg: cout << FONT_COLOR_GREEN << "Dragon egg" << FONT_COLOR_WHITE; break;
						case Amulet: cout << "Amulet"; break;
						case RedCrystal: cout << FONT_COLOR_RED << "Red" << FONT_COLOR_WHITE << " crystal"; break;
						case BlueCrystal: cout << FONT_COLOR_BLUE << "Blue" << FONT_COLOR_WHITE << " crystal"; break;
						case GreenCrystal: {
							cout << FONT_COLOR_GREEN << "Green";
							cout << FONT_COLOR_WHITE << " crystal";
						} break;
						case BlackCrystal: cout << "Black crystal"; break;
						case Gold:{
							cout << FONT_COLOR_YELLOW;
							cout << "Gold (" << checkGold() << " coins)"<< FONT_COLOR_WHITE; 
						} break;
					}
					if (i != items.size()-1)
						cout << FONT_COLOR_WHITE << ", ";
				}
			if (chest) {
				if (f)
					cout << ", ";
				cout << BG_COLOR_BLUE << FONT_COLOR_YELLOW << "Chest";
			}				
		}
	private:
		static const int probabilityTroll = 50;
		static const int probabilitySpider = 35;
		bool dark, monster, chest, exists;
		mobID name;
		vector<Item> items;
		bool doors[4];
};

class Map {
	public:
		Map (int nRoom): size(nRoom){
			size = (size + 2) / 2;
			rooms = new Room*[size];
			for (int i = 0; i < size; ++i)
				rooms[i] = new Room[size];
			switch (rand()%4) {
				case 0: {
					placeX = rand() % size;
					placeY = 0;
					}break;
				case 1: {
					placeX = rand() % size;
					placeY = size - 1;
					}break;
				case 2: {
					placeX = 0;
					placeY = rand() % size;
					}break;
				case 3: {
					placeX = size - 1;
					placeY = rand() % size;
					}break;
			}
			backX = placeX;
			backY = placeY;
			int X = placeX, Y = placeY, itRoom = nRoom;
			rooms[X][Y].buld();
			--itRoom;
			while (itRoom) {
				switch (rand()%4) {
					case 0: {
						if (X+1 <= size-1)
							++X;
						} break;
					case 1: {
						if (X-1 >= 0)
							--X;
						} break;
					case 2: {
						if (Y+1 <= size-1)
							++Y;
						} break;
					case 3: {
						if (Y-1 >= 0)
							--Y;
						} break;
				}	
				if (!rooms[X][Y].check()) {
					rooms[X][Y].buld();
					--itRoom;
				}
			}
			for (int i = 0; i < size; ++i)
				for (int j = 0; j < size; ++j)
					if (rooms[i][j].check()) {
						if ((i+1 <= size-1) && (rooms[i+1][j].check()))
							rooms[i][j].buldDoor(E);
						if ((j+1 <= size-1) && (rooms[i][j+1].check()))
							rooms[i][j].buldDoor(S);
						if ((i-1 >= 0) && (rooms[i-1][j].check()))
							rooms[i][j].buldDoor(W);
						if ((j-1 >= 0) && (rooms[i][j-1].check()))
							rooms[i][j].buldDoor(N);
						for (int it = 0; it < 5; ++it)
							rooms[i][j].addItem();
						rooms[i][j].addGold();
					}
			X = placeX;
			Y = placeY;
			int life = size*2, keyMove = rand()%life, chestMove, door = rand()%4;
			chestMove = rand()%(life-keyMove);
			bool f = false;
			for (; keyMove && size != 1; --keyMove) {
				f = false;
				while (!f){
					door = rand()%4;
					if (rooms[X][Y].checkDoor(door))
						f = true;
				} 
				switch (door) {
					case 0: --Y; break;
					case 1: ++X; break;
					case 2: ++Y; break;
					case 3: --X; break;
				}
			}
			rooms[X][Y].addKey();
			for (; chestMove && size != 1; --chestMove) {
				f = false;
				while (!f){
					door = rand()%4;
					if (rooms[X][Y].checkDoor(door))
						f = true;
				} 
				switch (door) {
					case 0: --Y; break;
					case 1: ++X; break;
					case 2: ++Y; break;
					case 3: --X; break;
				}
			}
			rooms[X][Y].addChest();
			for (int i = 0; i < size; ++i)
				for (int j = 0; j < size; ++j)
					if (rooms[i][j].check() && !rooms[i][j].checkItem(Key) && !rooms[i][j].checkChest()) {
						if (rand()%100 < 30 && !(i == placeX && j == placeY))
							if (rand()%2)
								rooms[i][j].addDark();
							else
								rooms[i][j].addMonster();
					}			
		}
		void addItem (itemsID id) {
			rooms[placeX][placeY].addItemID(id);
		}
		void killMonster () {
			rooms[placeX][placeY].killMonster();
		}
		bool checkDoor(int d) {
			return rooms[placeX][placeY].checkDoor(d);
		}
		int getX () {
			return placeX;
		}
		int getY () {
			return placeY;
		}
		void getItem (itemsID id) {
			rooms[placeX][placeY].getItem(id);
		}
		void moveN() {
			--placeY;
		}
		void moveE() {
			++placeX;
		}
		void moveS() {
			++placeY;
		}
		void moveW() {
			--placeX;
		}
		void remember () {
			backX = placeX;
			backY = placeY;
		}
		void reverse () {
			placeX = backX;
			placeY = backY;
		}
		void nameMonster () {
				rooms[placeX][placeY].printName();
		}
		int getNDoors () {
			int k = 0;
			for (int i = 0; i < SIDE; ++i)
				if (rooms[placeX][placeY].checkDoor(i))
					++k;
			return k;
		}
		bool checkItem (itemsID id) {
			return rooms[placeX][placeY].checkItem(id);
		}
		bool checkMonster () {
			return rooms[placeX][placeY].checkMonster();
		}
		bool checkDark (bool torch) {
			return rooms[placeX][placeY].checkDark(torch);
		}
		int checkGold () {
			return rooms[placeX][placeY].checkGold();
		}
		void printDoors () {
			for (int i = 0; i < SIDE; ++i)
				if(rooms[placeX][placeY].checkDoor(i))
					switch (i) {
						case N: cout << FONT_COLOR_BLUE << "N" << FONT_COLOR_WHITE; break;
						case E: cout << FONT_COLOR_YELLOW << "E" << FONT_COLOR_WHITE; break;
						case S: cout << FONT_COLOR_RED << "S" << FONT_COLOR_WHITE; break;
						case W: cout << FONT_COLOR_GREEN << "W" << FONT_COLOR_WHITE; break;
					}
		}
		void printItems () {
			rooms[placeX][placeY].print();
		}
		~Map () {
			for (int i = 0; i < size; ++i)
				delete []rooms[i];
		}
	private:
		Room **rooms;
		int placeX, placeY, size, backX, backY;
};

class Player {
	public:
		Player (int l): life(l), gold(0){}
		void move() {
			--life;
		}
		void heal (itemsID id) {
			switch (id) {
				case (Egg): ++life; break;
				case (RoastLeg): life += 3; break;
				case (DragonEgg): life += 10; break;
			}
		}
		void addItem (itemsID id) {
			Item a;
			a.set(id);
			items.push_back(a);
		}
		int checkLife() {
			return life;
		}
		void getItem (itemsID id) {
			bool f = false;
			for (int i = 0; i < items.size() && !f; ++i)
				if (items[i].check() == id) {
					f = true;
					items.erase(items.begin()+i);
				}
		}
		void damage() {
			life -= life / 10;
		}
		void addGold (int g) {
			gold += g;
		}
		int checkGold () {
			return gold;
		}
		void printInv () {
			if (!gold)
				cout << "Gold (haven't)";
			else
				cout << "Gold (" << gold << " coins)";
			for (int i = 0; i < items.size(); ++i)
				if (items[i].check() != None) {	
					cout << ", ";
					switch (items[i].check()) {
						case Key: cout << "Key"; break;
						case Sword: cout << "Sword"; break;
						case Torchlight: cout << "Torchlight"; break;
						case Egg: cout <<"Egg"; break;
						case RoastLeg: cout <<"Roast leg"; break;
						case DragonEgg: cout <<"Dragon egg"; break;
						case Amulet: cout << "Amulet"; break;
						case RedCrystal: cout <<"Red crystal"; break;
						case BlueCrystal: cout <<"Blue crystal"; break;
						case GreenCrystal: cout << "Green crystal"; break;
						case BlackCrystal: cout << "Black crystal"; break;
					}
				}
		}
		bool checkItem (itemsID id) {
			bool f = false;
			for (int i = 0; i < items.size(); ++i)
				if (items[i].check() == id)
					f = true;
			return f;
		}
	private:
		int gold, life;
		vector<Item> items;
};

void menuOut(Map *m, Player *p) {
	if (m->checkDark(p->checkItem(Torchlight))) {
		cout << "Can’t see anything in this dark place!\n";
		return;
	} else if (m->checkMonster()) {
		cout << FONT_COLOR_WHITE << BG_COLOR_BLACK << "There is an evil ";
		m->nameMonster();
		cout << " in the room!\n";
	}
	cout << "You are in the room [" << BG_COLOR_YELLOW << FONT_COLOR_BLACK;
	cout << m->getX() << "," << m->getY() << BG_COLOR_BLACK << FONT_COLOR_WHITE;
	cout << "]. There are [" << m->getNDoors() << "] doors: [";
	m->printDoors();  
	cout << "].\nItems in the room: [";
	m->printItems();  
	cout << FONT_COLOR_WHITE << BG_COLOR_BLACK << "];\n>And so, what you do?\n";
}

bool menuIn(Map *m, Player *p, string input, bool inTime) {
	if (m->checkMonster()) {
		int r = rand() % 3;
		if (input.length() == 1 && inTime) {
			if (r == 0) {
				cout << "You threw!\n";
				p->damage();
				p->move();
				m->reverse();
				return false;
			} else if (r == 1) {
				p->damage();
				p->move();
			}
			switch (input[0]) {
				case 'N': {
					if (m->checkDoor(N)) {
						m->moveN();
						p->move();
					} else
						cout  << FONT_COLOR_RED << "This door don't exists!\n" << FONT_COLOR_WHITE;
					return false;
				} break;
				case 'E': {
					if (m->checkDoor(E)) {
						m->moveE();
						p->move();
					} else
						cout  << FONT_COLOR_RED << "This door don't exists!\n" << FONT_COLOR_WHITE;
					return false;
				} break;
				case 'S': {
					if (m->checkDoor(S)) {
						m->moveS();
						p->move();
					} else
						cout  << FONT_COLOR_RED << "This door don't exists!\n" << FONT_COLOR_WHITE;
					return false;
				} break;
				case 'W': {
					if (m->checkDoor(W)) {
						m->moveW();
						p->move();
					} else
						cout  << FONT_COLOR_RED << "This door don't exists!\n" << FONT_COLOR_WHITE;
					return false;
				} break;
				case 'q': return false; break; 
			}
		} else if (input == "fight" && inTime && p->checkItem(Sword)) {
			if (r == 0) {
				cout << "You threw!\n";
				p->damage();
				p->move();
				m->reverse();
				return false;
			} else if (r == 1) {
				p->damage();
				p->move();
			}
			m->killMonster();
			return false;
		}
		cout << "You threw!\n";
		p->damage();
		p->move();
		m->reverse();
		return false;
	}
	m->remember();
	if (input.length() == 1) 
		switch (input[0]) {
			case 'N': {
				if (m->checkDoor(N)) {
					m->moveN();
					p->move();
				} else
					cout  << FONT_COLOR_RED << "This door don't exists!\n" << FONT_COLOR_WHITE;
				return false;
			} break;
			case 'E': {
				if (m->checkDoor(E)) {
					m->moveE();
					p->move();
				} else
					cout  << FONT_COLOR_RED << "This door don't exists!\n" << FONT_COLOR_WHITE;
				return false;
			} break;
			case 'S': {
				if (m->checkDoor(S)) {
					m->moveS();
					p->move();
				} else
					cout  << FONT_COLOR_RED << "This door don't exists!\n" << FONT_COLOR_WHITE;
				return false;
			} break;
			case 'W': {
				if (m->checkDoor(W)) {
					m->moveW();
					p->move();
				} else
					cout  << FONT_COLOR_RED << "This door don't exists!\n" << FONT_COLOR_WHITE;
				return false;
			} break;
			case 'q': return false; break; 
		}
	
	string checkA = "get", checkB;
	checkB = "XXX";
	for (int i = 0; i < checkA.length() && i < input.length(); ++i)
		checkB[i] = input[i];
	if (checkA == checkB) {
		checkA = "Egg";
		checkB = "XXX";
		for (int i = 0; i < checkA.length() && i+4 < input.length(); ++i)
			checkB[i] = input[i+4];
		if (checkA == checkB) 
			if (m->checkItem(Egg)) {
				m->getItem(Egg);
				p->addItem(Egg);
				return false;
			}
		checkA = "Roast leg";
		checkB = "XXXXXXXXX";
		for (int i = 0; i < checkA.length() && i+4 < input.length(); ++i)
			checkB[i] = input[i+4];
		if (checkA == checkB) 
			if (m->checkItem(RoastLeg)) {
				m->getItem(RoastLeg);
				p->addItem(RoastLeg);
				return false;
			}
		checkA = "Dragon egg";
		checkB = "XXXXXXXXXX";
		for (int i = 0; i < checkA.length() && i+4 < input.length(); ++i)
			checkB[i] = input[i+4];
		if (checkA == checkB) 
			if (m->checkItem(DragonEgg)) {
				m->getItem(DragonEgg);
				p->addItem(DragonEgg);
				return false;
			}
		checkA = "Key";
		checkB = "XXX";
		for (int i = 0; i < checkA.length() && i+4 < input.length(); ++i)
			checkB[i] = input[i+4];
		if (checkA == checkB)
			if (m->checkItem(Key)) {
				m->getItem(Key);
				p->addItem(Key);
				return false;
			}
		checkA = "Sword";
		checkB = "XXXXX";
		for (int i = 0; i < checkA.length() && i+4 < input.length(); ++i)
			checkB[i] = input[i+4];
		if (checkA == checkB) 
			if (m->checkItem(Sword)) {
				m->getItem(Sword);
				p->addItem(Sword);
				return false;
			}
		checkA = "Torchlight";
		checkB = "XXXXXXXXXX";
		for (int i = 0; i < checkA.length() && i+4 < input.length(); ++i)
			checkB[i] = input[i+4];
		if (checkA == checkB) 
			if (m->checkItem(Torchlight)) {
				m->getItem(Torchlight);
				p->addItem(Torchlight);
				return false;
			}
		checkA = "Amulet";
		checkB = "XXXXXX";
		for (int i = 0; i < checkA.length() && i+4 < input.length(); ++i)
			checkB[i] = input[i+4];
		if (checkA == checkB) 
			if (m->checkItem(Amulet)) {
				m->getItem(Amulet);
				p->addItem(Amulet);
				return false;
			}
		checkA = "Red crystal";
		checkB = "XXXXXXXXXXX";
		for (int i = 0; i < checkA.length() && i+4 < input.length(); ++i)
			checkB[i] = input[i+4];
		if (checkA == checkB) 
			if (m->checkItem(RedCrystal)) {
				m->getItem(RedCrystal);
				p->addItem(RedCrystal);
				return false;
			}
		checkA = "Blue crystal";
		checkB = "XXXXXXXXXXXX";
		for (int i = 0; i < checkA.length() && i+4 < input.length(); ++i)
			checkB[i] = input[i+4];
		if (checkA == checkB) 
			if (m->checkItem(BlueCrystal)) {
				m->getItem(BlueCrystal);
				p->addItem(BlueCrystal);
				return false;
			}
		checkA = "Green crystal";
		checkB = "XXXXXXXXXXXXX";
		for (int i = 0; i < checkA.length() && i+4 < input.length(); ++i)
			checkB[i] = input[i+4];
		if (checkA == checkB) 
			if (m->checkItem(GreenCrystal)) {
				m->getItem(GreenCrystal);
				p->addItem(GreenCrystal);
				return false;
			}
		checkA = "Black crystal";
		checkB = "XXXXXXXXXXXXX";
		for (int i = 0; i < checkA.length() && i+4 < input.length(); ++i)
			checkB[i] = input[i+4];
		if (checkA == checkB) 
			if (m->checkItem(BlackCrystal)) {
				m->getItem(BlackCrystal);
				p->addItem(BlackCrystal);
				return false;
			}
		checkA = "Gold";
		checkB = "XXXX";
		for (int i = 0; i < checkA.length() && i+4 < input.length(); ++i)
			checkB[i] = input[i+4];
		if (checkA == checkB) 
			if (m->checkItem(Gold)) {
				p->addGold(m->checkGold());
				m->getItem(Gold);
				return false;
			}
		cout  << FONT_COLOR_RED << "No such item!\n" << FONT_COLOR_WHITE;
		return false;
	}
	checkA = "eat";
	checkB = "XXX";
	for (int i = 0; i < 3 && i < input.length(); ++i)
		checkB[i] = input[i];
	if (checkA == checkB) {
		checkA = "Egg";
		checkB = "XXX";
		for (int i = 0; i < checkA.length() && i+4 < input.length(); ++i)
			checkB[i] = input[i+4];
		if (checkA == checkB) {
			if (m->checkItem(Egg)) {
				m->getItem(Egg);
				p->heal(Egg);
				return false;
			} else if (p->checkItem(Egg)) {
				p->getItem(Egg);
				p->heal(Egg);
				return false;
			}
		}
		checkA = "Roast leg";
		checkB = "XXXXXXXXX";
		for (int i = 0; i < checkA.length() && i+4 < input.length(); ++i)
			checkB[i] = input[i+4];
		if (checkA == checkB) {
			if (m->checkItem(RoastLeg)) {
				m->getItem(RoastLeg);
				p->heal(RoastLeg);
				return false;
			} else if (p->checkItem(RoastLeg)) {
				p->getItem(RoastLeg);
				p->heal(RoastLeg);
				return false;
			}
		}
		checkA = "Dragon egg";
		checkB = "XXXXXXXXXX";
		for (int i = 0; i < checkA.length() && i+4 < input.length(); ++i)
			checkB[i] = input[i+4];
		if (checkA == checkB) {
			if (m->checkItem(DragonEgg)) {
				m->getItem(DragonEgg);
				p->heal(DragonEgg);
				return false;
			} else if (p->checkItem(DragonEgg)) {
				p->getItem(DragonEgg);
				p->heal(DragonEgg);
				return false;
			}
		}
		cout  << FONT_COLOR_RED << "No such item!\n" << FONT_COLOR_WHITE;
		return false;
	}
	checkA = "drop";
	checkB = "XXXX";
	for (int i = 0; i < checkA.length() && i < input.length(); ++i)
		checkB[i] = input[i];
	if (checkA == checkB) {
		checkA = "Egg";
		checkB = "XXX";
		for (int i = 0; i < checkA.length() && i+5 < input.length(); ++i)
			checkB[i] = input[i+5];
		if (checkA == checkB) 
			if (p->checkItem(Egg)) {
				p->getItem(Egg);
				m->addItem(Egg);
				return false;
			}
		checkA = "Roast leg";
		checkB = "XXXXXXXXX";
		for (int i = 0; i < checkA.length() && i+5 < input.length(); ++i)
			checkB[i] = input[i+5];
		if (checkA == checkB) 
			if (p->checkItem(RoastLeg)) {
				p->getItem(RoastLeg);
				m->addItem(RoastLeg);
				return false;
			}
		checkA = "Dragon egg";
		checkB = "XXXXXXXXXX";
		for (int i = 0; i < checkA.length() && i+5 < input.length(); ++i)
			checkB[i] = input[i+5];
		if (checkA == checkB) 
			if (p->checkItem(DragonEgg)) {
				p->getItem(DragonEgg);
				m->addItem(DragonEgg);
				return false;
			}
		checkA = "Key";
		checkB = "XXX";
		for (int i = 0; i < checkA.length() && i+5 < input.length(); ++i)
			checkB[i] = input[i+5];
		if (checkA == checkB)
			if (p->checkItem(Key)) {
				p->getItem(Key);
				m->addItem(Key);
				return false;
			}
		checkA = "Sword";
		checkB = "XXXXX";
		for (int i = 0; i < checkA.length() && i+5 < input.length(); ++i)
			checkB[i] = input[i+5];
		if (checkA == checkB) 
			if (p->checkItem(Sword)) {
				p->getItem(Sword);
				m->addItem(Sword);
				return false;
			}
		checkA = "Torchlight";
		checkB = "XXXXXXXXXX";
		for (int i = 0; i < checkA.length() && i+5 < input.length(); ++i)
			checkB[i] = input[i+5];
		if (checkA == checkB) 
			if (p->checkItem(Torchlight)) {
				p->getItem(Torchlight);
				m->addItem(Torchlight);
				return false;
			}
		checkA = "Amulet";
		checkB = "XXXXXX";
		for (int i = 0; i < checkA.length() && i+5 < input.length(); ++i)
			checkB[i] = input[i+5];
		if (checkA == checkB) 
			if (p->checkItem(Amulet)) {
				p->getItem(Amulet);
				m->addItem(Amulet);
				return false;
			}
		checkA = "Red crystal";
		checkB = "XXXXXXXXXXX";
		for (int i = 0; i < checkA.length() && i+5 < input.length(); ++i)
			checkB[i] = input[i+5];
		if (checkA == checkB) 
			if (p->checkItem(RedCrystal)) {
				p->getItem(RedCrystal);
				m->addItem(RedCrystal);
				return false;
			}
		checkA = "Blue crystal";
		checkB = "XXXXXXXXXXXX";
		for (int i = 0; i < checkA.length() && i+5 < input.length(); ++i)
			checkB[i] = input[i+5];
		if (checkA == checkB) 
			if (p->checkItem(BlueCrystal)) {
				p->getItem(BlueCrystal);
				m->addItem(BlueCrystal);
				return false;
			}
		checkA = "Green crystal";
		checkB = "XXXXXXXXXXXXX";
		for (int i = 0; i < checkA.length() && i+5 < input.length(); ++i)
			checkB[i] = input[i+5];
		if (checkA == checkB) 
			if (p->checkItem(GreenCrystal)) {
				p->getItem(GreenCrystal);
				m->addItem(GreenCrystal);
				return false;
			}
		checkA = "Black crystal";
		checkB = "XXXXXXXXXXXXX";
		for (int i = 0; i < checkA.length() && i+5 < input.length(); ++i)
			checkB[i] = input[i+5];
		if (checkA == checkB) 
			if (p->checkItem(BlackCrystal)) {
				p->getItem(BlackCrystal);
				m->addItem(BlackCrystal);
				return false;
			}
		checkA = "Gold";
		checkB = "XXXX";
		for (int i = 0; i < checkA.length() && i+5 < input.length(); ++i)
			checkB[i] = input[i+5];
		if (checkA == checkB) {
			cout << "I don't drop Gold. Never!\n";
			return false;
		}
		cout  << FONT_COLOR_RED << "No such item!\n" << FONT_COLOR_WHITE;
		return false;
	}
	checkA = "open";
	if (checkA == input) {
		if (p->checkItem(Key))
			return true;
		cout  << FONT_COLOR_RED << "You don't have [Key]!\n" << FONT_COLOR_WHITE;
		return false;
	}
	cout  << FONT_COLOR_RED << "Unknown command!\n" << FONT_COLOR_WHITE;
	return false;
}
