
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <string.h>
using namespace std;

namespace buzzdb
{
        namespace externalsort
        {

#define UNUSED(p) ((void)(p))

                struct element
                {
                        uint64_t value;
                        size_t chunk_id = -1;
                };

                struct element_cmp
                {
                        bool operator()(element const &a, element const &b) const {
                                return a.value > b.value; // > for descending, < for asc
                        }
                };
                /// @brief In this toy external sort code snippet, I used array instead of File wrapper as of project 1 to avoid similarity. 
                ///        memcpy essentially does the same job by moving data around in terms of bytes.
                ///        The underlying sorting process is still external sorting
                /// @param input Input char array
                /// @param num_values values to be sorted in the input array
                /// @param output Empty char array to store output
                /// @param mem_size size of emulated memory
                void external_sort(char input[], size_t num_values, char output[], size_t mem_size)
                {

                        if (num_values == 0) {return;}

                        size_t RATIO = sizeof(uint64_t) / sizeof(char);
                        mem_size = int(mem_size / RATIO) * RATIO;
                        vector<element> mem;
                        if (num_values * RATIO <= mem_size) {
                                for (size_t i = 0; i < num_values; i++) {
                                        element curr = {0, 0};
                                        memcpy(&curr.value, &input[i*RATIO], sizeof(uint64_t));
                                        mem.push_back(curr);
                                }

                                make_heap(mem.begin(), mem.end(), element_cmp());
                                size_t counter = 0;
                                for (size_t i = 0; i < num_values; i++)
                                {
                                        element currmin = mem.front();
                                        pop_heap(mem.begin(), mem.end(), element_cmp());
                                        mem.pop_back();
                                        memcpy(&output[RATIO * counter++], &currmin.value, sizeof(uint64_t));
                                }
                                cout << "no overflow" << endl;
                                return;
                        }

                        int running_count = 0;
                        size_t curr = num_values * RATIO / mem_size + ((num_values * RATIO) % mem_size != 0);
                        int init_chunk = curr;
                        cout << "A" << endl;
                        cout << curr << " " << mem_size << " " << RATIO << endl;
                        while (curr > (mem_size / RATIO))
                        {
                                running_count += 1;
                                curr = curr - mem_size / RATIO + 1;
                                cout << curr;
                        }
                        cout << "B" << endl;
                        const size_t QUEUE_LENGTH = init_chunk + running_count;

                        size_t queue_idx = init_chunk;
                        vector<size_t> queue_inner_idx(QUEUE_LENGTH + 1);
                        std::vector<std::vector<char>> queue = std::vector<std::vector<char>>(QUEUE_LENGTH + 1);

                        for (int i = 0; i < init_chunk - 1; i++)
                        {
                                uint64_t *tempChunk = new uint64_t[mem_size / RATIO];
                                memcpy(tempChunk, &input[i*mem_size], mem_size);
                                std::sort((uint64_t *)tempChunk, (uint64_t *)tempChunk + mem_size / sizeof(uint64_t));
                                queue[i].assign((char *)tempChunk, (char *)tempChunk + mem_size);
                                // queue[i].insert(queue[i].end(), (char *)tempChunk, (char *)tempChunk + mem_size);
                                delete[] tempChunk;
                        }
                        uint64_t *tempChunk = new uint64_t[num_values - mem_size * (init_chunk - 1) / RATIO];
                        memcpy(tempChunk, &input[mem_size * (init_chunk - 1)], num_values * RATIO - mem_size * (init_chunk - 1));
                        std::sort(tempChunk, tempChunk + (num_values - mem_size * (init_chunk - 1) / RATIO));
                        queue[init_chunk - 1].insert(queue[init_chunk - 1].end(), (char *)tempChunk, (char *)tempChunk + num_values * RATIO - mem_size * (init_chunk - 1));
                        delete[] tempChunk;
                        cout << "C" << endl;
                        for (size_t i = 0; i < QUEUE_LENGTH; i += mem_size / RATIO)
                        {
                                //  Prepare for heap
                                size_t sizeOfNewChunk = 0;
                                for (size_t j = 0; j < mem_size / RATIO && i + j < queue_idx; j++)
                                {
                                        element curr = {0, i + j};
                                        
                                        sizeOfNewChunk += queue[i + j].size();
                                       
                                        memcpy((char *)&curr.value, &queue[i+j].data()[queue_inner_idx[i + j]],  sizeof(uint64_t));
                                        mem.push_back(curr);
                                }
                                make_heap(mem.begin(), mem.end(), element_cmp());

                                size_t counter = 0;

                                while (!mem.empty())
                                {
                                        element currmin = mem.front();

                                        pop_heap(mem.begin(), mem.end(), element_cmp());
                                        mem.pop_back();

                                        queue[queue_idx].insert(queue[queue_idx].begin() + RATIO * counter, (char *)&currmin.value, (char *)&currmin.value + sizeof(uint64_t));
                                        if (queue_idx == QUEUE_LENGTH)
                                        {

                                                memcpy(&output[RATIO * counter], (char *)&currmin.value, sizeof(uint64_t));

                                        }
                                        counter++;
                                        queue_inner_idx[currmin.chunk_id]++;
                                        element toPush = {0, currmin.chunk_id};
                                        if (queue_inner_idx[currmin.chunk_id] < queue[currmin.chunk_id].size() / RATIO)
                                        {
                                                memcpy((char *)&toPush.value, &queue[currmin.chunk_id].data()[queue_inner_idx[currmin.chunk_id] * RATIO], sizeof(uint64_t));
                                                mem.push_back(toPush);
                                                push_heap(mem.begin(), mem.end(), element_cmp());
                                        }
                                }
                                queue_idx++;
                        }
                        cout << "D" << endl;
                        cout << endl;
                        queue_inner_idx.clear();
                        mem.clear();
                }

        } // namespace externalsort
} // namespace buzzdb
