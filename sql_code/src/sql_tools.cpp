//----------------------------------------------------------------
//  This part realises lib of classes which interacts with SQL databases
//  Designed for project Interactive Map (group project for 4sem in MIPT)
//
//  @copyright AlexZ
//----------------------------------------------------------------


#include "sql_tools.h"


// ---------------------------------------------------------------------------------------------------------------------
// Realisation for class "DataBase"
// ---------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------
//!  Open database for subsequent interactions
//
//!  @param  [in]   path - path to Data base
//!  @return        sqlite* DB - pointer for openned database
//!  @note          In case of error in open DB, throw runtime error
//!  @copyright     AlexZ
//----------------------------------------------------------------

sqlite3* DataBase::open_db(const std::string path) 
{
    sqlite3* DB = nullptr; 
  
    if (sqlite3_open(path.c_str(), &DB)) 
    {
        std::string error_msg = "Can't open Data Base";
        spdlog::error("{}: {}", error_msg, sqlite3_errmsg(DB));

        throw std::runtime_error(error_msg);
    }

    spdlog::info("{:30} ... {}", "Openning Data Base", "OK");

    return DB;
}



//----------------------------------------------------------------
//!  Creating 2 tables:
//!  - "CREDS"  - users logins and passwords
//!  - "EVENTS" - info about events
//
//!  @param  [in]   DB - pointer on DB
//!  @return        None
//!  @note          In case of error in open DB, throw runtime error
//!  @note          Depends of CREDS structure!!!
//!  @copyright     AlexZ
//----------------------------------------------------------------

void DataBase::create_tables(sqlite3* DB) 
{
    if (!DB)
    {
        std::string error_msg = "DB doesn't exists, can't create tables!";
        spdlog::critical("{}: {}", error_msg, sqlite3_errmsg(DB));

        throw std::runtime_error(error_msg);
    }

    

    // CMD to create table for autetification
    std::string sql_cmd = "CREATE TABLE IF NOT EXISTS CREDS("
                          "ID INT PRIMARY KEY     NOT NULL, "
                          "LOGIN          TEXT    NOT NULL  UNIQUE, "
                          "PASSWORD       TEXT    NOT NULL, "
                          "LAST_EDIT_TIME INT     NOT NULL);";

    execute_sql(sql_cmd, "");


    // CMD to create table for events info
    sql_cmd = "CREATE TABLE IF NOT EXISTS EVENTS("
                      "ID INT PRIMARY KEY     NOT NULL, "
                      "NAME           TEXT    NOT NULL, "
                      "INFO           TEXT            , "
                      "ADDRESS        TEXT            , "
                      "DATE           TEXT            , "
                      "TIME           TEXT            , "
                      "OWNER          TEXT            , "
                      "LAST_EDIT_TIME INT     NOT NULL);";

    execute_sql(sql_cmd, "");


    spdlog::info("Tables created");
}



//----------------------------------------------------------------
//!  Constructor for class DataBase consist of
//!  - openning DB
//!  - creating tables in DB (CREDS and EVENTS)
//!
//!  @copyright     AlexZ
//----------------------------------------------------------------

DataBase::DataBase()
{
    ptr_ = open_db("main.db");
    create_tables(ptr_);
    // update_d
}



//----------------------------------------------------------------
//!  Method to get instance of singleton object
//!
//!  @return        DataBase& - link for object of class
//!  @copyright     AlexZ
//----------------------------------------------------------------

DataBase& DataBase::get_instance()
{
    static DataBase base;

    return base;
}



void DataBase::execute_sql(const std::string& sql_cmd, const std::string& table)
{
    // Reserve pointer for collect errors of sql commands
    char* messaggeError;
    int res = 0;


    if (table == "CREDS")
    {
        sqlite3_exec(ptr_, sql_cmd.c_str(), callback_person, 0, &messaggeError);
    }
    else if (table == "EVENTS")
    {
        sqlite3_exec(ptr_, sql_cmd.c_str(), callback_event, 0, &messaggeError);
    }
    else
    {
        sqlite3_exec(ptr_, sql_cmd.c_str(), nullptr, 0, &messaggeError);
    }


    if (res != SQLITE_OK) 
    {
        std::string error_msg = sql_cmd;
        spdlog::warn("Error in CMD {} : {}", error_msg, messaggeError ? messaggeError : "");
        sqlite3_free(messaggeError);
    }
}




// ---------------------------------------------------------------------------------------------------------------------
// Person part of interaction with DataBase
// ---------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------
//!  Add person to CREDS database
//!
//!  @param  [in]   person - person to search in DB
//!  @note          Depends of CREDS structure!!!
//!  @copyright     AlexZ
//----------------------------------------------------------------

