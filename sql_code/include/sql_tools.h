#ifndef SQL_TOOLS
#define SQL_TOOLS

#include <iostream>
#include <sstream>
#include <fstream>
#include <set>
#include <string>
#include <vector>
#include <vector>
#include <ctime>
#include "sqlite3.h"
#include <spdlog/spdlog.h>

#ifndef NETWORK_CONNECTION_SERVER_H
#include "network_connection_client.h"
#endif


class Person;
class Event;
class DataBase;


class Person
{
    int         id_;
    std::string login_;
    std::string password_;
    time_t      last_edit_time_;

  public:

    int         get_id()              {return id_;}
    std::string get_login()           {return login_;}
    std::string get_password()        {return password_;}
    time_t      get_last_edit_time()  {return last_edit_time_;}


    // Constructors
    Person(){};
    Person(int id, std::string login, std::string psw, time_t time): id_(id), login_(login), password_(psw), last_edit_time_(time){}
    Person(std::string login, std::string psw): Person(-1, login, psw, std::time(nullptr)){}
    Person(std::string msg);


    operator std::string() const;


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
    std::string get_info()            {return info_;}
    std::string get_address()         {return address_;}
    std::string get_date()            {return date_;}
    std::string get_time()            {return time_;}
    std::string get_owner()           {return owner_;}
    time_t      get_last_edit_time()  {return last_edit_time_;}


    Event(){}
    Event(int id, std::string name, std::string info, std::string address, std::string date, std::string time, 
          std::string owner, time_t last_edit_time): id_(id), name_(name), info_(info), address_(address), date_(date),
          time_(time), owner_(owner), last_edit_time_(last_edit_time){}
    Event(std::string name, std::string date, std::string time): Event(0, name, "", "", date, time, "", std::time(nullptr)){}
    Event(std::string msg);


    operator std::string() const;


    friend DataBase;
    friend std::ostream& operator<< (std::ostream &out, const Event &event);
};



class DataBase
{
    sqlite3*              ptr_;                     // Pointer to DataBase
    std::set<int>         reserved_persons_id_;     // set of already using ids of persons
    std::set<int>         reserved_events_id_;      // set of already using ids of events
    std::vector<Person>   persons_vector_;            // List for answer from DataBase for person's creds     // Почему падает, если поле объявить статическим
    std::vector<Event>    events_vector_;             // List for answer from DataBase for events

    bool                  i_am_server;
    
    
//

    // Initialize
    DataBase();
    ~DataBase();
    sqlite3*            open_db(const std::string path);
    void                create_tables(sqlite3* DB);

    // ID
    void                fill_reserved_persons_id();
    void                fill_reserved_events_id();
    int                 get_next_id(std::set<int>& id_set);
    void                remove_id(std::set<int>& id_set, const int& i);

    // SQL callbacks
    static int          callback_person(void* data, int argc, char** argv, char** azColName);
    static int          callback_event(void* data, int argc, char** argv, char** azColName);


  public:

    // Singleton:
    static DataBase&    get_instance();

    DataBase(DataBase&) = delete;
    DataBase& operator=(const DataBase&) = delete;


    // Person base
    void                add_person(Person& new_person);    // Насколько нужно эти методы объявлять константными?
    void                remove_person(Person& person);
    bool                person_exists(Person& person);


    // Event base
    void                add_event(Event& event);
    void                remove_event(Event& event);


    // SQL:
    void                execute_sql(const std::string& sql_cmd, const std::string& table);


    // Server interaction
    void                update_database();
    time_t              get_last_edit_time_persons();
    time_t              get_last_edit_time_events();
    int                parse_cmd(std::string cmd, std::string data);


    // Logic (for buttons)
    bool                person_verify(Person& person);               // Накрутить хеш, если будет время
    std::vector<Person> get_all_persons();
    std::vector<Event>  get_all_events();
    Event               get_event(std::string name);
    Event               get_event(Event& event); // Requires Name, Date, Time
    void                print_all_events();
    void                rename_event(std::string old_name, std::string new_name);
};


#endif



// Добавить в структуру флаг и коннект
// person(time) + event(time)
// see_all_events

// Написать взаимодействие с сервером
// Убрать костыль fill_reserved_id
// Подобавлять везде const &