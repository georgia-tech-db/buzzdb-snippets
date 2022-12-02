
#include <algorithm>
#include <fstream>
#include <iostream>
#include <utility>
#include "../include/utils/file.h"

namespace buzzdb {
namespace utils {

#define UNUSED(p) ((void)(p))

utils::RHT::RHT(int table_size){
  cout << "coming here" << endl;
  this->table_size = table_size;
  this->keys_inserted = 0;
  this->data_items.reserve(table_size);
  this->probe_seq_len.reserve(table_size);
  for(int i=0;i<table_size;i++){
    this->data_items[i].val = -1;
    this->probe_seq_len[i] = 0;
  }
}

int utils::RHT::key_hasher(int key){
  return (key % table_size);
}

bool utils::RHT::insert(int key, int val){
  bool inserted = false;
  if(keys_inserted == table_size)
    return inserted;
  
  int hash_key = key_hasher(key);
  int key_probe = 0;
  int curr_key = hash_key;
  while(true){
    if(data_items[hash_key].val == -1){
      data_items[hash_key].key = key;
      data_items[hash_key].val = val;
      keys_inserted++;
      inserted = true;
      break;
    }
    else if(key_probe > probe_seq_len[curr_key]){
      data tmp = data_items[curr_key];
      data_items[curr_key] = data_items[hash_key];
      key = tmp.key;
      val = tmp.val;
      key_probe = 0;
    }
    key_probe++;
    curr_key = (curr_key + 1) % table_size;
  }
  return inserted;
}

pair<int, bool> utils::RHT::search(int key) {
  bool found = false;
  int found_val = -1;
  int hash_key = key_hasher(key);
  int key_probe = 0;
  int curr_key = hash_key;
  while(true){
    if(data_items[hash_key].val != -1 && data_items[hash_key].key == key){
      found = true;
      found_val = data_items[hash_key].val;
      break;
    }
    else if(data_items[hash_key].val == -1 || key_probe > probe_seq_len[curr_key]){
      break;
    }
    key_probe++;
    curr_key = (curr_key + 1) % table_size;
  }
  return make_pair(found_val, found);
}

bool utils::RHT::erase(int key){
  bool deleted = false;
  int hash_key = key_hasher(key);
  int key_probe = 0;
  int curr_key = hash_key;
  while(true){
    if(data_items[hash_key].val == -1 || key_probe > probe_seq_len[curr_key]){
      break;
    }
    else if(data_items[hash_key].key == key){
      data_items[hash_key].val = -1; // try to make the val nullable | maybe using optional? | int when set to NULL defaults to 0
      probe_seq_len[hash_key] = -1;
      keys_inserted--;
      deleted = true;
      break; 
    }
    key_probe++;
    curr_key = (curr_key + 1) % table_size;
  }
  return deleted;
}


}  // namespace utils
}  // namespace buzzdb
