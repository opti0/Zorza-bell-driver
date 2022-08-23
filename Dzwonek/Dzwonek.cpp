#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <time.h>
#include <stdio.h>
#include "RSJparser.tcc"
#include <wiringPi.h>
//#include <curl/curl.h>
#include <unistd.h>

//#include <Windows.h>
using namespace std;

void ring() {
  digitalWrite(4, HIGH);
  sleep(2);
  digitalWrite(4, LOW);
  cout << "dzyn dzyn" << endl;
}

/*Sprawdzanie z listy dzwonków czy jest już ta godzina, o której ma dzwonić*/
void checkTime_andRingTime(int *secs, int size) {
/*Pobranie czasu systemowego od północy do aktualnej godziny*/

    /*To działa na Linuxie (do poprawy anyways)*/
   time_t stamp = time(NULL);
   struct tm diferencia;
   localtime_r(&stamp, &diferencia);
   int secondsFromMidnight = ((diferencia.tm_hour * 3600) + (diferencia.tm_min * 60) + (diferencia.tm_sec));
   cout << secondsFromMidnight << endl;

    /*To działa na Windowsie*/
    /*time_t stamp = time(NULL);
    struct tm  diferencia;
    localtime_s(&diferencia, &stamp);
    int secondsFromMidnight = ((diferencia.tm_hour * 3600) + (diferencia.tm_min * 60) + (diferencia.tm_sec));
    cout << secondsFromMidnight << endl;*/
/*Sprawdzenie w tablicy sekund czy już czas dzwonić przeglądem zupełnym (jeśli zadzwoni przerwij pętlę)*/
    for (int i = 0; i < size; i++) {
        if (secondsFromMidnight == secs[i]){ ring(); break; }
    }
}

/*Funckja do debugowania (usunąć wywołanie jej gdziekolwiek przy oddawaniu)*/
void displayArrays(int *hour, int *min, int *sec, int size ) {

    cout << "Dzien dzisiejszy" << endl;
    for (int i = 0; i < size; i++) {
        cout << "Dzwonek nr " << i + 1 << " \t " << hour[i] << ":" << min[i] << "\t  oraz w sekundach: \t" << sec[i] << "s" << endl;
    }
}

/*Załaduj do struktury poszczególne elementy i dodaj do tablic na dzwonki*/
void loadRingsFromBuffer(string &buffer, int *hour, int *min, int *seconds, int size, RSJresource JSON) {

    for (int i = 0; i < size; i++) {
        
        hour[i] = JSON["bells"][0][i][0].as<int>(); //Trzecia pozycja wskazuje czy chcemy minute czy godzinę, druga pozycja pokazduje którą tablicę dwuelementowa godziny i minuty chcemy (który dzwonek), pierwsza pozycja to dzień 
        min[i] = JSON["bells"][0][i][1].as<int>(); //Wczytaj minutę [1][i][1] do struktury, wcześniej godzinę [1][i][0]
        seconds[i] = (hour[i] * 3600) + (min[i] * 60); //Zamień na sekundy od północy 
      
    }
    displayArrays(hour, min, seconds, size);
}

void loadIntoBufferString(string &buffer) {
 /*Funckja wczytująca pod biblioteką <curl/curl.h> JSON'a z chmury pod linkiem*/
  /*  CURL* CURLHandle = curl_easy_init();
    CURLcode CURLResult;
    string JSONRaw;
    if (CURLHandle)
    {
        curl_easy_setopt(CURLHandle, CURLOPT_URL, "https://zorza.lo3.wroc.pl/timetable/api/1/bell/");
        curl_easy_setopt(CURLHandle, CURLOPT_HEADER, 0);
        curl_easy_setopt(CURLHandle, CURLOPT_FOLLOWLOCATION, 0);
        curl_easy_setopt(CURLHandle, CURLOPT_WRITEFUNCTION, CURLWriter);
        curl_easy_setopt(CURLHandle, CURLOPT_WRITEDATA, &JSONRaw);
    }
    else
    {
        //Jakby coś poszło nie tak z CURLem
    }

    curl_easy_perform(CURLHandle);
    curl_easy_cleanup(CURLHandle);
    cout << JSONRaw << endl;
    cin >> JSONRaw;*/

 /*Wczytaj cały plik JSON do bufora jako string*/
    ifstream t;
    t.open("rozklad.json");
    if (t.good())cout <<"chuj";
   getline(t, buffer);
   t.close();
}

int CURLWriter(char* data, size_t size, size_t nmemb, string* buffer) {
    fprintf(stderr, "Hello I am a function pointer\n");
    int result = 0;
    if (buffer != NULL) {
        buffer->append(data, size * nmemb);
        result = size * nmemb;
    }
    return result;
}

int main()
{
    /*Skonfiguruj port GPIO na wyjście */
    wiringPiSetup();  //for RPI GPIO
    pinMode(4, OUTPUT);
	std::cout<<"test"<<endl;
    string buffer; //Bufor do wczytania JSON'a
    fstream plik;
    plik.open("rozklad.json");
    if (plik.good())
	cout<<"Plik ok";
    else cout<<"Plik zepsuted";
    while(plik){plik>>buffer;}
    plik.close();
    loadIntoBufferString(buffer);  //Załaduj do stringa jsona
    cout << buffer << endl; // Debug (usunąć)

    RSJresource JSON(buffer); // Właduj stringa do zmiennej biblioteki "RSJparser.tcc"
    /*Tablice na dzisiejsze dzwonki w godzinach i minutach osobno*/
    int size = JSON["bells"][0].size();
    int* hour = new int[size];
    int* min = new int[size];
    int* seconds = new int[size];

    loadRingsFromBuffer(buffer, hour, min, seconds, size, JSON); // Wyłuskaj z niego listę godzin dzwonków do wektora struktur.

    /*Tutaj trzeba poprawić, bo może się stać tak, że nie zadziała o danym czasie dzwonek*/
    while(true) {
      sleep(1);
      checkTime_andRingTime(seconds, size);
    }
    return 0;
}

// cout << buffer.str();
// cout << JSON["bells"][0][0].as<int>(); //Trzecia pozycja wskazuje czy chcemy minute czy godzinę, druga pozycja pokazduje którą tablicę dwuelementowa godziny i minuty chcemy, pierwsza pozycja to dzień 
// cout << JSON["bells"][0][0].size(); // Pierwszy indeks to indeks dnia (1-7 dzień), drugi to indeks dzwonka (który dzwonek z kolei), trzeci to indeks godzina [0] lub minuta [1]

  // godzMin[0] = JSON["bells"][0][0][0].as<int>();
  // godzMin[1] = JSON["bells"][0][0][1].as<int>();

   /*Wczytaj listę dzwonków do wektora listaDzwonkowDzis jako strukturę godzinaDzwonkow */
