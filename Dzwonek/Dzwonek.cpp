#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime> 
#include "RSJparser.tcc"
#include <sstream>
#include <time.h>
#include <stdio.h>
#include <algorithm>

#include <Windows.h>
using namespace std;

//_CRT_SECURE_NO_WARNINGS;
// trza wziąc pod uwage zmianę czasu na zimowy 



struct bellRingTime{
    int hour, min;
    int inSeconds;
};
//int inSecondsToday[22], inSecondsTomorrow[22];

vector<bellRingTime>bellsListToday;
vector<bellRingTime>bellsListTomorrow;
stringstream buffer;

/*Funckja do debugowania (usunąć wywołanie jej gdziekolwiek przy oddawaniu)*/
void displayVectors() {

    cout << "Dzien dzisiejszy" << endl;
    for (int i = 0; i < bellsListToday.size(); i++) {
        cout << bellsListToday[i].hour << " " << bellsListToday[i].min << " " << bellsListToday[i].inSeconds << endl;
    }

    cout << "Dzien jutrzejszy" << endl;

    for (int i = 0; i < bellsListTomorrow.size(); i++) {
        cout << bellsListTomorrow[i].hour << " " << bellsListTomorrow[i].min << " " << bellsListToday[i].inSeconds << endl;
    }
}


void checkTime_andRingTime() {

  /*  time_t godzina = time(0);
    tm data;
    localtime_s(&data, &godzina);
    cout << data.tm_hour << ":" << data.tm_min << ":" << data.tm_sec << endl;*/

    time_t stamp = time(NULL);
    struct tm* diferencia = localtime(&stamp);
    int secondsFromMidnight = ((diferencia->tm_hour * 3600) + (diferencia->tm_min * 60) + (diferencia->tm_sec));
   // cout << secondsFromMidnight << endl;

    //if (find(bellsListToday.begin(), bellsListToday.end(), secondsFromMidnight) != bellsListToday.end()) {
    //    cout << "Dzyn dzyn dzwonek wstawac" << endl;
    //}
    
  
    auto found = [secondsFromMidnight](const bellRingTime& item) {
        return item.inSeconds == secondsFromMidnight;
    };
    if(find_if(begin(bellsListToday), end(bellsListToday), found) != end(bellsListToday))
  //  vector<bellRingTime>::iterator flag = std::search(vec.begin(), vec.end(), searchlist.begin(), searchlist.end(), MatchMember);
    cout << "Dzyn dzyn dzwonek wstawac" << endl;

}


/*Załaduj do struktury poszczególne elementy i dodaj do wektora przechowującego wszystkie dzwonki*/
void loadRingsFromBuffer() {
    RSJresource JSON(buffer.str());
    /*Załaduj dzisiejsze i jutrzejsze dzwonki*/
    for (int i = 0; i < JSON["bells"][0].size(); i++) {
        bellRingTime hourMinSecToday;
        hourMinSecToday.hour = JSON["bells"][0][i][0].as<int>();
        hourMinSecToday.min = JSON["bells"][0][i][1].as<int>();
        hourMinSecToday.inSeconds = (hourMinSecToday.hour * 3600) + (hourMinSecToday.min * 60);
        bellsListToday.push_back(hourMinSecToday);
    }
    for (int i = 0; i < JSON["bells"][1].size(); i++) {
        bellRingTime hourMinSecTommorrow;
        hourMinSecTommorrow.hour = JSON["bells"][1][i][0].as<int>();
        hourMinSecTommorrow.min = JSON["bells"][1][i][1].as<int>();
        hourMinSecTommorrow.inSeconds = (hourMinSecTommorrow.hour * 3600) + (hourMinSecTommorrow.min * 60);
        bellsListTomorrow.push_back(hourMinSecTommorrow);
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