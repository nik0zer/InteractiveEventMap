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


class User;
class Event;
class DataBase;



//----------------------------------------------------------------
//!  @brief Class of Users -> User
//----------------------------------------------------------------

class User
{
    int         id_;
    std::string login_;
    std::string password_;
    time_t      last_edit_time_;
    bool        archived_;


  public:

    // ---------------------------------------------------------------------------------------------------------------------
    // Getters:
    // ---------------------------------------------------------------------------------------------------------------------


    int         get_id()             const      { return id_; }
    std::string get_login()          const      { return login_; }
    std::string get_password()       const      { return password_; }
    time_t      get_last_edit_time() const      { return last_edit_time_; }
    bool        get_archived()       const      { return archived_; }


    // ---------------------------------------------------------------------------------------------------------------------
    // Constructors:
    // ---------------------------------------------------------------------------------------------------------------------


    //----------------------------------------------------------------
    //!  @brief Default constructor
    //----------------------------------------------------------------

    User(){};


    //----------------------------------------------------------------
    //!  @brief FULL - constructor
    //!  
    //!  @param id    - id of User (optionally, can be any)
    //!  @param login - login
    //!  @param psw   - password
    //!  @param time  - last edit time
    //----------------------------------------------------------------

    User(int id, std::string login, std::string psw, time_t time, bool archived): id_(id), login_(login), password_(psw), last_edit_time_(time), archived_(archived){}


    //----------------------------------------------------------------
    //!  @brief Construct a new User object by "login" and "password"
    //!  aka std:string.
    //!  
    //!  @param login - login
    //!  @param psw   - password
    //----------------------------------------------------------------

    User(std::string login, std::string psw): User(-1, login, psw, std::time(nullptr), false){}


    //----------------------------------------------------------------
    //!  @brief Construct a new User object from MESSAGE! It is constructor from 
    //!   forwarding messages client-server
    //!  
    //!  @param msg - data (\n separators)
    //----------------------------------------------------------------

    User(std::string msg);


    //----------------------------------------------------------------
    //!  @brief Type conversion from User to std::string (\n - separating)
    //!  
    //!  @return std::string 
    //----------------------------------------------------------------
    operator std::string() const;


    //----------------------------------------------------------------
    //!  @brief Redefinition of "<<" for streams
    //!  
    //!  @param out 
    //!  @param User 
    //!  @return std::ostream& 
    //----------------------------------------------------------------

    friend std::ostream& operator<<(std::ostream &out, const User &User);
};



//----------------------------------------------------------------
//!  @brief Class of Events -> Uvent
//----------------------------------------------------------------

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
    bool        archived_;


  public:

    // ---------------------------------------------------------------------------------------------------------------------
    // Getters and setters
    // ---------------------------------------------------------------------------------------------------------------------


    int         get_id()              const     { return id_; }
    std::string get_name()            const     { return name_; }
    std::string get_info()            const     { return info_; }
    std::string get_address()         const     { return address_; }
    std::string get_date()            const     { return date_; }
    std::string get_time()            const     { return time_; }
    std::string get_owner()           const     { return owner_; }
    time_t      get_last_edit_time()  const     { return last_edit_time_; }
    bool        get_archived()        const     { return archived_; }


    void set_name(const std::string& new_name) { name_ = new_name; }


    // ---------------------------------------------------------------------------------------------------------------------
    // Constructors:
    // ---------------------------------------------------------------------------------------------------------------------

    //----------------------------------------------------------------
    //!  @brief Construct a new Event object DEFAULT
    //----------------------------------------------------------------
    Event(){}


    //----------------------------------------------------------------
    //!  @brief Construct a new Event object FULL
    //----------------------------------------------------------------
    Event(int id, std::string name, std::string info, std::string address, std::string date, std::string time, 
          std::string owner, time_t last_edit_time, bool archived): id_(id), name_(name), info_(info), address_(address), date_(date),
          time_(time), owner_(owner), last_edit_time_(last_edit_time), archived_(archived){}

      
    //----------------------------------------------------------------
    //!  @brief Construct a new Event object by name, date, time
    //!  
    //!  @param name - name, must be not null
    //!  @param date - date (in std::string format)
    //!  @param time - time (in std::string format)
    //----------------------------------------------------------------
    Event(std::string name, std::string date, std::string time): Event(0, name, "", "", date, time, "", std::time(nullptr), false){}


    //----------------------------------------------------------------
    //!  @brief Construct a new Event object from MESSAGE! It is constructor from 
    //!   forwarding messages client-server
    //!  
    //!  @param msg - data (\n separators)
    //----------------------------------------------------------------
    Event(std::string msg);


    //----------------------------------------------------------------
    //!  @brief Type conversion from Event to string (\n - separating)
    //!  
    //!  @return std::string 
    //----------------------------------------------------------------
    operator std::string() const;


    //----------------------------------------------------------------
    //!  @brief Redefinition of "<<" for streams
    //!  
    //!  @param out 
    //!  @param Event 
    //!  @return std::ostream& 
    //----------------------------------------------------------------
    friend std::ostream& operator<< (std::ostream &out, const Event &event);
};



