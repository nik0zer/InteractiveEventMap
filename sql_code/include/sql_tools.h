#ifndef SQL_TOOLS
#define SQL_TOOLS

#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <list>
#include <ctime>
#include "sqlite3.h"
#include <spdlog/spdlog.h>


class Person;
class DataBase;


class Person
{
    int         id_;
    std::string login_;
    std::string password_;
    time_t      last_edit_time_;

  public:

    int         get_id() {return id_;}
    std::string get_login() {return login_;}
    std::string get_password() {return password_;}
    time_t      get_last_edit_time() {return last_edit_time_;}

    Person();
    Person(int id, std::string login, std::string psw): id_(id), login_(login), password_(psw){}
    Person(std::string login, std::string psw): Person(-1, login, psw){}

    friend DataBase;
    friend std::ostream& operator<< (std::ostream &out, const Person &person);
};



class Event
{
    int         id_;
    std::string name_;
    std::string info_;
    std::string address_;
    std::string date_;
    std::string time_;
    std::string owner_;
    time_t      last_edit_time_;

  public:

    int         get_id()              {return id_;}
    std::string get_name()            {return name_;}
    std::string get_address()         {return address_;}
    std::string get_date()            {return date_;}
    std::string get_time()            {return time_;}
    std::string get_owner()           {return owner_;}
    time_t      get_last_edit_time()  {return last_edit_time_;}

    Event(int id, std::string name, std::string info, std::string address, std::string date, std::string time, 
          std::string owner, time_t last_edit_time): id_(id), name_(name), info_(info), address_(address), date_(date),
          time_(time), owner_(owner), last_edit_time_(last_edit_time){}

    friend DataBase;
    friend std::ostream& operator<< (std::ostream &out, const Event &event);
};



// Переписать с двумя наследниками-таблицами?

class DataBase
{
    sqlite3*            ptr_;                     // Pointer to DataBase
    std::set<int>       reserved_persons_id_;     // set of already using ids of persons
    std::set<int>       reserved_events_id_;      // set of already using ids of events
    std::list<Person>   persons_list_;            // List for answer from DataBase for person's creds
    std::list<Event>    events_list_;             // List for answer from DataBase for events

    DataBase();
    sqlite3*   open_db(const std::string path);
    void       create_tables(sqlite3* DB);

    int        get_next_person_id();

  public:

    DataBase(DataBase&) = delete;
    DataBase& operator=(const DataBase&) = delete;

    static DataBase& get_instance();
    void execute_sql(const std::string& sql_cmd, const std::string& table);

    // Person part
    void             add_person(Person& new_person);    // Насколько нужно эти методы объявлять константными?
    void             remove_person(Person& person);
    bool             person_exists(Person& person);

    void             request_all_persons();
    void             print_persons_list();
    static int       callback_person(void* data, int argc, char** argv, char** azColName);
    static int       callback_event(void* data, int argc, char** argv, char** azColName);

    // Event part
    void             add_event(Event& new_person);
    // void             remove_person(Person& person);
    // bool             person_exists(Person& person);

    // void             request_all_persons();
    // void             print_persons_list();
    // static int       callback_person(void* data, int argc, char** argv, char** azColName);

};



#endif