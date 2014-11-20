//#include <iostream>
//#include <vector>
#include "game.h"

using namespace std;

int main () {
	srand(time(0));	
	int nRoom = 0;
	string userInput = "";
	cout << BG_COLOR_BLACK << FONT_COLOR_WHITE << CLRSCR;
	cout << "Let's play," << endl << "how many rooms?" << endl;
	cin >> nRoom;
	if (nRoom <= 0) {
		cout << "Number of rooms can't be less than 1, or being symbol." << endl;
		return 1;
	}
	cout << "Bulding map\n";
	Map game(nRoom);
	cout << CLRSCR;
	Player user(nRoom*2);
	bool win = false, alive = true, inTime = false;
	while (userInput != "q" && !win && alive) {
		menuOut(&game, &user);
		auto start = chrono::high_resolution_clock::now();
		getline(cin, userInput);
		auto end = chrono::high_resolution_clock::now();
		cout << CLRSCR;
		if (((end - start).count() * ((double) chrono::high_resolution_clock::period::num /chrono::high_resolution_clock::period::den)) < 5)
			inTime = true;
		else
			inTime = false;
		if (userInput.length() > 0)
			win = menuIn(&game, &user, userInput, inTime);
		if (user.checkLife() <= 0)
			alive = false;
		//game.print(user.checkItem(Torchlight));
	}
	if (!alive) {
		cout << FONT_COLOR_SYS << BG_COLOR_SYS << CLRSCR << "You are dead!!\n";
		return 0;
	} else if (win) {
		cout << FONT_COLOR_SYS << BG_COLOR_SYS << CLRSCR << "You Win!!\n";
		cout << "And you have " << user.checkGold() << " coins!\n";
		return 0;
	}
	cout << FONT_COLOR_SYS << BG_COLOR_SYS << CLRSCR << "Good bye\n";
	
	return 0;	
}
