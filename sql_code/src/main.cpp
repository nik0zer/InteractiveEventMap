#include <iostream>
#include <fstream>
#include "sqlite3.h"
#include "sql_tools.h"

int main() 
{
    // std::cout << "All events\n";
    // for (const auto& item : DataBase::get_instance().get_all_events())
    // {
    //     std::cout << item << std::endl;
    // }

    Event ev1("1", "1", "1");
    Event ev2("2", "2", "2");
    Event ev3("3", "3", "3");
    DataBase::get_instance().add_event(ev1);
    DataBase::get_instance().add_event(ev2);
    DataBase::get_instance().add_event(ev3);

    std::cout << "Ev1 = " << ev1 << std::endl;

    Event found = DataBase::get_instance().get_event("1");
    std::cout << "Found = " << found << std::endl;
    DataBase::get_instance().print_all_events();

    // std::cout << "All events\n";
    // for (const auto& item : DataBase::get_instance().get_all_events())
    // {
    //     std::cout << item << std::endl;
    // }

    // DataBase& main_base = DataBase::get_instance();
    // Person per1("Name 1", "Psw 1");
    // Person per2("Name 12", "Psw 1");
    // Event ev1("First event");
    // main_base.add_event(ev1);

    // std::vector<Event>all_ev = main_base.get_all_events();
    
    // std::cout << "All events:\n";
    // for (const auto& item : all_ev)
    // {
    //     std::cout << item << std::endl;
    // }
    // Event found = main_base.get_event(ev1);
    // std::cout << "Found event:\n";
    // std::cout << found;

    // main_base.add_person(per);
    // main_base.add_person(per);
    // main_base.add_person(per1);
    // main_base.add_person(per2);
    // main_base.remove_person(per1);
    // std::vector<Person> person_vector = main_base.get_all_persons();

    // for (auto& item : person_vector)
    // {
    //     std::cout << item.get_login() << ", ";
    // }

    // std::cout << std::endl;
    

    // DataBase::get_instance().print_persons_vector();
    // main_base.print_persons_vector();     // Почему пустой список??

    return(0);


}