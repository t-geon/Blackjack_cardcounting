#include <iostream>
#include <cstdlib>
#include<ctime>
#include <vector>

using namespace std;

//player class
class person {
public:
	vector<pair<int, char>> mycard;	//player have card
	int sum[2];
	int win;
	int money;
	int blackjack;
	//int draw;
	//int blackwin;

	person() {
		sum[0] = 0; sum[1] = 0;
		win = 0;
		money = 100000;
		blackjack = 0;
		//	draw = 0;
		//	blackwin = 0;
	}

	void cardreset() { //Function to initialize the card to zero
		mycard.clear();
		sum[0] = 0; sum[1] = 0;
		blackjack = 0;
	}
};

void Shuffle(vector<pair<int, char>>& card);	//Card shuffling function
int HitStatus(person* p);	//Dealer, player2's hit, stand decision function
void SumofNum(person* p); //Dealer, player2 card total calculation and save
int probHitStatus(person* p, person* d, int* cc);	//Hit and stand decision function through the probability of player1
void probSumofNum(person* p);	//Player1's card total is calculated and saved

//s=¢¼, c=¢À, h=¢¾, d=¡ß
//1=A, 2~10 equal to value, 11=j, 12=q, 13=k
int main() {
	srand((unsigned int)time(NULL));

	vector<pair<int, char>> origincard(312);	//Initial card configuration
	vector<pair<int, char>> playcard;	//Game card composition

	int cardcount[13];	//A total of 13 cards
	fill_n(cardcount, 13, 0);//Count by card number

	int compare[2] = { 0, };	//If -1, do not compare with the dealer, if 1, compare with the dealer

	int game = 0;	//game count
	cout << "Play Game : ";
	cin >> game;

	//origincard initialization
	vector<pair<int, char>>::iterator iter = origincard.begin();//card iterator
	for (int i = 1; i < 14; i++) {
		for (int j = 0; j < 4; j++) {	//card shape setting
			if (j == 0) { //¢¼
				pair<int, char> setter = make_pair(i, 's');	//¢¼ setting
				fill(iter, iter + 6, setter);
				iter += 6;
			}
			if (j == 1) {//¢À
				pair<int, char> setter = make_pair(i, 'c');	//¢À setting
				fill(iter, iter + 6, setter);
				iter += 6;
			}
			if (j == 2) {//¢¾
				pair<int, char> setter = make_pair(i, 'h');	//¢¾ setting
				fill(iter, iter + 6, setter);
				iter += 6;
			}
			if (j == 3) {//¡ß	
				pair<int, char> setter = make_pair(i, 'd');	//¡ß setting
				fill(iter, iter + 6, setter);
				if (i < 13) { iter += 6; }
			}
		}
	}

	//play card setting
	playcard = origincard;	//The game card is taken from the original card.
	Shuffle(playcard);	//card suffle

	person* d = new person();	//dealer
	person* p1 = new person();	//player1
	person* p2 = new person();	//player2

	//game loop
	for (int i = 0; i < game; i++) {

		//Betting
		p1->money -= 100;
		p2->money -= 100;

		// When more than 80% of all cards are used
		if (playcard.size() < 312 * 0.2) {
			playcard = origincard;		//Receive original card
			Shuffle(playcard);			//Shuffle cards
			fill_n(cardcount, 13, 0);	//card count initialization
		}

		//card setting
		d->mycard.push_back(playcard.at(0));	//dealer get first card
		playcard.erase(playcard.begin());	//card erase, first card can not count
		p1->mycard.push_back(playcard[0]);		//player1 get first card
		cardcount[playcard.at(0).first - 1] += 1; playcard.erase(playcard.begin());	//card count, card erase
		p2->mycard.push_back(playcard[0]);		//player2 get first card
		cardcount[playcard.at(0).first - 1] += 1; playcard.erase(playcard.begin());	//card count, card erase
		d->mycard.push_back(playcard.at(0));	//dealer get first card
		cardcount[playcard.at(0).first - 1] += 1; playcard.erase(playcard.begin());	//card count, card erase
		p1->mycard.push_back(playcard[0]);		//player1 get first card
		cardcount[playcard.at(0).first - 1] += 1; playcard.erase(playcard.begin());	//card count, card erase
		p2->mycard.push_back(playcard[0]);		//player2 get first card
		cardcount[playcard.at(0).first - 1] += 1; playcard.erase(playcard.begin());	//card count, card erase

		//player1 card check
		while (1) {
			probSumofNum(p1);	//player1 card total calculation
			int result = probHitStatus(p1, d, cardcount);		//Hit and stand decision

			if (result == 1) {	//hit
				p1->mycard.push_back(playcard[0]);	 //1 player1 card added
				cardcount[playcard.at(0).first - 1] += 1; playcard.erase(playcard.begin());
			}
			else if (result == 0) {	//bust
				compare[0] = -1;
				p1->cardreset();
				break;
			}
			else {//stand
				compare[0] = 1;
				break;
			}
			cardcount[d->mycard.at(0).first - 1] += 1;	//After calculating the probability, the dealer's first card is increased by 1 in cc
		}

		//player2 card check
		while (1) {
			SumofNum(p2);	//player2 card total calculation
			int result = HitStatus(p2);		//Hit and stand decision

			if (result == 1) {	//hit
				p2->mycard.push_back(playcard[0]);	 //1 player2 card added
				cardcount[playcard.at(0).first - 1] += 1; playcard.erase(playcard.begin());
			}
			else if (result == 0) {	//bust
				compare[1] = -1;
				p2->cardreset();
				break;
			}
			else {//stand
				compare[1] = 1;
				break;
			}
		}

		if (compare[0] == 1 || compare[1] == 1) {//If even one player is not bust
			//dealer card sum check
			while (1) {
				SumofNum(d);	//Dealer card total calculation
				int result = HitStatus(d);
				if (result == 1) {	//hit
					d->mycard.push_back(playcard[0]);	 //Add 1 dealer card
					cardcount[playcard.at(0).first - 1] += 1; playcard.erase(playcard.begin());
				}
				else if (result == 0) {	//bust
					if (compare[0] == 1) { p1->win += 1; p1->money += 200; }//Dealer bust, not bust, player1 win
					if (compare[1] == 1) { p2->win += 1; p2->money += 200; }//Dealer bust, not bust, player2 win
					d->cardreset();
					break;
				}
				else {//stand

					//player1 VS dealer
					if (compare[0] == 1) {

						//sum select
						int sel = 0;
						if (p1->sum[1] > 21) { sel = p1->sum[0]; }
						else { sel = p1->sum[1]; }

						if (sel > d->sum[0]) { //Player 1 has a higher value than the dealer (closer to 21)
							if (p1->blackjack == 1) {//In the case of blackjack
								//p1->blackwin+=1;
								p1->win += 1;//Player 1 wins
								p1->money += 250;
							}
							else {
								p1->win += 1; //Player 1 wins
								p1->money += 200;
							}
						}
						else if (sel == d->sum[0]) {	//The sum of player 1 cards and the sum of dealer cards are the same
							if (p1->blackjack == 1 && d->blackjack != 1) {//If only player 1 is blackjack
								//p1->blackwin += 1;
								p1->win += 1;//Player 1 win
								p1->money += 250;
							}
							else if (p1->blackjack != 1 && d->blackjack == 1) {}//If only the dealer is blackjack
							else {//If there is no blackjack and the value is the same, if both are blackjack
								p1->money += 100;
								//p1->draw++;
							}
						}
					}

					//player2 VS dealer
					if (compare[1] == 1) {
						if (p2->sum[0] > d->sum[0]) { //Player 2 has a higher value than the dealer (closer to 21)
							if (p2->blackjack == 1) {//In the case of blackjack
								p2->win += 1;//In the case of blackjack
								//p2->blackwin += 1;
								p2->money += 250;
							}
							else {
								p2->win += 1; //Player 2 wins
								p2->money += 200;
							}
						}
						else if (p2->sum[0] == d->sum[0]) {	//The sum of player 2 cards and the sum of dealer cards are the same
							if (p2->blackjack == 1 && d->blackjack != 1) {//If only player 2 is blackjack
								p2->win += 1;//ÇÃ·¹ÀÌ¾î2½Â¸®
								p2->money += 250;
								//p2->blackwin += 1;
							}
							else if (p2->blackjack != 1 && d->blackjack == 1) {}//If only the dealer is blackjack
							else {//If there is no blackjack and the value is the same
								p2->money += 100;
								//p2->draw++;
							}
						}
					}
					break;
				}
			}
		}
		d->cardreset();
		p1->cardreset();
		p2->cardreset();
	}
	cout << "<<Player1>>" << endl << "½Â¸® : " << p1->win << "  ¹«½ÂºÎ ¹× ÆÐ¹è : " << game - p1->win << endl << "money = " << p1->money << endl;
	//cout << "ÀÏ¹Ý½Â = " << p1->win - p1->blackwin << "  ºí·¢Àè½Â = " << p1->blackwin << "  ¹«½ÂºÎ : " << p1->draw << endl<<endl;
	cout << "<<Player2>>" << endl << "½Â¸® : " << p2->win << "  ¹«½ÂºÎ ¹× ÆÐ¹è : " << game - p2->win << endl << "money = " << p2->money << endl;
	//cout << "ÀÏ¹Ý½Â = " << p2->win - p2->blackwin << "  ºí·¢Àè½Â = " << p2->blackwin << "  ¹«½ÂºÎ : " << p2->draw << endl<<endl;
	return 0;
}

