#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <chrono>
#include <thread>
using namespace std;

void menu();
void save(int** dizi, int boyut, int hamlesayaci);
void bekleme(int sure);
void oyun_aciklamasi();
void diziyiyazdir(int** array, int size);
void cikis();
void loadingBar(int total, int bar_width);
void baslatma();
void win_page();



void loadingBar(int total, int bar_width) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // Get handle to console
	for (int progress = 0; progress <= total; progress++) {
		float ratio = progress / (float)total;
		int bar_length = (int)(ratio * bar_width);

		SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << "\n\n\nWATER SORTING GAME\n\n\n\n";

		SetConsoleTextAttribute(console, FOREGROUND_BLUE); // Set text color to blue
		cout << "\n\nLoading ....\n\n\n";
		cout << " [";
		for (int i = 0; i < bar_width; i++) {
			if (i < bar_length) {
				cout << "=";
			}
			else if (i == bar_length) {
				cout << ">";
			}
			else {
				cout << " ";
			}
		}
		cout << "] " << int(ratio * 100.0) << " %\r";
		cout.flush();
		SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Set text color back to default
		this_thread::sleep_for(chrono::milliseconds(50));
		system("cls");
	}
	cout << endl;
}

void save(int** dizi, int boyut, int hamlesayaci)
{
	ofstream save("game.txt");
	if (!save.is_open())
	{
		cout << "File could not be created.";
	}
	else
	{
		save << boyut << endl;
		save << hamlesayaci << endl;

		for (int i = 0; i < 4; i++) 
		{
			for (int j = 0; j < boyut; j++) 
			{
				save << dizi[i][j] << " ";
			}
			save << endl;

		}
		save.close(); // dosyayý kapatýyoruz.
		cout << "The game save has been successfully saved!" << endl;
	}
}

