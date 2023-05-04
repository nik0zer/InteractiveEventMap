//----------------------------------------------------------------
//  This part realises lib of classes which interacts with SQL databases
//  Designed for project Interactive Map (group project for 4sem in MIPT)
//
//  @copyright AlexZ
//----------------------------------------------------------------


#include "sql_tools.h"


// ---------------------------------------------------------------------------------------------------------------------
// Constructions for communicate to server
// 
// Operator std::string convert object to string, parsing by \n
// Construcotr of msg - construct the object from msg by parsing msg by \n
// ---------------------------------------------------------------------------------------------------------------------

#ifndef NETWORK_CONNECTION_SERVER_H
    void handler1(ReadData read_data)
    {
        DataBase::get_instance().parse_cmd(read_data.data_name(), read_data.data_str());
    }
#endif

Person::operator std::string() const 
{
    std::string str = std::to_string(id_) + "\n";
    str += login_ + "\n";
    str += password_ + "\n";
    str += std::to_string(last_edit_time_) + "\n";
    return str;
}



Person::Person(std::string msg)
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



Event::operator std::string() const 
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



Event::Event(std::string msg)
{
    std::vector<std::string> data_vector;
    std::string temp;
    std::stringstream input_stream(msg);

    for (int i = 0; i < 8; i++)
    {
        std::getline(input_stream, temp);
        data_vector.push_back(temp);
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
    out << person.id_ << "    " << person.login_ << "    " << person.password_ << "    " << person.last_edit_time_;

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
    out << event.id_ << "    " << event.name_ << "    " << event.info_ << "    " << event.address_
                     << "    " << event.date_ << "    " << event.time_ << "    " << event.owner_ << "    " << event.last_edit_time_;

    return out;
}



// ---------------------------------------------------------------------------------------------------------------------
// Initial part of DataBase
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
        spdlog::error("{}: {}", error_msg, sqlite3_errmsg(DB));

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
    // fill_reserved_persons_id();
    // fill_reserved_events_id();
}



DataBase::~DataBase()
{
    if (sqlite3_close(ptr_) == SQLITE_OK)
    {
        spdlog::info("Database closed");
    }
    else
    {
        spdlog::error("Can't close Database!");
    }
}



void DataBase::fill_reserved_persons_id()
{
    std::string sql_cmd = "SELECT * FROM CREDS;";
    execute_sql(sql_cmd, "CREDS");

    for (auto& item : persons_vector_)
    {
        reserved_persons_id_.insert(item.get_id());
    }
}



