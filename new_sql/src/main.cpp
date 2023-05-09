#include <iostream>
#include <sqlite_modern_cpp.h>
#include "sql_new.h"
// using namespace  sqlite;
// using namespace std;

// int main() 
// {
//    // DataBase& b = DataBase::get_instance();
//    try 
//    {
//       // Table_Events table;
//       // table.create_table_event(b.get_db());
//       // spdlog::info("Table created");   

//       // Event ev1("1", "1", "1");
//       // table.add_event(ev1);
//       // table.print_all_events();

//       DataBase_Server& main = DataBase_Server::get_instance();
//       // User u1("1", "1");
//       // User u2("2", "2");
//       // main.add_user(u1);
//       // main.add_user(u2);


//       // Event ev1("1", "1", "1");
//       // Event ev2("2", "2", "2");
//       // main.add_event(ev1);
//       // main.add_event(ev1);
//       // main.add_event(ev2);

//       // main.verify_user(u1);
//       // main.remove_user_by_login("1");

//       main.get_last_edit_time_events();
//       main.get_last_edit_time_users();
//       main.get_users_to_sync(0);
//       main.get_events_to_sync(0);

//    }
//    catch (const sqlite::sqlite_exception& e) {
//         spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
//                                                 __FILE__, __FUNCTION__, __LINE__);
//     }

// }