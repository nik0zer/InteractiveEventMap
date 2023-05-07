#include <iostream>
#include <sqlite_modern_cpp.h>
#include "sql_new.h"
using namespace  sqlite;
using namespace std;

int main() 
{
   DataBase b;
   try 
   {
      *b.get_db() << "CREATE TABLE IF NOT EXISTS EVENTS ("
               "   _id INT PRIMARY KEY AUTOINCREMENT NOT NULL,"
               "   AGE INT,"
               "   NAME TEXT,"
               "   weight real"
                                 ");";

      *b.get_db() << "CREATE TABLE IF NOT EXISTS EVENTS1 ("
                        "ID INT PRIMARY KEY AUTOINCREMENT NOT NULL, "
                        "NAME                      TEXT    NOT NULL, "
                        "INFO                      TEXT            , "
                        "ADDRESS                   TEXT            , "
                        "DATE                      TEXT            , "
                        "TIME                      TEXT            , "
                        "OWNER                     TEXT            , "
                        "LAST_EDIT_TIME            INT             );";

      spdlog::info("Table created");


      *b.get_db() << "insert into events (name) values (?);"
         << u"bob";

      // *b.get_db() << "insert into events (NAME) values (?);" << u"supername";

      spdlog::info("Inserted");

   }
   catch (const sqlite::sqlite_exception& e) {
      spdlog::error("{}: {} during {}", e.get_code(), e.what(), e.get_sql());
   }
}


int test()
{
   DataBase b;
   try {
      Table_Events table;
      table.create_table_event(b.get_db());

      Event ev1("Name1", "date 1", "time 1");
      table.add_event(ev1);
   }
   catch (const sqlite::sqlite_exception& e) {
      spdlog::error("{}: {} during {}", e.get_code(), e.what(), e.get_sql());
   }
}