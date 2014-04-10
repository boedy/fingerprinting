#ifndef DATABASE_H
#define DATABASE_H

#include <map>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/any.hpp>
#include <string>
#include "fstream"
#include <boost/filesystem.hpp>

using namespace std;

template <class T>
class Database
{
public:
    Database(string name);
    T get(string);
    bool insert(string key, T value);
    bool update(string key, T value);
    void readFromDisk();
    void writeToDisk();
    template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            ar & hashtable;
        }
private:
    multimap<string, T> hashtable;
    string database_name;
};

template <class T>
Database<T>::Database(string name)
{
    database_name = "storage/"+name+".db";
}

template <class T>
T Database<T>::get(string key){
    typename multimap<string, T>::iterator needle = hashtable.find(key);
    if(needle == hashtable.end()){
        cout << "Not found" << endl;
    }else{
        return needle->second;
    }
}

template <class T>
bool Database<T>::insert(string key, T value){
    hashtable.insert(make_pair(key, value));
    return true;
}

template <class T>
bool Database<T>::update(string key, T value){
    //hashtable[key] = value;
    return true;
}

template <class T>
void Database<T>::readFromDisk(){
    ifstream ifs(database_name);
    if(boost::filesystem3::exists(database_name)){
        {
            boost::archive::binary_iarchive ia(ifs);
            ia >> hashtable;
        }
    }
}

template <class T>
void Database<T>::writeToDisk(){
    ofstream ofs(database_name);
    {
        boost::archive::binary_oarchive oa(ofs);
        oa << hashtable;
    }
}

#endif // DATABASE_H
