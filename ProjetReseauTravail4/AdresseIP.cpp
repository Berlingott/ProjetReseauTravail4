#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>
#include <string>

#include <clocale>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

void Recherche(string, string);


void main(int argc, char **argv)
{
	string hostName = "localhost";
	hostName = argv[1];
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	int nRet;
	string adresseip = "172.16.14.39";
	string adressProf = "172.16.14.32";
	string firstAdress = "172.16.0.1";
	string lastAdress = "172.16.255.255";
	string currentAdress = "172.16.";

	setlocale(LC_ALL, "french");  //  permet d'afficher les caractères accentués français

	nRet = WSAStartup(wVersionRequested, &wsaData);
	if (wsaData.wVersion != wVersionRequested) {
		cout << "Wrong version";
		return;
	}

	for (int troisiemeChiffre = 0; troisiemeChiffre <= 255; troisiemeChiffre++)
	{
		for (int quatriemeChiffre = 1; quatriemeChiffre < 255; quatriemeChiffre++)
		{
			currentAdress = currentAdress + to_string(troisiemeChiffre) + "." + to_string(quatriemeChiffre);
			//cout << currentAdress << "\n";
			Recherche(currentAdress, hostName);
			currentAdress = "172.16.";
		}
	}


	//Recherche(adressProf);

	WSACleanup();
}

void Recherche(string adresseip, string WantedName)
{
	struct sockaddr_in socketinfo;

	char hostname[NI_MAXHOST];
	char servInfo[NI_MAXSERV];

	string Nom;
	DWORD dwRetval;
	
	socketinfo.sin_family = AF_INET;
	//socketinfo.sin_port = htons(1111);  // numéro de port bidon

	ofstream myfile;
	myfile.open("fichier.txt");

	inet_pton(AF_INET, adresseip.c_str(), &socketinfo.sin_addr);

	dwRetval = getnameinfo((struct sockaddr *) &socketinfo,
		sizeof(struct sockaddr),
		hostname,
		NI_MAXHOST, servInfo, NI_MAXSERV, NI_NUMERICSERV);

	if (dwRetval == 0)  // si le serveur a répondu
	{
		Nom = hostname;  // initialisation de la string Nom (pour faciliter les opérations de comparaison)

		if (adresseip != Nom)  // si la getnameinfo n'a pas simplement retourné l'adresse IP mais un véritable nom d'ordinateur
		{
			//myfile << "Nom trouvé pour " << adresseip << ": " << Nom << "\n";
			//cout << "Nom trouvé pour " << adresseip << ": " << Nom << "\n";
			if (Nom.find(WantedName) != -1)  // la chaîne a été trouvée
			{
				myfile << "On a trouvé "<< WantedName << " à l'adresse " << adresseip << "\n";
				cout << "On a trouvé " << WantedName << " à l'adresse " << adresseip << "\n";
			}
			else
			{
				myfile << "L'ordinateur à l'adresse " << adresseip << " a un nom mais pas celui demande " << Nom << "\n";
				cout << "L'ordinateur à l'adresse " << adresseip << " a un nom mais pas celui demande ! " << Nom << "\n";
			}

		}
		//else {
			 //myfile << "Aucun nom trouvé pour l'adresse: " << adresseip << "\n";
			 //cout << "Aucun nom trouvé pour l'adresse: " << adresseip << "\n";
		//}
	}

	return;
}