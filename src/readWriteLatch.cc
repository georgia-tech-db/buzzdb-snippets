#include <thread>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <shared_mutex>
#include <mutex>
#include <unistd.h>
#include <time.h>

// ______               _                 
// | ___ \             | |                
// | |_/ /___  __ _  __| |                
// |    // _ \/ _` |/ _` |                
// | |\ \  __/ (_| | (_| |                
// \_| \_\___|\__,_|\__,_|                
                                    
//  _    _      _ _                       
// | |  | |    (_) |                      
// | |  | |_ __ _| |_ ___                 
// | |/\| | '__| | __/ _ \                
// \  /\  / |  | | ||  __/                
//  \/  \/|_|  |_|\__\___|                
                                                                            
//  _           _       _                 
// | |         | |     | |                
// | |     __ _| |_ ___| |__              
// | |    / _` | __/ __| '_ \             
// | |___| (_| | || (__| | | |            
// \_____/\__,_|\__\___|_| |_|            
                                                                             
//  _____                          _      
// |  ___|                        | |     
// | |____  ____ _ _ __ ___  _ __ | | ___ 
// |  __\ \/ / _` | '_ ` _ \| '_ \| |/ _ \
// | |___>  < (_| | | | | | | |_) | |  __/
// \____/_/\_\__,_|_| |_| |_| .__/|_|\___|
//                          | |           
//                          |_|           


// create a resource that will be incrimented once by each writer thread.
int resource = 0;

std::shared_mutex resourceLock; // latch for resource. Can be shared for read access, or exclusive for write access.

std::mutex printLock; // exclusive latch for printing, otherwise printed lines will coincide with each other.

std::vector<std::thread> threads; // threads to be joined.

void readWriteLatchExample() {

    /*

    Objective: showcase behavior of read and write latches (with std::shared_mutex). In this simple example, we will atomically increment a resource value to represent the number of writers who have obtained the writer lock.
    
    Things to notice once run:

    1. Many read latches may be acquired in a row. This is because reading threads acquire a shared mutex. Similarly, many read latches may be released in a row.

    2. Threads that eventually acquire a write latch must wait (often a while) for all read latches to be released. Read threads must wait until the write lock is released to acquire read locks.

    3. The final resource value equals the number of writer threads, since atomic incrementation is experienced through mutual exclusion.
    
    */

    srand(time(NULL));
    for (size_t i = 0; i < 10; ++i) {
        threads.emplace_back([i] {

            bool isWriter = rand() / float(RAND_MAX) > 0.7;

            {
                // RAII-style latch mechanism
                std::lock_guard<std::mutex> lock(printLock);
                std::cout << "Thread "<< i << " waiting for " << (isWriter ? "writer" : "reader") << " latch" << std::endl;
            }

            if (isWriter) {
                resourceLock.lock();
                {
                    std::lock_guard<std::mutex> lock(printLock);
                    std::cout << "Thread "<< i << " ACQUIRED writer latch" << std::endl;
                }
                resource++;
                {
                    std::lock_guard<std::mutex> lock(printLock);
                    std::cout << "\nResource is now: " << resource << "\n" << std::endl;
                    std::cout << "Thread "<< i << " releasing writier latch" << std::endl;
                }
                resourceLock.unlock();
            } else {
                resourceLock.lock_shared();
                {
                    std::lock_guard<std::mutex> lock(printLock);
                    std::cout << "Thread "<< i << " ACQUIRED read latch" << std::endl;
                }
                sleep(1);
                {
                    std::lock_guard<std::mutex> lock(printLock);
                    std::cout << "Thread "<< i << " releasing read latch" << std::endl;
                }
                resourceLock.unlock_shared();
            }
        });
    }
    for (auto& thread : threads) {
        thread.join();
    }
    std::cout << "--------\nNumber of writers processed: " << resource << std::endl;
}

int main() {
    readWriteLatchExample();
}

/* 

Example output:

Thread 0 waiting for writer latch
Thread 0 ACQUIRED writer latch

Resource is now: 1

Thread 0 releasing writier latch
Thread 2 waiting for reader latch
Thread 2 ACQUIRED read latch
Thread 1 waiting for writer latch
Thread 5 waiting for reader latch
Thread 5 ACQUIRED read latch
Thread 7 waiting for reader latch
Thread 7 ACQUIRED read latch
Thread 6 waiting for writer latch
Thread 4 waiting for reader latch
Thread 4 ACQUIRED read latch
Thread 3 waiting for reader latch
Thread 3 ACQUIRED read latch
Thread 9 waiting for reader latch
Thread 9 ACQUIRED read latch
Thread 8 waiting for reader latch
Thread 8 ACQUIRED read latch
Thread 5 releasing read latch
Thread 7 releasing read latch
Thread 3 releasing read latch
Thread 2 releasing read latch
Thread 9 releasing read latch
Thread 8 releasing read latch
Thread 4 releasing read latch
Thread 1 ACQUIRED writer latch

Resource is now: 2

Thread 1 releasing writier latch
Thread 6 ACQUIRED writer latch

Resource is now: 3

Thread 6 releasing writier latch
--------
Number of writers processed: 3
*/