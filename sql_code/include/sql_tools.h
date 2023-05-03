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
    Person(int id, std::string login, std::string psw, time_t time): id_(id), login_(login), password_(psw), last_edit_time_(time){}
    Person(std::string login, std::string psw): Person(-1, login, psw, std::time(nullptr)){}
    Person(std::string msg): Person("", "")
    {
        std::vector<std::string> data_vector;
        std::string temp;
        std::stringstream input_stream(msg);

        for (int i = 0; i < 4; i++)
        {
          std::getline(input_stream, temp);
          data_vector.push_back(temp);
          // spdlog::info("Readed part = '{}'", temp);
        }

        id_ = std::atoi(data_vector[0].c_str());
        login_ = data_vector[1];
        password_ = data_vector[2];
        last_edit_time_ = std::atol(data_vector[3].c_str());

    }


    operator std::string() const 
    {
      std::string str = std::to_string(id_) + "\n";
      str += login_ + "\n";
      str += password_ + "\n";
      str += std::to_string(last_edit_time_) + "\n";
      return str;
    }

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

    Event(int id, std::string name, std::string info, std::string address, std::string date, std::string time, 
          std::string owner, time_t last_edit_time): id_(id), name_(name), info_(info), address_(address), date_(date),
          time_(time), owner_(owner), last_edit_time_(last_edit_time){}
    Event(std::string name, std::string date, std::string time): Event(0, name, "", "", date, time, "", std::time(nullptr)){}

    Event(std::string msg): Event("", "", "")
    {
        std::vector<std::string> data_vector;
        std::string temp;
        std::stringstream input_stream(msg);

        for (int i = 0; i < 8; i++)
        {
          std::getline(input_stream, temp);
          data_vector.push_back(temp);
          // spdlog::info("Readed part = '{}'", temp);
        }

        id_             = std::atoi(data_vector[0].c_str());
        name_           = data_vector[1];
        info_           = data_vector[2];
        address_        = data_vector[3];
        date_           = data_vector[4];
        time_           = data_vector[5];
        owner_          = data_vector[6];
        last_edit_time_ = std::atol(data_vector[7].c_str());

    }

    operator std::string() const 
    {
        std::string str = std::to_string(id_) + "\n";
        str += name_ + "\n";
        str += info_ + "\n";
        str += address_ + "\n";
        str += date_ + "\n";
        str += time_ + "\n";
        str += owner_ + "\n";
        str += std::to_string(last_edit_time_) + "\n";
        return str;
    }

    friend DataBase;
    friend std::ostream& operator<< (std::ostream &out, const Event &event);
};



// Переписать с двумя наследниками-таблицами?

class DataBase
{
    sqlite3*              ptr_;                     // Pointer to DataBase
    std::set<int>         reserved_persons_id_;     // set of already using ids of persons
    std::set<int>         reserved_events_id_;      // set of already using ids of events
    std::vector<Person>   persons_vector_;            // List for answer from DataBase for person's creds     // Почему падает, если поле объявить статическим
    std::vector<Event>    events_vector_;             // List for answer from DataBase for events

    // Initialize
    DataBase();
    sqlite3*   open_db(const std::string path);
    void       create_tables(sqlite3* DB);

    // Destructor
    ~DataBase()
    {
        if (sqlite3_close(ptr_) == SQLITE_OK)
        {
            spdlog::info("Database closed");
        }
        else
        {
            spdlog::critical("Can't close Database!");
        }
    }

    // ID
    int        get_next_id(std::set<int>& id_set);
    void       remove_id(std::set<int>& id_set, const int& i);


  public:
    void       fill_reserved_persons_id();
    void       fill_reserved_events_id();

    // Singleton:
    static DataBase& get_instance();
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
    static int          callback_person(void* data, int argc, char** argv, char** azColName);
    static int          callback_event(void* data, int argc, char** argv, char** azColName);

    // Logic
    bool                person_verify(Person& person);               // Накрутить хеш
    std::vector<Person> get_all_persons();
    std::vector<Event>  get_all_events();
    Event               get_event(std::string name);
    Event               get_event(Event& event); // Requires Name, Date, Time
    void                parse_cmd(std::string cmd, std::string data);
    void                print_all_events();
    void                rename_event(std::string old_name, std::string new_name);
};


#endif


// Убрать костыль fill_reserved_id