void DataBase::fill_reserved_events_id()
{
    std::string sql_cmd = "SELECT * FROM EVENTS;";
    execute_sql(sql_cmd, "EVENTS");

    for (auto& item : events_vector_)
    {
        reserved_events_id_.insert(item.get_id());
    }
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



// ---------------------------------------------------------------------------------------------------------------------
// Server interaction
// ---------------------------------------------------------------------------------------------------------------------



void DataBase::update_database()
{
    time_t persons_actual_time = get_last_edit_time_persons();
    time_t events_actual_time  = get_last_edit_time_events();

    // send_to_server("send_me_actual_persons", "time")
    // send_to_server("send_me_actual_events", "time")


    #ifndef NETWORK_CONNECTION_SERVER_H
        connection->thread_cycle_read(handler1);
        connection->send_data("send_me_actual_persons", "0");
        connection->send_data("send_me_actual_events", "0");
    #endif

    // call parse_cmd после получения ответа    
}



time_t DataBase::get_last_edit_time_persons()
{
    std::string sql_cmd = "SELECT * FROM CREDS WHERE LAST_EDIT_TIME = (MAX(LAST_EDIT_TIME) FROM CREDS);";
    execute_sql(sql_cmd, "CREDS");

    if (events_vector_.size() == 0)
    {
        spdlog::warn("No persons found for get last edit time");
        return 0;
    }
    
    return events_vector_[0].get_last_edit_time();      // Не эстетично
}



time_t DataBase::get_last_edit_time_events()
{
    std::string sql_cmd = "SELECT * FROM EVENTS WHERE LAST_EDIT_TIME = (MAX(LAST_EDIT_TIME) FROM EVENTS);";
    execute_sql(sql_cmd, "EVENTS");

    if (events_vector_.size() == 0)
    {
        spdlog::critical("No events found for get last edit time");
        return 0;
    }

    return events_vector_[0].get_last_edit_time();
}



int DataBase::parse_cmd(std::string cmd, std::string data)
{
    if (cmd == "update_event")
    {
        Event new_event(data);
        Event old_event = get_event(new_event.get_name());

        DataBase::get_instance().remove_event(old_event);
        DataBase::get_instance().add_event(new_event);
        return 0;
    }
    else if (cmd == "update_person")
    {
        Person new_person(data);

        DataBase::get_instance().remove_person(new_person);
        DataBase::get_instance().add_person(new_person);
        return 0;
    }
    else
    {
        spdlog::error("Unknown cmd to parse from server = '{}'", cmd);
        return 1;
    }
    
}



std::vector<Person> DataBase::get_persons_to_sync(time_t time)
{
    std::string sql_cmd = fmt::format("SELECT * FROM CREDS WHERE LAST_EDIT_TIME >= {};", time);
    execute_sql(sql_cmd, "CREDS");

    return persons_vector_;
}



std::vector<Event> DataBase::get_events_to_sync(time_t time)
{
    std::string sql_cmd = fmt::format("SELECT * FROM EVENTS WHERE LAST_EDIT_TIME >= {};", time);
    spdlog::info("Cmd = {}", sql_cmd);
    execute_sql(sql_cmd, "EVENTS");

    spdlog::info("Size of vector = {}", events_vector_.size());

    return events_vector_;
}



// ---------------------------------------------------------------------------------------------------------------------
// SQL part
// ---------------------------------------------------------------------------------------------------------------------



void DataBase::execute_sql(const std::string& sql_cmd, const std::string& table)
{
    // Reserve pointer for collect errors of sql commands
    char* messaggeError;
    int res = 0;


    if (table == "CREDS")
    {
        persons_vector_.clear();
        sqlite3_exec(ptr_, sql_cmd.c_str(), callback_person, 0, &messaggeError);
    }
    else if (table == "EVENTS")
    {
        events_vector_.clear();
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



//----------------------------------------------------------------
//!  Fill member "persons_vector_" with returned info from DB
//!
//!  @note          Depends of CREDS structure!!!
//!  @copyright     AlexZ
//----------------------------------------------------------------

int DataBase::callback_person(void* data, int argc, char** argv, char** azColName) 
{
    spdlog::info("Called callback of PERSON");

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
    spdlog::info("Called callback of EVENT");
    
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



// ---------------------------------------------------------------------------------------------------------------------
// Base person options with DataBase
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
    DataBase::get_instance().fill_reserved_persons_id();

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
// Base event options with DataBase
// ---------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------
//!  Add event to CREDS database
//!
//!  @param  [in]   person - person to search in DB
//!  @note          Depends of EVENTS structure!!!
//!  @copyright     AlexZ
//----------------------------------------------------------------

void DataBase::add_event(Event& event)
{
    DataBase::get_instance().fill_reserved_events_id();

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



//----------------------------------------------------------------
//!  Remove person to CREDS database
//!
//!  @param  [in]   person - person to search in DB
//!  @note          Depends of CREDS structure!!!
//!  @copyright     AlexZ
//----------------------------------------------------------------

void DataBase::remove_event(Event& event)
{
    std::string sql_cmd = fmt::format("DELETE FROM EVENTS WHERE NAME = '{}';", 
                                        event.get_name());

    execute_sql(sql_cmd, "EVENTS");
    // need remove id

    spdlog::info("Event {} removed successfully", event.get_name());
}



// ---------------------------------------------------------------------------------------------------------------------
// Logic
// ---------------------------------------------------------------------------------------------------------------------



bool DataBase::person_verify(Person& person)
{
    std::string sql_cmd = fmt::format("SELECT * FROM CREDS WHERE LOGIN='{}' AND PASSWORD='{}';",
                                        person.get_login(), person.get_password());

    execute_sql(sql_cmd, "CREDS");

    if (persons_vector_.size() == 0) return false;
    if (persons_vector_.size() == 1) return true;

    spdlog::critical("Found more than 1 person in verify func (person login = {})", person.get_login());

    return true;
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
    update_database();

    std::string sql_cmd = "SELECT * FROM EVENTS;";
    execute_sql(sql_cmd, "EVENTS");

    return events_vector_;
}



Event DataBase::get_event(Event& event)
{
    std::string sql_cmd = fmt::format("SELECT * FROM EVENTS WHERE NAME='{}' AND DATE='{}' AND TIME='{}';",
                                        event.get_name(),
                                        event.get_date(),
                                        event.get_time());

    execute_sql(sql_cmd, "EVENTS");
    if (events_vector_.size() == 0)
    {
        return Event("");
    }
    if (events_vector_.size() == 1)
    {
        return events_vector_[0];
    }
    
    spdlog::critical("There are multyple found of event (name = {})", event.get_name());

    return Event("");
}



Event DataBase::get_event(std::string name)
{
    std::string sql_cmd = fmt::format("SELECT * FROM EVENTS WHERE NAME='{}';", name);

    execute_sql(sql_cmd, "EVENTS");
    if (events_vector_.size() == 0)
    {
        spdlog::warn("Event not found (name = '{}')", name);
        return Event("");
    }
    if (events_vector_.size() == 1)
    {
        return events_vector_[0];
    }
    
    spdlog::critical("There are multyple found of event (name = {})", name);

    return Event("");
}



void DataBase::rename_event(std::string old_name, std::string new_name)
{
    std::string sql_cmd = fmt::format("UPDATE EVENTS SET NAME='{}' WHERE NAME='{}';",
                                        new_name,
                                        old_name);

    execute_sql(sql_cmd, "EVENTS");
}



void DataBase::print_all_events()
{
    std::cout << "All events:\n";

    for (const auto& item : get_all_events())
    {
        std::cout << item << std::endl;
    }
}