void DataBase::add_person(Person& person)
{
    if (person_exists(person))
    {
        spdlog::warn("Person already exists ({})", person.login_);
        return;
    }

    std::string sql_cmd = fmt::format("INSERT INTO CREDS VALUES({}, '{}', '{}', {});", 
                            std::to_string(get_next_id(reserved_persons_id_)), 
                            person.login_, 
                            person.password_, 
                            std::to_string(std::time(nullptr)));

    execute_sql(sql_cmd, "CREDS");

    spdlog::info("{:30} ... {}", "Adding person '" + person.login_ + "'", "OK");
}



//----------------------------------------------------------------
//!  Remove person to CREDS database
//!
//!  @param  [in]   person - person to search in DB
//!  @note          Depends of CREDS structure!!!
//!  @copyright     AlexZ
//----------------------------------------------------------------

void DataBase::remove_person(Person& person)
{
    if (!person_exists(person))     // The correct key will be setted for person in func
    {
        spdlog::warn("Person '{}' doesn't exist", person.get_login());
        return;
    }

    std::string sql_cmd = fmt::format("DELETE FROM CREDS WHERE LOGIN = '{}';", person.get_login());
    execute_sql(sql_cmd, "CREDS");
    remove_id(reserved_persons_id_, person.get_id());

    spdlog::info("Person {} removed successfully", person.get_login());
}



//----------------------------------------------------------------
//!  DataBase class have field reserved_persons_id_ vector. So we need the
//!  next id for adding new person.
//!
//!  @return        int next_id
//!  @copyright     AlexZ
//----------------------------------------------------------------

int DataBase::get_next_id(std::set<int>& id_set)
{
    int i;
    for (i = 1; id_set.size() != 0 && id_set.find(i) != id_set.end(); i++) ;
    id_set.insert(i);

    return i;
}



void DataBase::remove_id(std::set<int>& id_set, const int& i)
{
    if (id_set.find(i) == id_set.end())
    {
        spdlog::warn("Invalid id={} to remove from reserved_person_id_");
        return;
    }
    id_set.erase(i);   
    spdlog::info("Reserved id={} removed", i);
}



//----------------------------------------------------------------
//!  Fill member "persons_vector_" with returned info from DB
//!
//!  @note          Depends of CREDS structure!!!
//!  @copyright     AlexZ
//----------------------------------------------------------------

int DataBase::callback_person(void* data, int argc, char** argv, char** azColName) 
{
    if (data)
    {
        spdlog::warn("Received data: {}", (const char*)data ? (const char*)data : "");
        return 0;
    }

    Person temp(atoi(argv[0]), argv[1], argv[2], atol(argv[3]));

    DataBase::get_instance().persons_vector_.push_back(temp);

    return 0;
}



//----------------------------------------------------------------
//!  Fill member "events_vector_" with returned info from DB
//!
//!  @note          Depends of CREDS structure!!!
//!  @copyright     AlexZ
//----------------------------------------------------------------

int DataBase::callback_event(void* data, int argc, char** argv, char** azColName) 
{
    if (data)
    {
        spdlog::warn("Reveived data: {}", (const char*)data ? (const char*)data : "");
        return 0;
    }

    Event temp(atoi(argv[0]), argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], atol(argv[7]));

    DataBase::get_instance().events_vector_.push_back(temp);

    return 0;
}



//----------------------------------------------------------------
//!  Return vector of all persons from DB to person_vector_ member of class
//!
//!  @copyright     AlexZ
//----------------------------------------------------------------

std::vector<Person> DataBase::get_all_persons()
{
    std::string sql_cmd = "SELECT * FROM CREDS;";
    execute_sql(sql_cmd, "CREDS");

    return persons_vector_;
}



std::vector<Event> DataBase::get_all_events()
{
    std::string sql_cmd = "SELECT * FROM EVENTS;";
    execute_sql(sql_cmd, "EVENTS");

    return events_vector_;
}


// Event DataBase::get_event(Event& event)
// {
//     std::string sql_cmd = "SELECT * FROM EVENTS WH"
// }


//----------------------------------------------------------------
//!  Check if person exist in database by login
//!
//!  @return        true (if exists), false (if not)
//!  @note          If person exists, method set person.id_ to actual in database.
//!  @copyright     AlexZ
//----------------------------------------------------------------

bool DataBase::person_exists(Person& person)
{   
    std::string sql_cmd = fmt::format("SELECT * FROM CREDS WHERE LOGIN='{}';", person.login_);

    persons_vector_.clear();
    execute_sql(sql_cmd, "CREDS");

    if (persons_vector_.size() > 0)
    {
        person.id_ = persons_vector_.begin()->id_;
        return true;
    }

    else
    {
        return false;
    }
}


