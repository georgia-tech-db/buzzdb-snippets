#pragma once

#include <iostream>     
#include <thread>   
#include <condition_variable>
#include <mutex>          
#include <string> 
#include <vector>

//Reference: 
//https://stackoverflow.com/questions/12033188/how-would-you-implement-your-own-reader-writer-lock-in-c11
//https://github.com/kw90/ReadWriteLock
//https://www.educative.io/blog/modern-multithreading-and-concurrency-in-cpp

std::mutex mtx;         // mutex for critical section
bool write_locked = false;
int reader_cnt = 0;
std::condition_variable can_read_or_write;
std::condition_variable can_unlock;

void read_lock() {
  std::unique_lock<std::mutex> unq_mtx(mtx);
  while (write_locked){
    can_read_or_write.wait(unq_mtx);  // wait under notification(notify-all)
    // Atomically unlocks lock, blocks the current executing thread. A notify will allow it to lock the lock, and unblocks the current thread.
  }

  reader_cnt++;
  std::cout << "Reader locks successfully" << std::endl;
  std::cout << "Number of concurrent readers now: " << reader_cnt << std::endl;
  can_unlock.notify_all();
}


void read_unlock(){

    std::unique_lock<std::mutex> unq_mtx(mtx);
    while(reader_cnt < 1){
        can_unlock.wait(unq_mtx);
    }

    reader_cnt --;
    std::cout << "Reader unlocks successfully" << std::endl;
    std::cout << "Number of concurrent readers now: " << reader_cnt << std::endl;
    if(reader_cnt == 0){
        can_read_or_write.notify_all();
    }
}

void write_lock(){

  std::unique_lock<std::mutex> unq_mtx(mtx);
  while(reader_cnt || write_locked){
    can_read_or_write.wait(unq_mtx);
  }
  write_locked = true;
  std::cout << "writer locks successfully" << std::endl;
  can_unlock.notify_all();
}

void write_unlock(){

  std::unique_lock<std::mutex> unq_mtx(mtx);
  while(!write_locked){
    can_unlock.wait(unq_mtx);
  }
  
  write_locked = false;
  std::cout << "writer unlocks successfully" << std::endl;
  can_read_or_write.notify_all();  // notify all waits
}