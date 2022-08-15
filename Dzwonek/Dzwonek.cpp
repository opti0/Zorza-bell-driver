#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <ctime> 
#include "RSJparser.tcc"
#include <sstream>
#include <time.h>
#include <stdio.h>
#include <algorithm>
#include <Windows.h>
using namespace std;

// trza wziąc pod uwage zmianę czasu na zimowy, ale to już powinien system załatwić, kiedy jest podłączony do internetu

/*Struktura godziny i minuty danego dzwonka, oraz obliczona z tego godzina w sekundach od północy*/
struct bellRingTime{
    int hour, min;
    int inSeconds;
};

/*Lista dzwonków dzisiejszych i jutrzejszych*/
vector<bellRingTime>bellsListToday;
vector<bellRingTime>bellsListTomorrow;
/*Bufor do wczytania JSON'a*/
stringstream buffer;

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
    struct tm  diferencia;
    localtime_s(&diferencia, &stamp);
    int secondsFromMidnight = ((diferencia.tm_hour * 3600) + (diferencia.tm_min * 60) + (diferencia.tm_sec));
    cout << secondsFromMidnight << endl;

    /*Przeszukaj w wektorze z dzwonkami godziny które pasują do aktualnej*/ 
    auto found = [secondsFromMidnight](const bellRingTime& item) {
        return item.inSeconds == secondsFromMidnight;
    };
    if(find_if(begin(bellsListToday), end(bellsListToday), found) != end(bellsListToday)) 
    cout << "Dzyn dzyn dzwonek wstawac" << endl; // Symulacja dzwonienia xd

}


/*Załaduj do struktury poszczególne elementy i dodaj do wektora przechowującego wszystkie dzwonki*/
void loadRingsFromBuffer() {
    RSJresource JSON(buffer.str());
    /*Załaduj dzisiejsze i jutrzejsze dzwonki*/
    for (int i = 0; i < JSON["bells"][0].size(); i++) {
        bellRingTime hourMinSecToday;
        hourMinSecToday.hour = JSON["bells"][0][i][0].as<int>(); //Trzecia pozycja wskazuje czy chcemy minute czy godzinę, druga pozycja pokazduje którą tablicę dwuelementowa godziny i minuty chcemy (który dzwonek), pierwsza pozycja to dzień 
        hourMinSecToday.min = JSON["bells"][0][i][1].as<int>();
        hourMinSecToday.inSeconds = (hourMinSecToday.hour * 3600) + (hourMinSecToday.min * 60); //Zamień na sekundy od północy 
        bellsListToday.push_back(hourMinSecToday); // Dodaj do wektora dzwonków
    }
    for (int i = 0; i < JSON["bells"][1].size(); i++) {
        bellRingTime hourMinSecTommorrow;
        hourMinSecTommorrow.hour = JSON["bells"][1][i][0].as<int>(); //Trzecia pozycja wskazuje czy chcemy minute czy godzinę, druga pozycja pokazduje którą tablicę dwuelementowa godziny i minuty chcemy (który dzwonek), pierwsza pozycja to dzień
        hourMinSecTommorrow.min = JSON["bells"][1][i][1].as<int>();
        hourMinSecTommorrow.inSeconds = (hourMinSecTommorrow.hour * 3600) + (hourMinSecTommorrow.min * 60); //Zamień na sekundy od północy 
        bellsListTomorrow.push_back(hourMinSecTommorrow); // Dodaj do wektora dzwonków
    }

}
void loadIntoBufferString() {
    /*Wczytaj cały plik JSON do bufora jako string*/
    ifstream t("rozklad.json");
    buffer << t.rdbuf();
}

int main()
{   
    loadIntoBufferString();
    loadRingsFromBuffer();
    displayVectors();
    for (;;) {
        Sleep(1000);
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