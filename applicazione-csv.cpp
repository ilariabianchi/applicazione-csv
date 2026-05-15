#include <iostream>
#include <string>
#include <fstream>
//per convertire in maiuscolo
#include <algorithm>
#include <cctype>
using namespace std;

struct Location {
    double latitudine1;
    double longitudine1;
};

struct NumerazioneCivica {
    string classe;
    string descrizione;
    string numero;
    string subalterno;
    string cap;
    string istat;
    double latitudine;
    double longitudine;
    Location posto;
};

bool CaricaDati(NumerazioneCivica v[], int righe_tot){
	//apro il file
    ifstream leggi("Comune_Bergamo_-_Numerazione_civica.csv");
    //se il file non è aperto, ritorno falso
    if(!leggi.is_open()) {
        return false;
    }
    string riga, dati;
    //salto la prima riga di intestazione
    getline(leggi, riga);
    int i=0;
    //controllo che il ciclo si ripeta per il numero delle righe e che non sia alla fine del file
    while (i<righe_tot && !leggi.eof()){
    	//leggo il primo campo della riga e se siamo già alal fine del file esco dal ciclo
        getline(leggi, v[i].classe, ',');
        if(leggi.eof()){
        	break;	
		}
		//leggo gli altri campi separati dalla virgola
        getline(leggi, v[i].descrizione, ',');
        getline(leggi, v[i].numero, ',');
        getline(leggi, v[i].subalterno, ',');
        getline(leggi, v[i].cap, ',');
        getline(leggi, v[i].istat, ',');
		//converto da stringa a double
        getline(leggi, dati, ',');
        v[i].latitudine=stod(dati);
		//alla fine della riga si va a capo e la virgola non c'è più
        getline(leggi, dati, '\n');
        v[i].longitudine=stod(dati);
		//copio gli stessi valori latitudine e longitudine in Location
        v[i].posto.latitudine1=v[i].latitudine;
        v[i].posto.longitudine1=v[i].longitudine;
        i++;
    }
	//chiudo il file
    leggi.close();
    return true;
}

string VisualizzaDati(NumerazioneCivica v[], int d) {
    string s;
    for(int i=0; i<d; i++){
        s+=v[i].classe+"\t";
        s+=v[i].descrizione+"\t";
        s+=v[i].numero+"\t";
        s+=v[i].subalterno+"\t";
        s+=v[i].cap+"\t";
        s+=v[i].istat+"\t";
        s+=to_string(v[i].latitudine)+"\t";
        s+=to_string(v[i].longitudine)+"\n";
    }
    //ritorno tutta la stringa
    return s;
}

void Swap(NumerazioneCivica &a, NumerazioneCivica &b){
	NumerazioneCivica temp=a;
	a=b;
	b=temp;
}
void OrdinaCivico(NumerazioneCivica v[], int d){
	//bubblesort
	for(int i=0; i<d-1; i++){
		for(int j=0; j<d-1; j++){
			//converto i numeri letti come stringhe in int
			int n1=stoi(v[j].numero);
            int n2=stoi(v[j+1].numero);
            if(n1>n2){
                Swap(v[j], v[j+1]);
            }		
		}
	}	
}

int CercaVia(string descr, NumerazioneCivica v[], NumerazioneCivica corrispondenti[], int d){
	//faccio un array in cui inserirò tutte le vie corrisponenti alla ricerca
	int vie=0;
	for(int i=0; i<d; i++){
		//se nell'array oroginale trovo il nome che corrisponde a quello che sto cercando lo copio nell'array con solo quelle vie
        if(v[i].descrizione==descr){
        	//copio
            corrispondenti[vie]=v[i];
            vie++;
        }
    }
    //numero per ordine civico le vie che ho cercato
	OrdinaCivico(corrispondenti, vie);
	return vie;
}

//ordino i dati istat
void OrdinaIstat(NumerazioneCivica v[], int d){
    for(int i=0; i<d; i++){
        for(int j=0; j<d-1; j++){
            if(v[j].istat>v[j+1].istat){
                Swap(v[j], v[j+1]);
            }
        }
    }
}
//faccio la tabella report in html
bool TabellaIstat(NumerazioneCivica v[], int d){
	ofstream html("tabella_istat.html");
	//creo la struttura html
	html<<"<html>";
	html<<"<body>";
    html<<"<h1>tabella istat</h1>";
    html<<"<table border='1'>";
    html<<"<tr><th>istat</th><th>totale</th></tr>";
    //ordino gli istat
    OrdinaIstat(v, d);
    //indice per guardare tutto l'array
    int i=0;
    while(i<d){
    	//salvo il codice istat attuale
    	string codice=v[i].istat;
    	//conta quante volte appare
    	int cont=0;
    	// onto tutti quelli uguali consecutivi
        while(i<d&&v[i].istat==codice){
            cont++;
            i++;
        }
        //scrivo la riga della tabella
        html<<"<tr>";
        html<<"<td>"<<codice<<"</td>";
        html<<"<td>"<<cont<<"</td>";
        html<<"</tr>";
	}
	//chiudo tuttp
    html << "</table></body></html>";
    html.close();
    return true;
}


int main() {
    int opzione;
    NumerazioneCivica x;
    NumerazioneCivica dati[1000];
    int d=sizeof(dati)/sizeof(dati[0]);

    do{
        cout<<"APPLICAZIONE CSV\n1 - carica dati\n2 - visualizza dati\n3 - cerca via \n4 - tabella istat in html\n0 - stop";
        cout<<"\nopzione: ";
        cin>>opzione;
        switch(opzione){
            case 1: {
                if(CaricaDati(dati, d)){
                    cout<<"\ndati caricati\n\n";
                }
                //se i dati non vengono caricati si viene avvertiti
                else{
                    cout<<"\nfile non aperto\n\n";
                }
                break;
            }

            case 2:{
            	//funzione visualizza dati
            	cout<<endl<<VisualizzaDati(dati, d)<<endl;
				break;
			}
			
			case 3:{
				cout<<"\ninserisci il nome della via: ";
                fflush(stdin);
                //faccio inserire il nome della via
                string via;
                getline(cin, via);
                //funzione stringa che la trasforma tutta in  maiuscolo
                transform(via.begin(), via.end(), via.begin(), [](unsigned char c){
        			return toupper(c);
    			});
                //faccio un array in cui inserirò le vie corrispondenti a quelel che sto cercando
                NumerazioneCivica corrispondenti[1000];
                int cerca=CercaVia(via, dati, corrispondenti, d);
                if(cerca==0){
                	cout<<"nessun numero civico trovato\n\n";
				}
                else{
                	cout<<"\nci sono "<<cerca<<" numeri civici nell via che hai cercato:\n";
                	cout<<VisualizzaDati(corrispondenti, cerca)<<endl;
				}
				break;
			}
			
			case 4:{
				bool html=TabellaIstat(dati, d);
				if(html){
					cout<<"\nreport html creato\n\n";
				}
				break;
			}	
        }
    }
	while(opzione!=0);

    return 0;
}
