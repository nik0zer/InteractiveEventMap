#include <iostream>
#include <sqlite_modern_cpp.h>
#include "sql_new.h"
using namespace  sqlite;
using namespace std;

int main() 
{
   try 
   {
      DataBase_Server& main = DataBase_Server::get_instance();
      User u1("1", "1");
      User u2("2", "2");
      main.add_user(u1);
      main.add_user(u2);


      Event ev1("1", "1", "1");
      Event ev2("2", "2", "2");
      main.add_event(ev1);
      main.add_event(ev1);
      main.add_event(ev2);

      main.print_all_events();

      system("sleep 1");

      ev1.set_id(1);
      ev1.set_archived(true);
      ev1.set_date("11.11.11");
      main.update_event(ev1);
      
      main.print_all_events();


      std::cout << "Print all not archived events:\n";
      for (const auto& item : main.get_all_events())
      {
         std::cout << item << std::endl;
      }

   }
   catch (const sqlite::sqlite_exception& e) {
        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);
    }

}