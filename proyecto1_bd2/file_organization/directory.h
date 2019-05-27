#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <vector>
#include <string>
#include <set>
#include "file_organization/bucket.h"

class Directory {
    int global_depth, bucket_size; // profundidad global y tamaño de los buckets
    vector<Bucket*> buckets; // vector que apunta a los buckets

    int hash(int n) { return n & ((1<<global_depth) - 1); } // funcion hash
    
    int pairIndex(int bucket_no, int depth) { return bucket_no ^ (1<<(depth - 1)); } // 
    
    void grow() { // funcion grow
        for (int i = 0; i < 1<<global_depth; i++) // aumentar buckets
            buckets.push_back(buckets[i]);
        global_depth++; // aumenta profundidad global
    }
    
    void split(int bucket_no) { // funcion split
        int local_depth, pair_index, index_diff, dir_size;
        map<int, long> temp;

        local_depth = buckets[bucket_no]->incrementDepth();
        if (local_depth > global_depth) // incrementar profundidad global si un bucket tiene una mas profunda
            grow();
        
        pair_index = pairIndex(bucket_no, local_depth);
        buckets[pair_index] = new Bucket(local_depth, bucket_size);
        temp = buckets[bucket_no]->copy();
        buckets[bucket_no]->clear();
        index_diff = 1<<local_depth;
        dir_size = 1<<global_depth;

        for (int i = pair_index - index_diff; i >= 0; i-=index_diff) 
            buckets[i] = buckets[pair_index];
        for (int i = pair_index + index_diff; i < dir_size; i+=index_diff) 
            buckets[i] = buckets[pair_index];
        for (auto it = temp.begin(); it != temp.end(); it++) // insertar antiguas key en nuevos buckets
            insert((*it).first, (*it).second, 1);
    }
    
    string bucket_id(int n) { // funcion bucket_id
        int d;
        string s;
        d = buckets[n]->getDepth();
        s = "";
        while (n > 0 && d > 0) {
            s = (n % 2 == 0 ? "0" : "1") + s;
            n /=2;
            d--;
        }
        while (d > 0) {
            s = "0" + s;
            d--;
        }
        return s;
    }

    public:
        Directory(int depth, int bucket_size) : global_depth(depth), bucket_size(bucket_size) { // Constructor del Directory 
            for (int i = 0; i < 1<<depth ; i++)
                buckets.push_back(new Bucket(depth, bucket_size));
        }
        
        int insert(int key, long value, bool inserted) { // funcion insert
            int bucket_no = hash(key); // bucar posicion de la key
            int status = buckets[bucket_no]->insert(key, value);
            if (status == 1) { // si no esta key en bucket
                if (!inserted) 
                    cout << "Key insertada " << key << " en bucket " << bucket_id(bucket_no) << endl;
                else // al hacer split y mover key
                    cout << "Key movida " << key << " a bucket " << bucket_id(bucket_no) << endl;
            } else if (status == 0) { // si bucket esta lleno
                split(bucket_no);
                insert(key, value, inserted);
            } else { // si ya existe key en el bucket
                cout << "Key " << key << " ya existe en bucket " << bucket_id(bucket_no) << endl;
                return 1;
            }
            return 0;
        }
        
        void update(int key, long value) { // funcion update
            int bucket_no = hash(key); // buscar posicion de la key
            buckets[bucket_no]->update(key, value); // actualizar valor de ka key
        }
        
        long search(int key) { // funcion search
            int bucket_no = hash(key); // buscar posicion de la key
            cout << "Buscando key " << key << " en bucket " << bucket_id(bucket_no) << endl;
            long pos = buckets[bucket_no]->search(key); // posicion del bucket
            cout << "pos = " << pos << endl;
            return pos;
        }
        
        void display(bool duplicates) { // funcion display
            int d;
            string s;
            set<string> shown;
            cout << "Global depth: " << global_depth << endl;
            for (int i = 0; i < buckets.size(); i++) { // imprime el hash table y los buckets con sus keys
                d = buckets[i]->getDepth();
                s = bucket_id(i);
                if (duplicates || shown.find(s) == shown.end()) {
                    shown.insert(s);
                    for (int j = d; j <= global_depth; j++)
                        cout << " ";
                    cout << s << " -> ";
                    buckets[i]->display();
                }
            }
        }
};

#endif