//card shuffle
void Shuffle(vector<pair<int, char>>& card) {
	for (int i = 0; i < 10000; i++) {
		int change1 = rand() % 312;//creat random index
		int change2 = rand() % 312;

		pair<int, char> temp = card[change1];
		card[change1] = card[change2];
		card[change2] = temp;
	}
}

//player2, dealer's hit satetus(sum[0]==sum[1])
//-1 returns stand (stop), 0 returns: bust, 1 returns hit (1 more draw)
int HitStatus(person* p) {
	if (p->sum[0] < 17) { return 1; }	//hit
	else if (p->sum[0] > 21) { return 0; }	//bust
	else { return -1; }	//stand
}

//Get the sum of the cards for that person. In sum[0], A is treated as 1, sum
void SumofNum(person* p) {
	vector<pair<int, char>> card = p->mycard;	//Passing the card of the person who was handed in
	int size = card.size();			//Number of cards handed over by the person
	int s[2] = { 0, };				//s[0] treats all A as 1, s[1] treats 1 A as 11

	for (int i = 0; i < size; i++) {	//Repeat as many cards as the person has
		int q = card[0].first;			//card number
		card.erase(card.begin());		//sumed card erase

		if (q > 10) {	//If the card number is J, Q, K
			s[0] += 10;	s[1] += 10;
		}
		else if (q == 1) {	//For A
			s[0] += 11;	s[1] += 11;
		}
		else {	//card number is 2-10
			s[0] += q;	s[1] += q;
		}
	}
	//Save the card sum variable of the person (s[0]: minimum value, s[1]: maximum value)
	p->sum[0] = s[0];
	p->sum[1] = s[1];

	if (s[0] == 21 || s[1] == 21) {
		if (size == 2) {
			p->blackjack = 1;
		}
	}
}

