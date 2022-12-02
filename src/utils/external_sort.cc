
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
                                return a.value < b.value; // > for descending, < for asc
                        }
                };

                void external_sort(char input[], size_t num_values, char output[], size_t mem_size)
                {

                        if (num_values == 0) {return;}

                        size_t RATIO = sizeof(uint64_t) / sizeof(char);
                        mem_size = int(mem_size / RATIO) * RATIO;
                        // cout << RATIO;
                        // output.resize(num_values * RATIO);
                        vector<element> mem;
                        if (num_values * RATIO <= mem_size) {
                                for (size_t i = 0; i < num_values; i++) {
                                        element curr = {0, 0};
                                        // curr.value = input[i*RATIO];
                                        memcpy(&curr.value, &input[i*RATIO], sizeof(uint64_t));
                                        // input.read_block(i * RATIO, sizeof(uint64_t), (char *)&curr.value);
                                        // mem[j] = curr;
                                        mem.push_back(curr);
                                        // cout << curr.value << endl;
                                }

                                make_heap(mem.begin(), mem.end(), element_cmp());
                                size_t counter = 0;
                                for (size_t i = 0; i < num_values; i++)
                                {
                                        element currmin = mem.front();
                                        pop_heap(mem.begin(), mem.end(), element_cmp());
                                        mem.pop_back();
                                        memcpy(&output[RATIO * counter++], &currmin.value, sizeof(uint64_t));
                                        // output.write_block((char *)&currmin.value, RATIO * counter++, sizeof(uint64_t));
                                }
                                return;
                        }

                        int running_count = 0;
                        // int prevCurr = num_values;
                        size_t curr = num_values * RATIO / mem_size + ((num_values * RATIO) % mem_size != 0);
                        int init_chunk = curr;
                        while (curr > (mem_size / RATIO))
                        {
                                running_count += 1;
                                // curr = prevCurr % mem_size + curr / mem_size; //+ (curr % mem_size != 0);
                                curr = curr - mem_size / RATIO + 1;
                        }
                        // running_count = init_chunk /(mem_size/RATIO) + ((num_values % (mem_size/RATIO)) != 0);

                        const size_t QUEUE_LENGTH = init_chunk + running_count;
                        // cout << curr << " curr";
                        size_t queue_idx = init_chunk;
                        // int queue_inner_idx[curr + 1] = {0};
                        vector<size_t> queue_inner_idx(QUEUE_LENGTH + 1);
                        std::vector<std::vector<char>> queue[QUEUE_LENGTH + 1];

                        // cout << "BEGIN of initializing chunk" << flush;
                        //  cout << mem_size << endl;
                        //  cout << init_chunk << endl;
                        //  cout << num_values << endl;
                        //  cout << QUEUE_LENGTH << endl;
                        for (int i = 0; i < init_chunk - 1; i++)
                        {
                                // cout << i << endl;
                                uint64_t *tempChunk = new uint64_t[mem_size / RATIO];
                                memcpy(tempChunk, &input[i*mem_size], mem_size);
                                // input.read_block(i * mem_size, mem_size, (char *)(tempChunk));
                                std::sort((uint64_t *)tempChunk, (uint64_t *)tempChunk + mem_size / sizeof(uint64_t));
                                // memcpy(&(queue[i][0]), (char *)tempChunk, mem_size);
                                queue[i].insert(queue[i].end(), (char *)tempChunk, (char *)tempChunk + mem_size);
                                // std::copy((char *)tempChunk, (char *)tempChunk + mem_size, &queue[i][0]);
                                // queue[i] = std::move(
                                //     File::make_temporary_file());
                                // queue[i]->resize(mem_size);
                                // queue[i]->write_block((char *)tempChunk, 0, mem_size);
                                // cout << "INIT CHUNK content" << endl;
                                // for (size_t a = 0; a < mem_size/RATIO; a++) {
                                //       cout << tempChunk[a] << " ";
                                // }
                                // cout << endl;
                                delete[] tempChunk;
                                // cout << "FIRST chunk created" << flush;
                        }
                        // cout << "Begin last creation\n" << flush;
                        // Sort the last small chunk
                        uint64_t *tempChunk = new uint64_t[num_values - mem_size * (init_chunk - 1) / RATIO];
                        memcpy(tempChunk, &input[mem_size * (init_chunk - 1)], num_values * RATIO - mem_size * (init_chunk - 1));
                        // input.read_block(mem_size * (init_chunk - 1), num_values * RATIO - mem_size * (init_chunk - 1), (char *)(tempChunk));
                        std::sort(tempChunk, tempChunk + (num_values - mem_size * (init_chunk - 1) / RATIO));
                        // queue[init_chunk - 1] = std::move(File::make_temporary_file());
                        // queue[init_chunk - 1]->resize((num_values * RATIO - mem_size * (init_chunk - 1)));
                        // queue[init_chunk - 1]->write_block((char *)tempChunk, 0, (num_values * RATIO - mem_size * (init_chunk - 1)));
                        // memcpy(queue[init_chunk - 1].data(), tempChunk, num_values * RATIO - mem_size * (init_chunk - 1));
                        // std::copy((char *)tempChunk, (char *)tempChunk + num_values * RATIO - mem_size * (init_chunk - 1), &queue[init_chunk - 1][0]);
                        queue[init_chunk - 1].insert(queue[init_chunk - 1].end(), (char *)tempChunk, (char *)tempChunk + num_values * RATIO - mem_size * (init_chunk - 1));
                        // cout << "INIT CHUNK content" << endl;
                        //       for (size_t a = 0; a < num_values - mem_size*(init_chunk-1)/RATIO; a++) {
                        //             cout << tempChunk[a] << " ";
                        //       }
                        //       cout << endl;
                        delete[] tempChunk;

                        // cout << "Reached end of chunk creation" << endl;

                        for (size_t i = 0; i < QUEUE_LENGTH; i += mem_size / RATIO)
                        {
                                // size_t file_idx_arr [mem_size];
                                //  Prepare for heap
                                size_t sizeOfNewChunk = 0;
                                for (size_t j = 0; j < mem_size / RATIO && i + j < queue_idx; j++)
                                {
                                        element curr = {0, i + j};
                                        // cout << i << " " << j << " " << queue_idx << endl;
                                        sizeOfNewChunk += queue[i + j].size();
                                        // uint64_t *tempChunk2 = new uint64_t[queue[i + j]->size() / RATIO];
                                        // queue[i + j]->read_block(0, queue[i + j]->size(), (char *)tempChunk2);
                                        // cout << "Number of i " << i << " j " << j << endl;
                                        // for (size_t a = 0; a < queue[i + j]->size(); a++) {
                                        //       cout << tempChunk2[a] << " ";
                                        // }
                                        // cout << "\n";
                                        memcpy((char *)&curr.value, &queue[i+j].data()[queue_inner_idx[i + j]],  sizeof(uint64_t));
                                        // queue[i + j]->read_block(queue_inner_idx[i + j], sizeof(uint64_t), (char *)&curr.value);
                                        // mem[j] = curr;
                                        //  cout << "INNER IDX \n";
                                        //  for (size_t b: queue_inner_idx) {
                                        //        cout << b << " ";
                                        //  }
                                        //  cout << endl;
                                        //  cout << curr.value << " ";
                                        mem.push_back(curr);
                                        // cout << curr.value << endl;
                                        // cout << queue_inner_idx[i + j] << "before\n";
                                        // queue_inner_idx[i + j] += 1;
                                        // cout << queue_inner_idx[i + j] << "after\n";
                                }
                                make_heap(mem.begin(), mem.end(), element_cmp());

                                // for (element i: mem) {
                                //       cout << i.value << " ";
                                // }
                                // cout << endl;
                                // cout << mem.front().chunk_id << endl;
                                // cout << queue_idx << endl;
                                // queue[queue_idx] = std::move(File::make_temporary_file());
                                // queue[queue_idx] = std::vector<char>(sizeOfNewChunk);
                                queue[queue_idx].clear();
                                // queue[queue_idx]->resize(sizeOfNewChunk);
                                size_t counter = 0;

                                // cout << "BEFORE while" << endl;
                                // cout << "output size " << output.size() << endl;
                                // cout << !mem.empty() << endl;

                                while (!mem.empty())
                                {
                                        element currmin = mem.front();

                                        pop_heap(mem.begin(), mem.end(), element_cmp());
                                        mem.pop_back();

                                        // cout << "currmin: " << currmin.value << " | mem layout";
                                        // for (element i: mem) {
                                        //       cout << i.value << " ";
                                        // }
                                        // cout << endl;
                                        // if (queue_idx == 200) {
                                        // cout << currmin.value << "  " ;
                                        //}
                                        // queue[queue_idx]->write_block((char *)&currmin.value, RATIO * counter, sizeof(uint64_t));
                                        // std::copy((char *)&currmin.value, (char *)&currmin.value + sizeof(uint64_t), &queue[queue_idx][RATIO * counter]);
                                        queue[queue_idx].insert(queue[queue_idx].begin() + RATIO * counter, (char *)&currmin.value, (char *)&currmin.value + sizeof(uint64_t));
                                        // memcpy(&queue[queue_idx].data()[RATIO * counter], (char *)&currmin.value, sizeof(uint64_t));
                                        // cout << "BEFORE Write to output" << endl;
                                        if (queue_idx == QUEUE_LENGTH)
                                        {
                                                // cout << "Start writing when idx == len" << endl;
                                                // cout << currmin.value << " ";
                                                memcpy(&output[RATIO * counter], (char *)&currmin.value, sizeof(uint64_t));
                                                // output.write_block((char *)&currmin.value, RATIO * counter, sizeof(uint64_t));
                                        }
                                        counter++;
                                        queue_inner_idx[currmin.chunk_id]++;
                                        element toPush = {0, currmin.chunk_id};
                                        if (queue_inner_idx[currmin.chunk_id] < queue[currmin.chunk_id].size() / RATIO)
                                        {
                                                memcpy((char *)&toPush.value, &queue[currmin.chunk_id].data()[queue_inner_idx[currmin.chunk_id] * RATIO], sizeof(uint64_t));
                                                // queue[currmin.chunk_id]->read_block(queue_inner_idx[currmin.chunk_id] * RATIO, sizeof(uint64_t), (char *)&toPush.value);
                                                mem.push_back(toPush);
                                                push_heap(mem.begin(), mem.end(), element_cmp());
                                        }
                                }
                                queue_idx++;
                        }
                        cout << endl;

                        // cout << "## Queue idx " << queue_idx << " | Queue Length " << QUEUE_LENGTH;
                        // if (queue_idx != QUEUE_LENGTH) {
                        //       cout << "Entering queue idx != LEN \n";
                        //       const size_t remChunkCount = QUEUE_LENGTH % mem_size;
                        //       for (size_t j = QUEUE_LENGTH - remChunkCount; j < remChunkCount; j++) {
                        //             element curr = {0, j};
                        //             queue[j]->read_block(queue_inner_idx[j] * RATIO, sizeof(uint64_t), (char *)&curr.value);
                        //             //mem[j] = curr;
                        //             mem.push_back(curr);
                        //             queue_inner_idx[j] += 1;
                        //       }
                        //       make_heap(mem.begin(), mem.end(), element_cmp());

                        //       size_t counter = 0;
                        //       while (!mem.empty()) {
                        //             element currmin = mem.front();
                        //             pop_heap(mem.begin(), mem.end(), element_cmp());
                        //             mem.pop_back();
                        //             output.write_block((char *)&currmin.value, RATIO * counter++, sizeof(uint64_t));
                        //             queue_inner_idx[currmin.chunk_id]++;
                        //             element toPush = {0,currmin.chunk_id};
                        //             if (queue_inner_idx[currmin.chunk_id] < queue[currmin.chunk_id]->size()) {
                        //                   queue[currmin.chunk_id]->read_block(queue_inner_idx[currmin.chunk_id], sizeof(uint64_t), (char *)&toPush.value);
                        //                   mem.push_back(toPush);
                        //                   push_heap(mem.begin(), mem.end(), element_cmp());
                        //             }
                        //       }
                        // }

                        // Release any undeleted variable
                        // delete[] &mem;
                        // delete[] &queue;
                        // delete[] &queue_idx;
                        // delete[] &queue_inner_idx;
                        // for (size_t i  = 0; i < QUEUE_LENGTH + 1; i++) {
                        //       delete (&queue[i]);
                        // }
                        // queue.clear();

                        // delete[] queue;
                        queue_inner_idx.clear();
                        mem.clear();
                }

        } // namespace externalsort
} // namespace buzzdb