//----------------------------------------------------------------
//!  @brief Class of Database - contain opened database
//----------------------------------------------------------------

class DataBase
{
    sqlite::database* db_;    

  public:

    //----------------------------------------------------------------
    //!  @brief Get the pointer to db object
    //!  
    //!  @return sqlite::database* 
    //----------------------------------------------------------------
    sqlite::database* get_db() const { return db_; };


    //----------------------------------------------------------------
    //!  @brief Construct Singleton object
    //----------------------------------------------------------------

    DataBase(DataBase&) = delete;

    DataBase& operator=(const DataBase&) = delete;

    static DataBase& get_instance()
    {
        static DataBase base;
        return base;
    }


  protected:

    //----------------------------------------------------------------
    //!  @brief Construct a new Data Base object (openning file)
    //----------------------------------------------------------------
    DataBase();


    //----------------------------------------------------------------
    //!  @brief Destroy the Data Base object (closing file)
    //----------------------------------------------------------------
    ~DataBase();

};



//----------------------------------------------------------------
//!  @brief Class of Table of Events
//!
//!  @note Throw errors if something went wrong
//----------------------------------------------------------------

class Table_Events
{
    sqlite::database* db_;

  public:

    //----------------------------------------------------------------
    //!  @brief Create a table event object
    //!  
    //!  @param db will be save to db_ (connect table with db)
    //----------------------------------------------------------------
    void               create_table_event   (sqlite::database* db);


    //----------------------------------------------------------------
    //!  @brief Adding event (all fields, with the exception of id)
    //!  
    //!  @param event 
    //----------------------------------------------------------------
    void               add_event            (const Event& event)       const;


    //----------------------------------------------------------------
    //!  @brief Find event by name in database
    //!  
    //!  @param name 
    //!  @return Event - object Event, wich was found
    //----------------------------------------------------------------
    Event              find_event_by_name   (const std::string& name)  const;


    //----------------------------------------------------------------
    //!  @brief Find event in database by id
    //!  
    //!  @param name 
    //!  @return Event - object Event, wich was found
    //----------------------------------------------------------------
    Event              find_event_by_id   (const int& id)  const;


    //----------------------------------------------------------------
    //!  @brief Remove event by name from database
    //!  
    //!  @param name 
    //! 
    //!  Question: why it should not be const?
    //----------------------------------------------------------------
    void    remove_event_by_name (const std::string& name)  const;


    //----------------------------------------------------------------
    //!  @brief Remove event from database by id
    //!  
    //!  @param name 
    //! 
    //!  Question: why it should not be const?
    //----------------------------------------------------------------
    void    remove_event_by_id (const int& id)  const;


    //----------------------------------------------------------------
    //!  @brief Print in std::cout names of all events in database
    //----------------------------------------------------------------
    void    print_all_events()                              const;


    //----------------------------------------------------------------
    //!  @brief Get vector of all events from database
    //!  
    //!  @return std::vector<Event> 
    //----------------------------------------------------------------
    std::vector<Event> get_all_events()                     const;


    //----------------------------------------------------------------
    //!  @brief Update events info. Searching by name and update it
    //!  
    //!  @param event - event with new info
    //----------------------------------------------------------------
    void    update_event(const Event& event)                const;


    //----------------------------------------------------------------
    //!  @brief Renaming event
    //!  
    //!  @param old_name 
    //!  @param new_name 
    //----------------------------------------------------------------
    void    rename_event(const std::string& old_name, const std::string& new_name) const;


    //----------------------------------------------------------------
    //!  @brief Get the last edit time of events object
    //!  
    //!  @return time_t 
    //----------------------------------------------------------------
    time_t  get_last_edit_time_events()                     const;


    //----------------------------------------------------------------
    //!  @brief Get the events to update object
    //! Returns Event where last_edit_time > time 
    //!  
    //!  @param time 
    //!  @return std::vector<Event> 
    //----------------------------------------------------------------
    std::vector<Event> get_events_to_sync(time_t time)      const;
};




//----------------------------------------------------------------
//!  @brief Class of Table of Events
//!
//!  @note Throw errors if something went wrong
//----------------------------------------------------------------

class Table_Users
{
    sqlite::database* db_;
    
  public:
    
    //----------------------------------------------------------------
    //!  @brief Create a table Users object
    //!  
    //!  @param db - pointer to db will be saved to db_
    //----------------------------------------------------------------
    void               create_table_users(sqlite::database* db);


    //----------------------------------------------------------------
    //!  @brief Adding user to table Users
    //!  
    //!  @param user 
    //----------------------------------------------------------------
    void               add_user(const User& user)                       const;


