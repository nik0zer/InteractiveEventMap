#include <iostream>
#include <fstream>
#include "sqlite3.h"
#include "sql_tools.h"

int main() 
{
    
    Event ev1("1", "1", "1");
    Event ev2("2", "2", "2");
    Event ev3("3", "3", "3");
    DataBase::get_instance().add_event(ev1);
    DataBase::get_instance().add_event(ev2);
    DataBase::get_instance().add_event(ev3);

    Person p1("1", "1");
    DataBase::get_instance().add_person(p1);

    DataBase::get_instance().print_all_events();

    std::cout << "Events to update:\n";
    for (auto& item : DataBase::get_instance().get_persons_to_sync(1683158595))
    {
        std::cout << item << std::endl;
    }

   
    return 0;
}