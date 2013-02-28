#include "leveldb/include/leveldb/db.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>

using namespace leveldb;
using namespace std;

int main( int argc, char** argv ) {
    DB *db;
    Options options;
    options.create_if_missing = true;

    // Open the database
    if ( ! DB::Open(options, "/tmp/testdb", &db).ok() )
    {
        cout << "Failed to open db" << endl;
        exit(1);
    } else
    {
        cout << "Database is open" << endl;
    }

    string keys[] = {"one", "two", "three", "four", "five"}, values[] = { "1", "2", "3", "4", "5" };

    // Populate the database
    for ( int i = 0; i < 4; ++i )
    {
        if ( ! db->Put(WriteOptions(), keys[i], values[i]).ok())
        {
            cout << "Unable to insert " << keys[i] << "\t|\t" << values[i] << endl;
            exit(1);
        } else
        {
            cout << "Inserting " << keys[i] << "\t|\t" << values[i] << endl;
        }
    }

    string value;
    for ( int i = 3; i < 5; ++i )
    {
        // Reading a value
        if ( db->Get(ReadOptions(), keys[i], &value).ok() )
        {
            cout << "Reading key: " << keys[i] << " | " << value << endl;
            continue;
        } else
        {
            db->Put( WriteOptions(), keys[i], values[i] );
        }
    }

    // Iterate over the database
    cout << "The whole database is: " << endl;
    Iterator * it = db->NewIterator( ReadOptions() );
    for ( it->SeekToFirst(); it->Valid() && it->status().ok(); it->Next() )
    {
        cout << "Key: " << it->key().ToString() << "\t|\tValue: " << it->value().ToString() << endl;
    }
    delete it;

    // Delete all the database records
    it = db->NewIterator( ReadOptions() );
    for ( it->SeekToFirst(); it->Valid() && it->status().ok(); it->Next() )
    {
        if ( ! db->Delete(WriteOptions(), it->key().ToString() ).ok() )
        {
            cout << "Unable to delete " << it->key().ToString() << " | " << it->value().ToString() << endl;
            exit(1);
        } else
        {
            cout << "Deleting key: " << it->key().ToString() << endl;
        }
    }

    cout << "The whole database is: " << endl;
    it = db->NewIterator( ReadOptions() );
    for ( it->SeekToFirst(); it->Valid() && it->status().ok(); it->Next() )
    {
        cout << "Key: " << it->key().ToString() << "\t|\tValue: " << it->value().ToString() << endl;
    }
    delete it;

    // Close the database
    cout << "Closing the database" << endl;
    delete db;

    return 0;
}
