#ifndef BUCKET_H
#define BUCKET_H

#include <iostream>
#include <map>
using namespace std;

class Bucket {
    int depth, size;
    map<int, string> values;

    public:
        Bucket(int depth, int size) : depth(depth), size(size) {};
        
        void search(int key) {
            auto it = values.find(key);
            if (it != values.end())
                cout << "Valor = " << it->second << endl;
            else
                cout << "Key no existe\n";
        }

        int insert(int key,string value) {
            auto it = values.find(key);
            if (it != values.end())
                return -1;
            if (isFull())
                return 0;
            values[key] = value;
            return 1;
        }
        
        bool update(int key, string value) {
            auto it = values.find(key);
            if (it != values.end()) {
                values[key] = value;
                cout << "Valor actualizado\n";
                return true;
            } else {
                cout << "No se puede actualizar: key no existe\n";
                return false;
            }
        }
        
        void display() {
            for (auto it = values.begin(); it != values.end(); it++)
                cout << it->first << " ";
            cout << endl;
        }
        
        map<int, string> copy() {
            map<int, string> temp(values.begin(), values.end());
            return temp;
        }

        bool isFull() {
            if (values.size() == size)
                return true;
            return false;
        }
        
        bool isEmpty() {
            if (values.size() == 0)
                return true;
            return false;
        }
        
        int getDepth() { return depth; }
        
        int incrementDepth() {
            depth++;
            return depth;
        }
        
        int decrementDepth() {
            depth--;
            return depth;
        }
        
        void clear() { values.clear(); }
        
};
#endif
