#include <iostream>
#include <string>
#include <fstream>
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

//carica dati
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

//visualizza dati
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

//ordinamento per numero civico
void Swap(NumerazioneCivica &a, NumerazioneCivica &b){
	NumerazioneCivica temp=a;
	a=b;
	b=temp;
}
void OrdinaCivico(NumerazioneCivica v[], int d){
	//bubblesort
	for(int i=0; i<d-1; i++){
		for(int j=0; j<d-1; j++){
			if(v[j].numero>v[j+1].numero){
				swap(v[j], v[j+1]);
			}		
		}
	}	
}


//mostra tutti i numeri civici di quella via in ordine
void CercaVia(NumerazioneCivica n, NumerazioneCivica v[], int d){

}


int main() {
    int opzione;
    NumerazioneCivica x;
    NumerazioneCivica dati[10];
    int d=sizeof(dati)/sizeof(dati[0]);

    do{
        cout<<"1 - carica dati\n2 - visualizza dati\n3 - ordina numero civico\n4 - cerca via \n0 - stop";
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
				//visualizza vie ordinate per numero 
				cout<<endl;
				//ordino array per numero
				OrdinaCivico(dati, d);
				//visualizzo
				cout<<endl<<VisualizzaDati(dati, d)<<endl;
				cout<<endl;
				break;
			}	
        }
    }
	while(opzione!=0);

    return 0;
}