//A function that determines hit and stand through probability
//-1 returns stand (stop), 0 returns: bust, 1 returns hit (1 more draw)
int probHitStatus(person* p, person* d, int* cc) {
	if (p->blackjack == 1) { return -1; }//stand if player is blackjack
	if (p->sum[0] > 21) { return 0; }//bust
	if (p->sum[1] <= 10) { return 1; }//hit

	//Predicting the dealer's value
	int dealersum = 0;
	if (d->mycard.at(1).first == 1) { dealersum = 21; }
	else if (d->mycard.at(1).first > 10) { dealersum = 20; }
	else { dealersum = d->mycard.at(1).first + 10; }

	double nbustprob = 0.0, pmax_nbustprob = 0.0, pmin_nbustprob = 0.0;
	int allnum = 0, nbustnum = 0;
	int bustlimit = 21 - dealersum;
	int pmax_bustnum = 0, pmin_bustnum = 0;
	int pmax_bustlimit = 21 - (p->sum[1]), pmin_bustlimit = 21 - (p->sum[0]);

	for (int i = 0; i < 13; i++) {
		allnum += cc[i];	//Number of cards used so far
		if (i != 0 && i < bustlimit) { nbustnum += cc[i]; }	//number of values that do not cause bust to be used 
		if (i != 0 && i < pmax_bustlimit) { pmax_bustnum += cc[i]; }
		if (i != 0 && i < pmin_bustlimit) { pmin_bustnum += cc[i]; }
	}

	//The probability of not being bust when hit
	nbustprob = (double)(24 * (bustlimit - 1) - nbustnum) / (double)(312 - allnum);
	pmax_nbustprob = (double)(24 * (pmax_bustlimit - 1) - pmax_bustnum) / (double)(312 - allnum);
	pmin_nbustprob = (double)(24 * (pmin_bustlimit - 1) - pmin_bustnum) / (double)(312 - allnum);

	if (dealersum < 17) {	//Dealer expects to hit
		if (nbustprob < 0.4) { return -1; }//If the dealer has a high probability of being bust when hitting, stand.
		else {		//If the probability of not being bust is high -> hit
			if (p->sum[1] > 21) {//think that the maximum of p is bust -> only the minimum
				if (p->sum[0] >= dealersum) { return -1; }	//Stand when the minimum is greater than the expected value
				else if (pmin_nbustprob < 0.4) { return -1; }	//At least 60 bust probability when hit -> stand
				else { return 1; }
			}
			else {//The maximum is less than 21.
				if (p->sum[1] >= dealersum) { return -1; }	//If the maximum value is greater than or equal to the dealer's expected value -> stand
				else {//If the maximum is less than the dealer's expected value
					if (pmin_nbustprob < 0.4) { return -1; }	//When hit to the minimum, bust probability of more than 60 -> stand
					else { return 1; }
				}
			}
		}
	}
	else {	//Dealer expects to stand
		if (p->sum[1] > 21) {//think that the maximum of p is bust -> only the minimum
			if (p->sum[0] >= dealersum) { return -1; }	//Stand when the minimum is greater than the expected value
			else if (pmin_nbustprob < 0.4) { return -1; }	//At least 60 bust probability when hit -> stand
			else { return 1; }
		}
		else {//The maximum is less than 21.
			if (p->sum[1] >= dealersum) { return -1; }	//If the maximum value is greater than or equal to the dealer's expected value -> stand
			else {//If the maximum is less than the dealer's expected value
				if (pmin_nbustprob < 0.4) { return -1; }	//When hit to the minimum, bust probability of more than 60 -> stand
				else { return 1; }
			}
		}
	}
	return 0;
}

//player1 card sum calculation (the sum is different depending on the A value)
void probSumofNum(person* p) {
	vector<pair<int, char>> card = p->mycard;	//Passing the card of the person who was handed in
	int size = card.size();			//Number of cards handed over by the person
	int s[2] = { 0, };				//s[0] treats all A as 1, s[1] treats 1 A as 11
	int first = 0;			//0 if A is the first occurrence, 1 otherwise

	for (int i = 0; i < size; i++) {	//Repeat as many cards as the person has
		int q = card[0].first;			//card number
		card.erase(card.begin());		//sumed card erase

		if (q > 10) {	//If the card number is J, Q, K
			s[0] += 10;	s[1] += 10;
		}
		else if (q == 1) {	//For A
			s[0] += 1;
			if (first == 0) { s[1] += 11; first = 1; }	//A comes out first and adds 11 to s[1].
			else { s[1] += 1; }		//If A is not the first time, add it to 1.
		}
		else {	//card number is 2-10
			s[0] += q;	s[1] += q;
		}
	}
	//Save the card sum variable of the person (s[0]: minimum value, s[1]: maximum value)
	p->sum[0] = s[0];
	p->sum[1] = s[1];

	if (s[1] == 21 && size == 2) { p->blackjack = 1; }
}