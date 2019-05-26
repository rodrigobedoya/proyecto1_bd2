#include <iostream>
#include <string>

using namespace std;

#include "bucket.h"
#include "directory.h"

void menu() {
    cout << "--------------------" << endl;
    cout << "Opciones disponibles:" << endl;
    cout << "insert <key> <value>     (key: entero, value: string)" << endl;
    cout << "update <key> <new value>" << endl;
    cout << "search <key>" << endl;
    cout << "display" << endl;
    cout << "exit" << endl;
    cout << "--------------------" << endl;
}

int main() {
    bool show_messages, show_duplicate_buckets;
    int bucket_size, initial_global_depth;
    int key, mode;
    string choice, value;

    // Set show_messages to 0 when taking input using file redirection
    show_messages = 1;

    // Set show_duplicate_buckets to 1 to see all pointers instead of unique ones
    show_duplicate_buckets = 1;

    if(show_messages) 
        cout<<"Tamaño del bucket: ";
    cin>>bucket_size;
    if(show_messages) 
        cout<<"global depth inicial: "; 
    cin>>initial_global_depth;

    Directory d(initial_global_depth,bucket_size);
    cout<<endl<<"Menú"<<endl;

    if(show_messages)
        menu();

    do
    {
        cout<<endl;
        if(show_messages) { cout<<">>> "; }
        cin>>choice;
        if(choice=="insert") {
            cin>>key>>value;
            if(show_messages) { cout<<endl; }
            d.insert(key,value,0);
        } else if(choice=="update") {
            cin>>key>>value;
            if(show_messages) { cout<<endl; }
            d.update(key,value);
        } else if(choice=="search") {
            cin>>key;
            if(show_messages) { cout<<endl; }
            d.search(key);
        } else if(choice=="display") {
            if(show_messages) { cout<<endl; }
            d.display(show_duplicate_buckets);
        }
    } while(choice!="exit");

    return 0;
}