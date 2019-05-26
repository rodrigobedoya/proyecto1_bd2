#include <iostream>
#include <string>
#include "bucket.h"
#include "directory.h"

using namespace std;

void menu() {
    cout << "--------------------" << endl;
    cout << "Opciones disponibles:" << endl;
    cout << "(0) insert <key> <value>   (key: int, value: string)" << endl;
    cout << "(1) update <key> <new value>" << endl;
    cout << "(2) search <key>" << endl;
    cout << "(3) display" << endl;
    cout << "(4) exit" << endl;
    cout << "--------------------" << endl;
}

int main() {
    bool show_duplicate_buckets;
    int bucket_size;
    int key, mode;
    int choice;
    string value;


    // Set show_duplicate_buckets to 1 to see all pointers instead of unique ones
    show_duplicate_buckets = 1;

    cout<<"Tamaño del bucket: ";
    cin>>bucket_size;

    Directory d(1, bucket_size);
    cout<<endl<<"Menú: "<<endl;

    menu();

    do {
        cout<<endl;
        cout<<"Elija opción: ";
        cin>>choice;
        
        switch (choice) {
            case 0:
                cin >> key >> value;
                cout << endl;
                d.insert(key, value, 0);
                break;
            case 1:
                cin >> key >> value;
                cout << endl;
                d.update(key, value);
                break;
            case 2:
                cin >> key;
                cout << endl;
                d.search(key);
                break;
            case 3:
                cout << endl;
                d.display(show_duplicate_buckets);
                break;
        }
    } while(choice != 4);

    return 0;
}
