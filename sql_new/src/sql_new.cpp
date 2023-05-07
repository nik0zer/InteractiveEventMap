//----------------------------------------------------------------
//  This part realises lib of classes which interacts with SQL databases
//  Designed for project Interactive Map (group project for 4sem in MIPT)
//
//  @copyright AlexZ
//----------------------------------------------------------------


#include "sql_new.h"


// ---------------------------------------------------------------------------------------------------------------------
// Constructions for communicate to server
// 
// Operator std::string convert object to string, parsing by \n
// Construcotr of msg - construct the object from msg by parsing msg by \n
// ---------------------------------------------------------------------------------------------------------------------



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
// Initialize DataBase
// ---------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------
//!  Constructor for class DataBase consist of
//!  - openning DB
//!  - creating tables in DB (CREDS and EVENTS)
//!
//!  @copyright     AlexZ
//----------------------------------------------------------------

DataBase::DataBase()
{
    db_ = new sqlite::database("main.db");
}



DataBase::~DataBase()
{
    try
    {
        delete db_;
        spdlog::info("Database closed");
    }
    catch(std::exception& e)
    {
        spdlog::error("Can't close Database!");
    }
}



// ---------------------------------------------------------------------------------------------------------------------
// Tables
// ---------------------------------------------------------------------------------------------------------------------



void Table_Events::create_table_event(sqlite::database* db)
{
    db_ = db;
    *db << "CREATE TABLE IF NOT EXISTS EVENTS("
                        "ID INT PRIMARY KEY NOT NULL, "
                        "NAME                      TEXT    NOT NULL, "
                        "INFO                      TEXT            , "
                        "ADDRESS                   TEXT            , "
                        "DATE                      TEXT            , "
                        "TIME                      TEXT            , "
                        "OWNER                     TEXT            , "
                        "LAST_EDIT_TIME            INT     NOT NULL);";


    spdlog::info("Table EVENTS created");
}



void Table_Events::print_all_events()
{
    std::cout << "Print all events:\n";
    
    *db_ << "SELECT NAME FROM EVENTS;" >> [&](std::string name) 
    {
        std::cout << name << std::endl;
    };
}



void Table_Events::add_event(const Event& event)
{
    // *db_ << u"INSERT into EVENTS (NAME,INFO,ADDRESS,DATE,TIME,OWNER,LAST_EDIT_TIME) values (?,?,?,?,?,?,?);"
    if (!db_) spdlog::critical("Zero db");
    *db_ << u"INSERT into EVENTS (NAME) values (?)"
        << u"hello";
    //  << event.get_name()
    //  << event.get_info()
    //  << event.get_address()
    //  << event.get_date()
    //  << event.get_time()
    //  << event.get_owner()
    //  << event.get_last_edit_time();
}