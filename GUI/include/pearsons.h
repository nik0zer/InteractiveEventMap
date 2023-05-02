#ifndef PEARSONS_H
#define PEARSONS_H

#include <ctime>
#include <string>
#include <iostream>

class Person
{
    int id_;
    std::string login_;
    std::string password_;
    time_t last_edit_time_;
    
  public:
    int         get_id() {return id_;}
    std::string get_login() {return login_;}
    std::string get_password() {return password_;}
    time_t      get_last_edit_time() {return last_edit_time_;}

    Person(int id, std::string login, std::string password): id_(id), login_(login), password_(password){}; 
};

class Event
{
    int id_;
    std::string name_;
    std::string info_;
    std::string address_;
    std::string date_;
    std::string time_;
    std::string owner_;
    time_t last_edit_time_;

  public:
    int get_id() {return id_;}
    std::string get_name() {return name_;}
    std::string get_address() {return address_;}
    std::string get_date() {return date_;}
    std::string get_time() {return time_;}
    std::string get_owner() {return owner_;}
    time_t get_last_edit_time() {return last_edit_time_;}

    Event(std::string name, std::string info, std::string address, std::string date, std::string time, std::string owner,time_t last_edit_time):
    name_(name), info_(info), address_(address), date_(date), time_(time), owner_(owner){}; 

};

#endif