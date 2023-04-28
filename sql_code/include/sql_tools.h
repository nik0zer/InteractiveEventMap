#ifndef SQL_TOOLS
#define SQL_TOOLS

#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <list>
// #include <format>
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
    Person(int id, std::string login, std::string psw): id_(id), login_(login), password_(psw){}
    // Переопределить оператор вывода
    friend std::ostream& operator<< (std::ostream &out, const Person &person);

    friend DataBase;
};


class DataBase
{
    sqlite3*      ptr_;
    std::set<int> reserved_id_;
    std::list<Person> persons_list_;
    // std::list<Event> ans_events;

    DataBase();

  public:

    DataBase(DataBase&) = delete;
    DataBase& operator=(const DataBase&) = delete;

    static DataBase& get_instance();
    void             add_person(Person& new_person);    // Насколько нужно эти методы объявлять константными?
    bool             person_exists(Person& person);
    int              get_next_id();
    void             request_all_persons();
    void             print_persons_list();
    static int       callback_creds(void* data, int argc, char** argv, char** azColName);
};


sqlite3*   open_db(const std::string path);

void       create_tables(sqlite3* DB);
int        test_db(sqlite3* DB);
void       main_test();



#endif