#include <unistd.h>
#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <ctime> 
#include "RSJparser.tcc"
#include <sstream>
#include <time.h>
#include <stdio.h>
//#include <algorithm>
#include <wiringPi.h>
//#include <Windows.h>
#include <curl/curl.h>
using namespace std::chrono;
using namespace std;

// trza wziąc pod uwage zmianę czasu na zimowy, ale to już powinien system załatwić, kiedy jest podłączony do internetu

/*Struktura godziny i minuty danego dzwonka, oraz obliczona z tego godzina w sekundach od północy*/
class bellRingTime{
public:
    unsigned char hour, min;
    unsigned long long inSeconds;
};

/*Lista dzwonków dzisiejszych i jutrzejszych*/
vector<bellRingTime>bellsListToday;
vector<bellRingTime>bellsListTomorrow;
/*Bufor do wczytania JSON'a*/
stringstream buffer;

void ring() {

    
    digitalWrite(4, HIGH);
    sleep(2500);
    digitalWrite(4, LOW);
    
   cout << "dzyn dzyn" << endl;

}

/*Funckja do debugowania (usunąć wywołanie jej gdziekolwiek przy oddawaniu)*/
void displayVectors() {

    cout << "Dzien dzisiejszy" << endl;
    for (int i = 0; i < bellsListToday.size(); i++) {
        cout << "Dzwonek nr " << i + 1 << " \t " << bellsListToday[i].hour << ":" << bellsListToday[i].min << "\t  oraz w sekundach: \t" << bellsListToday[i].inSeconds << "s" << endl;
    }

    cout << "Dzien jutrzejszy" << endl;

    for (int i = 0; i < bellsListTomorrow.size(); i++) {
        cout << "Dzwonek nr " << i+1 << " \t " << bellsListTomorrow[i].hour << ":" << bellsListTomorrow[i].min << "\t  oraz w sekundach: \t " << bellsListToday[i].inSeconds << "s" << endl;
    }
}

/*Propotyp sprawdzania z listy dzwonków czy jest już ta godzina, o której ma dzwonić*/
void checkTime_andRingTime() {
    /*Pobranie czasu systemowego od północy do aktualnej godziny*/
    time_t stamp = time(NULL);
    struct tm diferencia;
    localtime_r(&stamp, &diferencia);
    int secondsFromMidnight = ((diferencia.tm_hour * 3600) + (diferencia.tm_min * 60) + (diferencia.tm_sec));
    cout << secondsFromMidnight << endl;
//	auto now = system_clock::now();
//	auto today = floor<days>(now);
//	auto second = duration_cast<seconds>(now-today);
//	int secondsFromMidnight = second;


    /*Przeszukaj w wektorze z dzwonkami godziny które pasują do aktualnej*/ 
/*    auto found = [secondsFromMidnight](const bellRingTime& item) {
        return item.inSeconds == secondsFromMidnight;
    };
    if (find_if(begin(bellsListToday), end(bellsListToday), found) != end(bellsListToday))
        ring();*/
	for(const bellRingTime &t:bellsListToday){
		if(t.inSeconds==secondsFromMidnight){
			ring();
			break;
		}
	}

    //  vector<bellRingTime>::iterator flag = std::search(vec.begin(), vec.end(), searchlist.begin(), searchlist.end(), MatchMember);
}


/*Załaduj do struktury poszczególne elementy i dodaj do wektora przechowującego wszystkie dzwonki*/
void loadRingsFromBuffer() {
    RSJresource JSON(buffer.str());
    /*Załaduj dzisiejsze i jutrzejsze dzwonki*/
    for (int i = 0; i < JSON["bells"][0].size(); i++) {
        bellRingTime hourMinSecToday;
        hourMinSecToday.hour = JSON["bells"][0][i][0].as<int>(); //Trzecia pozycja wskazuje czy chcemy minute czy godzinę, druga pozycja pokazduje którą tablicę dwuelementowa godziny i minuty chcemy (który dzwonek), pierwsza pozycja to dzień 
        hourMinSecToday.min = JSON["bells"][0][i][1].as<int>(); //Wczytaj minutę [1][i][1] do struktury, wcześniej godzinę [1][i][0]
        hourMinSecToday.inSeconds = (hourMinSecToday.hour * 3600) + (hourMinSecToday.min * 60); //Zamień na sekundy od północy 
        bellsListToday.push_back(hourMinSecToday); // Dodaj do wektora dzwonków
    }
    for (int i = 0; i < JSON["bells"][1].size(); i++) {
        bellRingTime hourMinSecTommorrow;
        hourMinSecTommorrow.hour = JSON["bells"][1][i][0].as<int>(); //Trzecia pozycja wskazuje czy chcemy minute czy godzinę, druga pozycja pokazduje którą tablicę dwuelementowa godziny i minuty chcemy (który dzwonek), pierwsza pozycja to dzień
        hourMinSecTommorrow.min = JSON["bells"][1][i][1].as<int>(); //Wczytaj minutę [1][i][1] do struktury, wcześniej godzinę [1][i][0]
        hourMinSecTommorrow.inSeconds = (hourMinSecTommorrow.hour * 3600) + (hourMinSecTommorrow.min * 60); //Zamień na sekundy od północy 
        bellsListTomorrow.push_back(hourMinSecTommorrow); // Dodaj do wektora dzwonków
    }

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

void loadIntoBufferString() {
    /*Wczytaj cały plik JSON do bufora jako string*/
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

    ifstream t("rozklad.json");
    buffer << t.rdbuf();
}

int main()
{
    /*Dodać tutaj trza będzie funkcję pobierającą z linka tego Jsona, jeśli się nie uda, działaj na lokalnym jsonie*/
    //
    //
    //

    /*Skonfiguruj port GPIO na wyjście */
    wiringPiSetup();  //for RPI GPIO
    pinMode(4, OUTPUT);

    loadIntoBufferString();  //Załaduj do stringa jsona
    loadRingsFromBuffer(); // Wyłuskaj z niego listę godzin dzwonków do wektora struktur.
   // displayVectors(); // funkcja do debugowania

    /*Tutaj trzeba poprawić, bo może się stać tak, że nie zadziała o danym czasie dzwonek*/
    while(true) {
      sleep(1000);
        checkTime_andRingTime();
    }
    return 0;
}

// cout << buffer.str();
// cout << JSON["bells"][0][0].as<int>(); //Trzecia pozycja wskazuje czy chcemy minute czy godzinę, druga pozycja pokazduje którą tablicę dwuelementowa godziny i minuty chcemy, pierwsza pozycja to dzień 
// cout << JSON["bells"][0][0].size(); // Pierwszy indeks to indeks dnia (1-7 dzień), drugi to indeks dzwonka (który dzwonek z kolei), trzeci to indeks godzina [0] lub minuta [1]

  // godzMin[0] = JSON["bells"][0][0][0].as<int>();
  // godzMin[1] = JSON["bells"][0][0][1].as<int>();

   /*Wczytaj listę dzwonków do wektora listaDzwonkowDzis jako strukturę godzinaDzwonkow */
