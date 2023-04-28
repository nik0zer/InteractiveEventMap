#include <iostream>
#include <fstream>
#include "sqlite3.h"
#include "sql_tools.h"

int main() 
{
    // main_test();

    DataBase& main_base = DataBase::get_instance();
    Person per(1, "Name 1", "");
    Person per1(1, "Name 1", "Psw 1");
    Person per2(1, "Name 12", "Psw 1");
    main_base.add_person(per);
    main_base.add_person(per);
    main_base.add_person(per1);
    main_base.add_person(per2);
    main_base.request_all_persons();
    main_base.print_persons_list();
    main_base.person_exists(per);

    // DataBase::get_instance().print_persons_list();
    // main_base.print_persons_list();     // Почему пустой список??
    system("rm main.db");

    return(0);


}