void menu()
{
jump:

	cout << "-----------------------------------" << endl;
	cout << "      WATER SORTING GAME MENU      " << endl;
	cout << "-----------------------------------" << endl;
	cout << "1. Press 1 to START THE NEW GAME." << endl;
	cout << "2. Press 2 to LOAD GAME" << endl;
	cout << "3. Press 3 to HOW TO PLAY?" << endl;
	cout << "4. Press 4 to EXIT THE PROGRAM." << endl;
	int secim;
	cin >> secim;
	system("cls");
	switch (secim)
	{
	case 1: baslatma(); // normal yeni oyun baþlangýcý için
		break;
	case 2: {
		ifstream inputFile("game.txt"); // oyunu kaydetmek ve kayýtlý oyunu açmak için

		if (!inputFile.is_open())
		{
			cout << "File could not be opened." << endl;
		}
		else
		{
			int hamlesayaci2 = 1;
			int kartno;
			int hamlesayisi;
			inputFile >> kartno;
			inputFile >> hamlesayaci2;

			int** kayitlidizi = new int* [4];

			for (int i = 0; i < 4; i++) {
				kayitlidizi[i] = new int[kartno];
				for (int j = 0; j < kartno; j++) {
					inputFile >> kayitlidizi[i][j];
				}
			}
			bool game = true;
			
			while (game) {
				
				
				int** tube = kayitlidizi;
				int size = kartno;
				
				

				diziyiyazdir(kayitlidizi, kartno);

				int first, second;
				int limit = 1;
				int sayac = 0;
				if (GetAsyncKeyState(VK_SHIFT)) { // oyun esnasýnda oyunu kaydetmek için. SHIFT TUÞU ÝLE
					save(kayitlidizi, kartno, hamlesayaci2);
					system("cls");
					cout << "Your game has been successfully saved!" << endl;
					bekleme(2);
					system("cls");
					menu();
				}
				if (GetAsyncKeyState(VK_ESCAPE)) { // oyun esnasýnda doðrudan çýkýþ için. ESC ÝLE.
					system("cls");
					cikis();
					exit(0);
				}
				HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				cout << "Enter a move: ";
				cin >> first >> second;
				cout << "\nYour number of moves: " << hamlesayaci2;
				bekleme(1);
				system("cls");
				if (first > size || second > size || first <= 0 || second <= 0 || first == second) // yanlýþ hamle olursa
				{
					cout << "\nERROR! You made the wrong move. \nPlease enter a correct move.\n";
					bekleme(2);
					system("cls");
				}

				else { // doðru hamle girilirse oluþan durumlar
					hamlesayaci2++;
					sayac++;
					int control1 = 0;
					while (control1 < limit) {
						// oyuncunun girdiði ilk tüpün en üstünde bir renk var mý diye kontrol ediliyor
						if (tube[control1][first - 1] != 0) {
							// ikinci tüp boþ mu diye kontrol ediliyor
							if (tube[3][second - 1] == 0) {
								tube[3][second - 1] = tube[control1][first - 1];
								// taþýnan öðe ilk tüpteki altýndaki öðeyle ayný deðere sahipse,
								// tüp deðiþikliðini tekrar et ve döngüyü yeniden baþlat
								if (control1 < 3 && tube[control1][first - 1] == tube[control1 + 1][first - 1]) {
									tube[control1][first - 1] = 0;
									control1++;
									continue;
								}
								tube[control1][first - 1] = 0;
							}
							// Eðer ikinci tüp dolu ise, ayný renkli öðeyi aramaya baþla
							else {
								for (int j = 3; j > 0; j--) {
									if (tube[j][second - 1] == tube[control1][first - 1] &&
										tube[j - 1][second - 1] == 0) {
										tube[j - 1][second - 1] = tube[control1][first - 1];
										// taþýnan öðe ilk tüpteki altýndaki öðeyle ayný deðere sahipse,
										// tüp deðiþikliðini tekrar et ve döngüyü yeniden baþlat
										if (control1 < 3 && tube[control1][first - 1] == tube[control1 + 1][first - 1]) {
											tube[control1][first - 1] = 0;
											control1++;
											continue;
										}
										tube[control1][first - 1] = 0;
									}
								}
							}
						}
						else {
							// Eðer ilk tüp boþ ise, limit deðiþkenini bir arttýr ve döngüyü bitir
							limit++;
						}
						control1++;
					}
				}
				///////////////////// oyunun kazanýlmasýnýn kontrol edilmesi
				int cont1, cont2;
				bool win = false;

				for (int i = 0; i < size && !win; i++)
				{
					for (int j = 1; j < 4 && !win; j++)
					{
						cont1 = tube[j - 1][i];
						cont2 = tube[j][i];

						if (cont1 != cont2)
						{
							continue;
						}
						else {
							sayac++;
						}
						// Sayac bu deðere ulaþýrsa oyuncu oyunu kazanmýþ olur ve win_page iþlevi çaðrýlýr. Game deðiþkeni daha sonra oyunun bittiðini belirtmek için false olarak ayarlanýr. Program daha sonra oyuncunun yaptýðý hamle sayýsýný yazdýrýr ve tekrar oynamak isteyip istemediðini sorar.
						if (sayac == 3 * size)
						{
							system("cls");
							HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
							SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
							cout << "CONGRATULATIONS!\nYou won the game!\n\nNumber of moves:" << hamlesayaci2 << endl;
							bekleme(2);
							win_page();
							win = true;
							game = false;
						}
					}
				}
				
			}

			for (int i = 0; i < 4; i++) {
				delete[] kayitlidizi[i];
			}
			delete[] kayitlidizi;
			inputFile.close();
		}

		break;
	}
	case 3: oyun_aciklamasi();
		system("cls");
		goto jump;
		break;
	case 4: cikis();
		exit(0);
		break;
	default: cout << "You made an invalid choice. Please try again.";
		bekleme(1);
		system("cls");
		goto jump;

		break;
	}

}	

