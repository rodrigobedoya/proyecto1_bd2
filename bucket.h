#ifndef BUCKET_H
#define BUCKET_H

#include <map>

class Bucket {
    int depth, size;
    map<int, string> values;

    public:
        Bucket(int depth, int size) : depth(depth), size(size) {};
        
        int insert(int key,string value) {
            auto it = values.find(key);
            if (it != values.end())
                return -1;
            if (isFull())
                return 0;
            values[key] = value;
            return 1;
        }
        
        int remove(int key) {
            auto it = values.find(key);
            if (it != values.end()) {
                values.erase(it);
                return -1;
            } else {
                cout << "No se puede remover: key no existe\n";
                return 0;
            }
        }
        
        int update(int key, string value) {
            auto it = values.find(key);
            if (it != values.end()) {
                values[key] = value;
                cout << "Valor actualizado\n";
                return 1;
            } else {
                cout << "No se puede actualizar: key no existe\n";
                return 0;
            }
        }
        
        void search(int key) {
            auto it = values.find(key);
            if (it != values.end())
                cout << "Valor = " << it->second << endl;
            else
                cout << "Key no existe\n";
        }
        
        int isFull() {
            if (values.size() == size)
                return 1;
            else
                return 0;
        }
        
        int isEmpty() {
            if (values.size() == 0)
                return 1;
            else
                return 0;
        }
        
        int getDepth() {
            return depth;
        }
        
        int increaseDepth() {
            depth++;
            return depth;
        }
        
        int decreaseDepth() {
            depth--;
            return depth;
        }
        
        map<int, string> copy() {
            map<int, string> temp(values.begin(), values.end());
            return temp;
        }
        
        void clear() {
            values.clear();
        }
        
        void display() {
            for (auto it = values.begin(); it != values.end(); it++)
                cout << it->first << " ";
            cout << endl;
        }
};
#endif