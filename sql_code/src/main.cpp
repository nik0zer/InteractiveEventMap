#include <iostream>
#include <fstream>
#include "sqlite3.h"
#include "sql_tools.h"

int main() 
{
    // main_test();

    DataBase& main_base = DataBase::get_instance();
    Person per("Name 1", "");
    Person per1("Name 1", "Psw 1");
    Person per2("Name 12", "Psw 1");
    main_base.add_person(per);
    main_base.add_person(per);
    main_base.add_person(per1);
    main_base.add_person(per2);
    main_base.remove_person(per1);
    std::vector<Person> person_vector = main_base.get_all_persons();

    for (auto& item : person_vector)
    {
        std::cout << item.get_login() << ", ";
    }

    std::cout << std::endl;
    

    // DataBase::get_instance().print_persons_vector();
    // main_base.print_persons_vector();     // Почему пустой список??
    system("rm main.db");

    return(0);


}