#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <vector>
#include <string>
#include <set>
#include "bucket.h"

class Directory {
    int global_depth, bucket_size;
    vector<Bucket*> buckets;

    int hash(int n) { return n & ((1<<global_depth) - 1); }
    
    int pairIndex(int bucket_no, int depth) { return bucket_no ^ (1<<(depth - 1)); }
    
    void grow() {
        for (int i = 0; i < 1<<global_depth; i++)
            buckets.push_back(buckets[i]);
        global_depth++;
    }
    
    void split(int bucket_no) {
        int local_depth, pair_index, index_diff, dir_size;
        map<int, string> temp;

        local_depth = buckets[bucket_no]->incrementDepth();
        if (local_depth > global_depth)
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
        for (auto it = temp.begin(); it != temp.end(); it++)
            insert((*it).first, (*it).second, 1);
    }
    
    string bucket_id(int n) {
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
        Directory(int depth, int bucket_size) : global_depth(depth), bucket_size(bucket_size) {
            for (int i = 0; i < 1<<depth ; i++)
                buckets.push_back(new Bucket(depth, bucket_size));
        }
        
        void insert(int key, string value, bool reinserted) {
            int bucket_no = hash(key);
            int status = buckets[bucket_no]->insert(key, value);
            if (status == 1) {
                if (!reinserted)
                    cout << "Key insertada " << key << " en bucket " << bucket_id(bucket_no) << endl;
                else
                    cout << "Key movida " << key << " a bucket " << bucket_id(bucket_no) << endl;
            } else if (status == 0) {
                split(bucket_no);
                insert(key, value, reinserted);
            } else {
                cout << "Key " << key << " ya existe en bucket " << bucket_id(bucket_no) << endl;
            }
        }
        
        void update(int key, string value) {
            int bucket_no = hash(key);
            buckets[bucket_no]->update(key, value);
        }
        
        void search(int key) {
            int bucket_no = hash(key);
            cout << "Buscando key " << key << " en bucket " << bucket_id(bucket_no) << endl;
            buckets[bucket_no]->search(key);
        }
        
        void display(bool duplicates) {
            int d;
            string s;
            set<string> shown;
            cout << "Global depth: " << global_depth << endl;
            for (int i = 0; i < buckets.size(); i++) {
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
