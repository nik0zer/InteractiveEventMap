#pragma once

#include <set>
#include <ctime>
#include <string>
#include <vector>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include "sqlite3.h"
#include <spdlog/spdlog.h>
#include <spdlog/spdlog.h>
#include <sqlite_modern_cpp.h>


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

    int         get_id()             const      {return id_;}
    std::string get_login()          const      {return login_;}
    std::string get_password()       const      {return password_;}
    time_t      get_last_edit_time() const      {return last_edit_time_;}


    // Constructors
    Person(){};
    Person(int id, std::string login, std::string psw, time_t time): id_(id), login_(login), password_(psw), last_edit_time_(time){}
    Person(std::string login, std::string psw): Person(-1, login, psw, std::time(nullptr)){}
    Person(std::string msg);


    operator std::string() const;


    // friend DataBase;
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

    int         get_id()              const     {return id_;}
    std::string get_name()            const     {return name_;}
    std::string get_info()            const     {return info_;}
    std::string get_address()         const     {return address_;}
    std::string get_date()            const     {return date_;}
    std::string get_time()            const     {return time_;}
    std::string get_owner()           const     {return owner_;}
    time_t      get_last_edit_time()  const     {return last_edit_time_;}


    Event(){}
    Event(int id, std::string name, std::string info, std::string address, std::string date, std::string time, 
          std::string owner, time_t last_edit_time): id_(id), name_(name), info_(info), address_(address), date_(date),
          time_(time), owner_(owner), last_edit_time_(last_edit_time){}
    Event(std::string name, std::string date, std::string time): Event(0, name, "", "", date, time, "", std::time(nullptr)){}
    Event(std::string msg);


    operator std::string() const;


    // friend DataBase;
    friend std::ostream& operator<< (std::ostream &out, const Event &event);
};



class DataBase
{
    sqlite::database* db_;


    sqlite3* open_db(const std::string path);

  public:
    sqlite::database* get_db() const { return db_; };

    // Initialize
    DataBase();
    ~DataBase();
};



class Table_Events
{
    sqlite::database* db_;
  public:
    void create_table_event(sqlite::database* db);
    void add_event(const Event& event);
    void print_all_events();
};