#ifndef SQL_TOOLS
#define SQL_TOOLS

#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <list>
#include "sqlite3.h"
#include <spdlog/spdlog.h>


class Person;
class DataBase;


class Person
{
    int         id_;
    std::string login_;
    std::string password_;

  public:
    Person();
    Person(std::string login, std::string psw): login_(login), password_(psw){}
    Person(int id, std::string login, std::string psw): id_(id), login_(login), password_(psw){}

    friend DataBase;
    friend std::ostream& operator<< (std::ostream &out, const Person &person);
};



// Переписать с двумя наследниками-таблицами

class DataBase
{
    sqlite3*            ptr_;               // Pointer to DataBase
    std::set<int>       reserved_id_;       // set of already using ids
    std::list<Person>   persons_list_;      // List for answer from DataBase for person's creds
    // std::list<Event> ans_events;            // List for answer from DataBase for events

    DataBase();
    sqlite3*   open_db(const std::string path);
    void       create_tables(sqlite3* DB);

  public:

    DataBase(DataBase&) = delete;
    DataBase& operator=(const DataBase&) = delete;

    static DataBase& get_instance();
    void             add_person(Person& new_person);    // Насколько нужно эти методы объявлять константными?
    void             remove_person(Person& person);
    bool             person_exists(Person& person);
    int              get_next_id();
    void             request_all_persons();
    void             print_persons_list();
    static int       callback_creds(void* data, int argc, char** argv, char** azColName);
};



#endif