    //----------------------------------------------------------------
    //!  @brief Find user by login in table Users
    //!  
    //!  @param login - the username we are looking for
    //!  @return User(login, found_password)  on SUCCESS
    //!  @return User("", "")                 if user NOT FOUND
    //----------------------------------------------------------------
    User               find_user_by_login(const std::string& login)     const;


    //----------------------------------------------------------------
    //!  @brief Find user by id in table Users
    //!  
    //!  @param login - the username we are looking for
    //!  @return User(login, found_password)  on SUCCESS
    //!  @return User("", "")                 if user NOT FOUND
    //----------------------------------------------------------------
    User               find_user_by_id(const int& id)     const;


    //----------------------------------------------------------------
    //!  @brief Remove user from Users by login
    //!  
    //!  @param login 
    //----------------------------------------------------------------
    void               remove_user_by_login(const std::string& login)   const;


    //----------------------------------------------------------------
    //!  @brief Remove user from Users by login
    //!  
    //!  @param login 
    //----------------------------------------------------------------
    void               remove_user_by_id(const int& id)   const;
    

    //----------------------------------------------------------------
    //!  @brief Print logins to std::cout 
    //----------------------------------------------------------------
    void               print_all_users()                                const;


    //----------------------------------------------------------------
    //!  @brief Get vector of all users
    //!  
    //!  @return std::vector<User> 
    //----------------------------------------------------------------
    std::vector<User>  get_all_users()                                  const;


    //----------------------------------------------------------------
    //!  @brief Verify User by login, password
    //!  
    //!  @param user 
    //!  @return true  - In case User successfully verified
    //!  @return false - In case User failed verification
    //-----------------------------------------------------------------
    bool               verify_user(const User& user)                    const;


    //----------------------------------------------------------------
    //!  @brief Updating user password (find by name from user struct)
    //!  
    //!  @param user - class with login and new password
    //----------------------------------------------------------------
    void               update_user_password(const User& user)           const;


    //----------------------------------------------------------------
    //!  @brief Updationg user password (fund by user_name)
    //!  
    //!  @param user_name 
    //!  @param new_password 
    //----------------------------------------------------------------
    void update_user_password(const std::string& user_name, const std::string& new_password) const;


    //----------------------------------------------------------------
    //!  @brief Return hash of data (to retain safely passwords in database)
    //!  
    //!  @param data - string to HASH
    //!  @return std::string - HASH
    //----------------------------------------------------------------
    std::string        sha256(const std::string& data)                  const;


    //----------------------------------------------------------------
    //!  @brief Get the last edit time of users object
    //!  
    //!  @return time_t 
    //----------------------------------------------------------------
    time_t             get_last_edit_time_users()                       const;


    //----------------------------------------------------------------
    //!  @brief Get the users to update object
    //! Returns events where last_edit_time > time 
    //!  
    //!  @param time 
    //!  @return std::vector<User> 
    //----------------------------------------------------------------
    std::vector<User>  get_users_to_sync(time_t time)                   const;
    
};




//----------------------------------------------------------------
//!  @brief Class of database for Client;
//!   Contatins only Events Table
//----------------------------------------------------------------

class DataBase_Client: public DataBase, public Table_Events
{

    //----------------------------------------------------------------
    //!  @brief Construct a new DataBase_Client object
    //!   Creting Events database
    //----------------------------------------------------------------

    DataBase_Client()   // Потребовал сделать конструктор DataBase публичным
    {
        create_table_event(DataBase::get_instance().get_db());
    }


  public:

    //----------------------------------------------------------------
    //!  @brief Making Singleton object
    //----------------------------------------------------------------

    DataBase_Client(DataBase_Client&) = delete;

    DataBase_Client& operator=(const DataBase_Client&) = delete;

    static DataBase_Client& get_instance()
    {
        static DataBase_Client base;
        return base;
    }
    
};



//----------------------------------------------------------------
//!  @brief Class of database for Server;
//!   Contatins Events Table and Users Table
//----------------------------------------------------------------

class DataBase_Server: public DataBase, public Table_Events, public Table_Users
{

    //----------------------------------------------------------------
    //!  @brief Construct a new DataBase_Client object
    //!   Creating Events table
    //!   Creating Users table
    //----------------------------------------------------------------

    DataBase_Server()   // Потребовал сделать конструктор DataBase публичным
    {
        create_table_event(DataBase::get_instance().get_db());
        create_table_users(DataBase::get_instance().get_db());
    }


  public:

    //----------------------------------------------------------------
    //!  @brief Making Singleton object
    //----------------------------------------------------------------

    DataBase_Server(DataBase_Server&) = delete;

    DataBase_Server& operator=(const DataBase_Server&) = delete;

    static DataBase_Server& get_instance()
    {
        static DataBase_Server base;
        return base;
    }
    
};