void oyun_aciklamasi()
{
	cout << "		Welcome to Water Sort Puzzle Game!" << endl;

	cout << "         ________________________________________________________" << endl;
	cout << "        |     In this game, your goal is to sort the water       |" << endl;
	cout << "        |          in the containers by color.                   |" << endl;
	cout << "        |                                                        |" << endl;
	cout << "        |     To do this, simply pour water from one container   |" << endl;
	cout << "        |  to another, but be careful not to mix the colors!     |" << endl;
	cout << "        |                                                        |" << endl;
	cout << "        |          Are you ready to play? Let's get started!     |" << endl;
	cout << "        |                                                        |" << endl;
	cout << "         -------------------------------------------------------- " << endl;
	for (int i = 10; i > 0; i--) {
		cout << "You will be redirected to the main menu in " << i << " seconds.\r";
		cout.flush();
		bekleme(1);
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {

	loadingBar(25, 100);
	menu();

	return 0;
}

//***********
void diziyiyazdir(int** array, int size) {

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	for (int j = 0; j < size; j++) {
		cout << "  " << j + 1 << "  ";
	}
	
	SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	cout << endl;
	cout << endl;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < size; j++) {
			for (int k = 1; k < size; k++)
			{
				if (*(*(array + i) + j) == 0)
				{
					SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				}
				else if (*(*(array + i) + j) == k)
				{
					SetConsoleTextAttribute(console, k);
				}
			}
			cout << "| " << *(*(array + i) + j) << " |";
		}
		cout << endl;
	}
	
}


//************


