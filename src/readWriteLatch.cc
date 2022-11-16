#include <thread>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <shared_mutex>
#include <mutex>
#include <unistd.h>
#include <time.h>

// create a resource that will be incrimented once by each writer thread.
int resource = 0;

std::shared_mutex resourceLock; // latch for resource. Can be shared for read access, or exclusive for write access.

std::mutex printLock; // exclusive latch for printing, otherwise printed lines will coincide with each other.

std::vector<std::thread> threads; // threads to be joined.

void readWriteLatchExample() {

    /*
    
    Things to notice once run:

    1. Many read latches may be aquired in a row. This is because reading threads aquire a shared mutex. Similarly, many read latches may be released in a row.

    2. Threads that aquire a write latch must wait (often a while) for all read latches to be released. Read threads must wait until the write lock is released to aquire read locks.

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
                    std::cout << "Thread "<< i << " AQUIRED writer latch" << std::endl;
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
                    std::cout << "Thread "<< i << " AQUIRED read latch" << std::endl;
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