#include "rwlatch.hpp"
#include <iostream>     
#include <thread>   
#include <condition_variable>
#include <mutex>          
#include <string>


void test_mutiple_reads(){
  std::cout << "Test_Mutiple_Reads" << std::endl;
  std::vector<std::thread> threads;
  for(int i = 0; i < 3; i++){
    threads.push_back(std::thread(read_lock));
  }

  for(int i = 0; i < 3; i++){
    threads.push_back(std::thread(read_unlock));
  }

  for (auto &th : threads) {
    th.join();
  }

  std::cout << std::endl;
}

void test_write_read_latch(){
  std::cout << "Test_Write_Read_Latch" << std::endl;
  std::vector<std::thread> threads;
 
  // two reader threads
  threads.push_back(std::thread(read_lock));
  threads.push_back(std::thread(read_lock));


  // one writer thread
  threads.push_back(std::thread(write_lock));

  // another read thread
  threads.push_back(std::thread(read_lock));

  // unlock all readers
  threads.push_back(std::thread(read_unlock));
  threads.push_back(std::thread(read_unlock));
  threads.push_back(std::thread(read_unlock));

  // unlock writer thread
  threads.push_back(std::thread(write_unlock));

  for (auto &th : threads) {
    th.join();
  } 
  std::cout << std::endl;
}


void test_write_read_latch_heavy(){
  std::cout << "Test_Write_Read_Latch_Heavy" << std::endl;
  std::vector<std::thread> threads;
 
  // three reader threads
  threads.push_back(std::thread(read_lock));
  threads.push_back(std::thread(read_lock));
  threads.push_back(std::thread(read_lock));

  threads.push_back(std::thread(read_unlock));

  // one writer thread
  threads.push_back(std::thread(write_lock));
  
  // unlock writer thread
  threads.push_back(std::thread(write_unlock));

  // another read thread
  threads.push_back(std::thread(read_lock));

  // unlock all readers
  threads.push_back(std::thread(read_unlock));
  threads.push_back(std::thread(read_unlock));
  threads.push_back(std::thread(read_unlock));


  for (auto &th : threads) {
    th.join();
  } 
}
int main(int argc, char* argv[]){
  
  test_mutiple_reads();
  test_write_read_latch();
  test_write_read_latch_heavy();
  
}