// ---------------------------------------------------------------------------------------------------------------------
// Person part of interaction with DataBase
// ---------------------------------------------------------------------------------------------------------------------


// //----------------------------------------------------------------
// //!  Add event to CREDS database
// //!
// //!  @param  [in]   person - person to search in DB
// //!  @note          Depends of EVENTS structure!!!
// //!  @copyright     AlexZ
// //----------------------------------------------------------------

void DataBase::add_event(Event& event)
{
    std::string sql_cmd = fmt::format("INSERT INTO EVENTS VALUES({}, '{}', '{}', '{}', '{}', '{}', '{}', {});", 
                                        std::to_string(get_next_id(reserved_events_id_)), 
                                        event.get_name(),
                                        event.get_info(),
                                        event.get_address(),
                                        event.get_date(),
                                        event.get_time(),
                                        event.get_owner(),
                                        std::to_string(std::time(nullptr)));

    execute_sql(sql_cmd, "EVENTS");

    spdlog::info("{:30} ... {}", "Adding event '" + event.get_name() + "'", "OK");
}


// //----------------------------------------------------------------
// //!  Remove person to CREDS database
// //!
// //!  @param  [in]   person - person to search in DB
// //!  @note          Depends of CREDS structure!!!
// //!  @copyright     AlexZ
// //----------------------------------------------------------------

void DataBase::remove_event(Event& event)
{
    std::string sql_cmd = fmt::format("DELETE FROM EVENTS WHERE NAME = '{}' AND INFO = '{}';", 
                                        event.get_name(),
                                        event.get_info());

    execute_sql(sql_cmd, "EVENTS");

    spdlog::info("Person {} removed successfully", event.get_name());
}



// ---------------------------------------------------------------------------------------------------------------------
// std::cout operators <<
// ---------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------
//!  Redefinition of operator << for class Person
//!
//!  @note          Depends of CREDS structure!!!
//!  @copyright     AlexZ
//----------------------------------------------------------------

std::ostream& operator<< (std::ostream &out, const Person &person)
{
    out << person.id_ << "    " << person.login_ << "    " << person.password_ << std::endl;

    return out;
}



//----------------------------------------------------------------
//!  Redefinition of operator << for class Event
//!
//!  @note          Depends of CREDS structure!!!
//!  @copyright     AlexZ
//----------------------------------------------------------------

std::ostream& operator<< (std::ostream &out, const Event &event)
{
    out << event.id_ << "    " << event.name_ << "    " << event.address_ << "    " << event.info_;

    return out;
}

















// int test_db(sqlite3* DB) 
// {
//     if (!DB) 
//     {
//         std::cout << "Error! Null DB in " <<  __func__ << std::endl;
//         return -1;
//     }


//     std::string query = "SELECT * FROM CREDS;";
//     std::string sql   = "INSERT INTO CREDS VALUES(1, 'STEVE', 'STEVE');"
//                         "INSERT INTO CREDS VALUES(2, 'ATEVE', 'STEVE');";

  
//     std::cout << "STATE OF CREDS BEFORE INSERT" << std::endl;
//     sqlite3_exec(DB, query.c_str(), DataBase::callback_person, NULL, NULL);

  
//     char* messaggeError;
//     int exit;
    

//     exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);

//     if (exit != SQLITE_OK) 
//     {
//         std::cerr << "Error Insert" << std::endl;
//         sqlite3_free(messaggeError);
//         exit = 0;
//     }

//     else
//     {
//         std::cout << "Records created Successfully!" << std::endl;
//     }
  

//     std::cout << "\nSTATE OF TABLE AFTER INSERT" << std::endl;
//     sqlite3_exec(DB, query.c_str(), DataBase::callback_person, NULL, NULL);
  

//     sql = "DELETE FROM CREDS WHERE ID = 2;";
//     exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);


//     if (exit != SQLITE_OK) 
//     {
//         std::cerr << "Error DELETE" << std::endl;
//         sqlite3_free(messaggeError);
//     }

//     else
//     {
//         std::cout << "Record deleted Successfully!" << std::endl;
//     }
  

//     std::cout << "STATE OF TABLE AFTER DELETE OF ELEMENT" << std::endl;
//     sqlite3_exec(DB, query.c_str(), DataBase::callback_person, NULL, NULL);


//     std::cout << "End of test_db\n";
//     return 0;
// }



// void main_test() 
// {
//     sqlite3* DB;
//     const std::string path_to_database = "example.db";

//     std::ofstream outfile(path_to_database);
//     std::remove(path_to_database.c_str());

//     open_db(path_to_database);
//     create_tables(DB);

//     // if (test_db(DB) != 0) {std::cout << "Error in test_db\n"; return;}

//     sqlite3_close(DB);

//     std::remove(path_to_database.c_str());
// }

