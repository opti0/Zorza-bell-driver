#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void json(){};
void database(){};

int main (int argc, char *argv[])
{
  //cout << argv[2] << endl;
  if (argc > 1)
    {
      char t = *argv[1];
      if (t == 'j'){
    	cout << "JSON";
    	json();
      }
      else if (t == 'd'){
	    cout << "Data Base";
	    database();
      }
    }
  else
    cout << "Test mode - using local db.sqlite3 file";

  return 0;
}
