#ifndef BUCKET_H
#define BUCKET_H

#include <iostream>
#include <map>
using namespace std;

class Bucket {
    int depth, size; // profundidad y tamaño del bucket
    map<int, long> values; // mapa del key y values

    public:
        Bucket(int depth, int size) : depth(depth), size(size) {}; //Constructor del bucket
        
        long search(int key) { // funcion search 
            auto it = values.find(key);
            if (it != values.end()) { // si la key esta en el bucket
                cout << "Valor = " << it->second << endl;
                return it->second;
            } else { // caso contrario
                cout << "Key no existe\n";
                return -1;
            }
        }

        int insert(int key,long value) { // funcion insert
            auto it = values.find(key);
            if (it != values.end()) // si key existe
                return -1;
            if (isFull()) // si el bucket está lleno
                return 0;
            values[key] = value;
            return 1;
        }
        
        bool update(int key, long value) {
            auto it = values.find(key);
            if (it != values.end()) { // si key existe
                values[key] = value;
                cout << "Valor actualizado\n";
                return true;
            } else { // caso contrario
                cout << "No se puede actualizar, key no existe\n";
                return false;
            }
        }
        
        void display() { // funcion display
            for (auto it = values.begin(); it != values.end(); it++) // imprime las keys en un bucket
                cout << it->first << " ";
            cout << endl;
        }
        
        map<int, long> copy() { // funcion copy
            map<int, long> temp(values.begin(), values.end()); // crea una copia del mapa values
            return temp;
        }

        bool isFull() { // funcion isFull
            if (values.size() == size) // si el bucket esta lleno retorna true
                return true;
            return false; // caso contrario 
        }
        
        bool isEmpty() { // funcion isEmpty
            if (values.size() == 0) // si el bucket no tiene keys retorna true
                return true;
            return false; // caso contrario
        }
        
        int getDepth() { return depth; } // retorna profundidad de bucket actual
        
        int incrementDepth() { // funcion incrementDepth
            depth++; // aumenta profundidad
            return depth;
        }
        
        int decrementDepth() { // funcion decrementDepth
            depth--; // disminuye profundidad
            return depth;
        }
        
        void clear() { values.clear(); } // borra todo el mapa 
        
};
#endif
