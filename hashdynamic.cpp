#include <iostream>
#include <string>
#include "bucket.h"
#include "directory.h"

using namespace std;

void menu() {
    cout << "--------------------" << endl;
    cout << "Opciones disponibles:" << endl;
    cout << "(0) insert" << endl;
    cout << "(1) update" << endl;
    cout << "(2) search" << endl;
    cout << "(3) display" << endl;
    cout << "(4) exit" << endl;
    cout << "--------------------" << endl;
}

int main() {
    bool show_duplicate_buckets;
    int bucket_size;
    int key, mode;
    int choice;
    long value;


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

        long res;
        switch (choice) {
            case 0:
                cout << "Ingrese key (int): ";
                cin >> key ;
                cout << "Ingrese value (string): ";
                cin >> value;
                cout << endl;
                res = d.insert(key, value, 0);
                cout << "RES: "<<res << endl;
                break;
            case 1:
                cout << "Ingrese key (int): ";
                cin >> key;
                cout << "Ingrese nuevo value (string): ";
                cin >> value;
                cout << endl;
                d.update(key, value);
                break;
            case 2:
                cout << "Ingrese key (int): ";
                cin >> key;
                cout << endl;
                res = d.search(key);
                cout << "RES: "<<res << endl;
                break;
            case 3:
                cout << endl;
                d.display(show_duplicate_buckets);
                break;
            default:
                cout << "opcion no válida\n";
                break;
        }
    } while(choice != 4);

    return 0;
}
