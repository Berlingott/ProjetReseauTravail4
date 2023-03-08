#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <clocale>

//#pragma comment(lib, "Ws2_32.lib") // utilisé seulement pour visual studio

using namespace std;

void Recherche(string, string, ofstream&);
void CheckingAddress(int, int, string, string, ofstream&);

int main(int argc, char **argv) {
	string wantedName = argv[1]; // argument du nom rechercher
    string prefixe  = argv [2]; // arguments du préfixes
    prefixe = prefixe + "." + argv [3] + ".";

	setlocale(LC_ALL, "french");  //  permet d'afficher les caractères accentués français
    ofstream myfile;
    myfile.open("Results.txt");

    thread th1(CheckingAddress, 0, 31, prefixe, wantedName, ref(myfile)); //lancement de tous les threads
    thread th2(CheckingAddress, 32, 63, prefixe, wantedName, ref(myfile));
    thread th3(CheckingAddress, 64, 95, prefixe, wantedName, ref(myfile));
    thread th4(CheckingAddress, 96, 127, prefixe, wantedName, ref(myfile));
    thread th5(CheckingAddress, 128, 159, prefixe, wantedName, ref(myfile));
    thread th6(CheckingAddress, 160, 191, prefixe, wantedName, ref(myfile));
    thread th7(CheckingAddress, 192, 223, prefixe, wantedName, ref(myfile));
    thread th8(CheckingAddress, 224, 225, prefixe, wantedName, ref(myfile));

    th1.join();//attends que tous les threads terminent
    th2.join();
    th3.join();
    th4.join();
    th5.join();
    th6.join();
    th7.join();
    th8.join();

    cout << "Vous pouvez trouver tous les hostname trouvé dans les fichiers textes généré." << endl;
    return 0;
}

void CheckingAddress(int firstThirdNumber, int lastThirdNumber, string prefixe, string wantedName, ofstream& myfile ){

    int nRet;
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    string currentAdress = prefixe;

    nRet = WSAStartup(wVersionRequested, &wsaData);

    if (wsaData.wVersion != wVersionRequested) {
        cout << "Wrong version";
        return;
    }

    for (int troisiemeChiffre = firstThirdNumber; troisiemeChiffre <= lastThirdNumber ; troisiemeChiffre++) {
        for (int quatriemeChiffre = 1; quatriemeChiffre < 2 ; quatriemeChiffre++) {
            currentAdress = currentAdress + to_string(troisiemeChiffre) + "." + to_string(quatriemeChiffre);
            Recherche(currentAdress, wantedName, myfile);
            currentAdress = prefixe;
        }
    }
};

void Recherche(string adresseip, string WantedName, ofstream& myfile){
	struct sockaddr_in socketinfo;

	char hostname[NI_MAXHOST];
	char servInfo[NI_MAXSERV];

	string Nom;
	DWORD dwRetval;

	socketinfo.sin_family = AF_INET;

	inet_pton(AF_INET, adresseip.c_str(), &socketinfo.sin_addr);

	dwRetval = getnameinfo((struct sockaddr *) &socketinfo,
		sizeof(struct sockaddr),
		hostname,
		NI_MAXHOST, servInfo, NI_MAXSERV, NI_NUMERICSERV);

	if (dwRetval == 0) { // une machine a répondu
		Nom = hostname;  // initialisation de la string Nom (pour faciliter les opérations de comparaison)

		if (adresseip != Nom) {  // si la getnameinfo n'a pas simplement retourné l'adresse IP mais un véritable nom d'ordinateur
			if (Nom.find(WantedName) != -1) {  // la chaîne a été trouvée
                myfile << "-- CIBLE TROUVÉE! -- \nLa machine à l'adresse "<< adresseip << " se nomme " << WantedName << "\n-- CIBLE TROUVÉE! -- \n";
				cout << "-- CIBLE TROUVÉE! -- \nLa machine à l'adresse "<< adresseip << " se nomme " << WantedName << "\n-- CIBLE TROUVÉE! -- \n";
			}
			else {
				myfile << "La machine à l'adresse "<< adresseip << " se nomme " << WantedName << "\n";
				cout << "La machine à l'adresse "<< adresseip << " se nomme " << WantedName << endl;
			}
		}
		else { // Aucune machine n'a répondu
            cout << "Aucune réponse pour l'adresse: " << adresseip << "\n";
		}
	}
    return;
}