#include <iostream>
#include <sqlite_modern_cpp.h>
#include "sql_new.h"
using namespace  sqlite;
using namespace std;

int main() {

   try {
      DataBase b;
      Table_Events table;
      table.create_table_event(b.get_db());

   }
   catch (const exception& e) {
      cout << e.what() << endl;
   }
}