void baslatma()
{
	int size;
	loop:
	cout << "How many tubes do you want to play the game with? ";
	cin >> size;
	system("cls");
	if (size < 4)
	{
		cout << "Please enter a number of 4 or greater." << endl;
		goto loop;
	}

	cout << "Random tubes are generated. The game is starting..." << endl;
	cout << "If you press the SHIFT key, your game will be saved immediately after entering your move." << endl;
	cout << "If you press the ESC key, you will exit the game directly after entering your move." << endl << endl;
	for (int i = 5; i > 0; i--) {
		cout << "Game will open in " << i << " seconds.\r";
		cout.flush();
		bekleme(1);
	}
	system("cls");

	//tüpteki tüm renkler. Burada pointer kullanarak yazdým.
	
	int** tube = new int* [4];
	for (int i = 0; i < 4; i++)
	{
		//dinamik olarak iki boyutlu (4*size) olacak þekilde yer ayrýlmasý;
		*(tube + i) = new int[size]; // bu da ayný þey --> tube[i] = new int[size], 
		for (int j = 0; j < size; j++)
		{
			*(*(tube+i)+j) = 0; // tüm elemanlarý 0 yapma iþlemi. Yoksa pointer NULL deðer atýyor.
		}
	}

	//sýfýrdan farklý olan renkler için iki boyutlu dizi. Bunun için aþaðýda rastgele sayýlarla dolduracaðýz.
	int** colours = new int* [4];
	for (int i = 0; i < 4; i++)
	{
		*(colours + i) = new int[size - 2]; // dinamik olarak iki boyutlu (4*(size-2)) olacak þekilde yer ayrýlmasý;
	}

	int number = 1;
	int control = 0; // her 4 seferde 1 yeni sayý atamasý yapýlmasý için kontrol deðiþkeni. Çünkü size-2 kadar farklý renk olmak zorunda.
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < size - 2; j++)
		{
			*(*(colours + i) + j) = number;
			control++;
			if (control == 4)
			{
				number++;
				control = 0;
			}
		}

	}
	srand(time(NULL));
	int control1 = 0;
	//rastgele renk atamasý iþlemi
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < size - 2; j++)
		{
		jump:
			int row = rand() % 4; // rastgele satýr seçilmesi
			int col = rand() % (size - 2); // rastgele sütun seçilmesi
			while (colours[row][col] == 0)
			{
				goto jump;
			}
			*(*(tube + i) + j) = *(*(colours + row) + col);
			*(*(colours + row) + col) = 0;
			control1++;
			if (control1 == 4 * (size - 2))
			{
				break;
			}

		}
	}

	int numbermove = 0;
	bool game = true;
	int hamlesayaci = 1;
	while (game)
	{
		diziyiyazdir(tube, size);
		int first, second;
		int limit = 1;
		int sayac = 0;
		if (GetAsyncKeyState(VK_SHIFT)) { // oyun esnasýnda oyunu kaydetmek için. SHIFT TUÞU ÝLE
			save(tube, size, hamlesayaci-1);
			system("cls");
			cout << "Your game has been successfully saved!" << endl;
			bekleme(2);	
			system("cls");
			menu();
		}
		if (GetAsyncKeyState(VK_ESCAPE)) { // oyun esnasýnda doðrudan çýkýþ için. ESC ÝLE.
			system("cls");
			cikis();
			exit(0);
		}
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << "Enter a move: ";
		cin >> first >> second;
		cout << "\nYour number of moves: " << hamlesayaci;
		bekleme(1);
		system("cls");
		if (first > size || second > size || first <= 0 || second <= 0 || first == second) // yanlýþ hamle olursa
		{
			cout << "\nERROR! You made the wrong move. \nPlease enter a correct move.\n";
			bekleme(2);
			system("cls");
		}

		else { // doðru hamle girilirse oluþan durumlar

			hamlesayaci++;
			int control1 = 0;
			while (control1 < limit) {
				// oyuncunun girdiði ilk tüpün en üstünde bir renk var mý diye kontrol ediliyor
				if (tube[control1][first - 1] != 0) {
					// ikinci tüp boþ mu diye kontrol ediliyor
					if (tube[3][second - 1] == 0) {
						tube[3][second - 1] = tube[control1][first - 1];
						// taþýnan öðe ilk tüpteki altýndaki öðeyle ayný deðere sahipse,
						// tüp deðiþikliðini tekrar et ve döngüyü yeniden baþlat
						if (control1 < 3 && tube[control1][first - 1] == tube[control1 + 1][first - 1]) {
							tube[control1][first - 1] = 0;
							control1++;
							continue;
						}
						tube[control1][first - 1] = 0;
					}
					// Eðer ikinci tüp dolu ise, ayný renkli öðeyi aramaya baþla
					else {
						for (int j = 3; j > 0; j--) {
							if (tube[j][second - 1] == tube[control1][first - 1] &&
								tube[j - 1][second - 1] == 0) {
								tube[j - 1][second - 1] = tube[control1][first - 1];
								// taþýnan öðe ilk tüpteki altýndaki öðeyle ayný deðere sahipse,
								// tüp deðiþikliðini tekrar et ve döngüyü yeniden baþlat
								if (control1 < 3 && tube[control1][first - 1] == tube[control1 + 1][first - 1]) {
									tube[control1][first - 1] = 0;
									control1++;
									continue;
								}
								tube[control1][first - 1] = 0;
							}
						}
					}
				}
				else {
					// Eðer ilk tüp boþ ise, limit deðiþkenini bir arttýr ve döngüyü bitir
					limit++;
				}
				control1++;
			}
		}

		
		///////////////////// oyunun kazanýlmasýnýn kontrol edilmesi
		int cont1, cont2;
		bool win = false;

		for (int i = 0; i < size && !win; i++)
		{
			for (int j = 1; j < 4 && !win; j++)
			{
				cont1 = tube[j - 1][i];
				cont2 = tube[j][i];

				if (cont1 != cont2)
				{
					continue;
				}
				else {
					sayac++;
				}
				// Sayac bu deðere ulaþýrsa oyuncu oyunu kazanmýþ olur ve win_page iþlevi çaðrýlýr. Game deðiþkeni daha sonra oyunun bittiðini belirtmek için false olarak ayarlanýr. Program daha sonra oyuncunun yaptýðý hamle sayýsýný yazdýrýr ve tekrar oynamak isteyip istemediðini sorar.
				if (sayac == 3 * size)
				{
					system("cls");
					HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
					SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
					cout << "CONGRATULATIONS!\nYou won the game!\n\nNumber of moves:" << hamlesayaci << endl;
					bekleme(2);
					win_page();
					win = true;
					game = false;
				}
			}
		}
	}


	//dinamik bellekle oluþturulan dizilerin hafýzada serbest býrakýlmasý.
	for (int i = 0; i < 4; i++)
	{
		delete[] tube[i];
		delete[] colours[i];
	}
	delete[] tube;
	delete[] colours;
}




//***********

void win_page() {
	system("cls");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "Do you want to play again? " << endl;
	cout << "Press 1 to PLAY AGAIN." << endl;
	cout << "Press 2 to RETURN TO THE MAIN MENU." << endl;
	cout << "Press 3 to EXIT THE GAME." << endl;
	int secim;
	cin >> secim;
	system("cls");
	switch (secim)
	{
	case 1: baslatma();
		break;
	case 2: menu();
		break;
	case 3: cikis();
		exit(0);
		break;
	}
}

void bekleme(int saniye)
{
	this_thread::sleep_for(std::chrono::seconds(saniye));
}
void cikis()
{
	cout << "You have successfully exited the game." << endl;
	cout << "We are waiting for you again." << endl << endl;
}