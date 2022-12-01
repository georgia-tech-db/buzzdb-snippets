#include "include/external_sort.h"
#include <vector>
#include <memory>
#include <algorithm>

/// @brief Merging chunk_files two by two.
void two_way_merge(std::vector<std::unique_ptr<File>> &chunk_files, File &output);

/// @brief Merging two sorted array(in files)
void merge_two_files(std::unique_ptr<File> file1, std::unique_ptr<File> file2,
                    File &output_file);

void external_sort(File& input, size_t num_values, File& output, size_t mem_size) {

  // First, we divide the file into small chunks
  // so that each chunk can be sorted in memory.
  size_t chunk_len = mem_size / sizeof(uint64_t);

  size_t num_chunks = num_values / chunk_len;
  std::vector<size_t> val_count_per_chunk(num_chunks, chunk_len);

  // If there is a remainder we have to have one more chunk
  if (num_values % chunk_len > 0) {
    num_chunks += 1;
    val_count_per_chunk.push_back(num_values % chunk_len);
  }

  // chunk files container
  std::vector<std::unique_ptr<File>> chunk_files;
  size_t input_file_offset = 0;

  std::unique_ptr<uint64_t[]> only_chunk;

  // build chunks and sort them
  for (size_t i = 0; i < num_chunks; i++) {
    size_t number_count = val_count_per_chunk[i];
    size_t num_bytes = number_count * sizeof(uint64_t);

    // get a chunk from input file
    auto chunk = std::make_unique<uint64_t[]>(number_count);
    input.read_block(input_file_offset, num_bytes,
                     reinterpret_cast<char*>(chunk.get()));
    input_file_offset += num_bytes;

    // sort the chunk
    std::sort(chunk.get(), chunk.get() + number_count);

    if (num_chunks == 1) {
      only_chunk = std::move(chunk);
      break;
    }

    // create temp file
    std::unique_ptr<File> temp_file = File::make_temporary_file();
    temp_file->resize(num_bytes);
    // write to temp file
    temp_file->write_block(reinterpret_cast<char*>(chunk.get()), 0, num_bytes);
    chunk_files.push_back(std::move(temp_file));
  }

  if (num_chunks == 1) {
    output.resize(num_values * sizeof(uint64_t));
    output.write_block(reinterpret_cast<char*>(only_chunk.get()), 0, num_values * sizeof(uint64_t));
  }
  else {
    // Now we have the sorted chunks and we are going to merge them
    // For simplicity, a two-way merge is used here.
    two_way_merge(chunk_files, output);
  }

}

void two_way_merge(std::vector<std::unique_ptr<File>> &chunk_files, File &output) {

  std::vector<std::unique_ptr<File>> input_files(std::move(chunk_files));

  while (input_files.size() > 1) {

    size_t num_output_files = input_files.size() / 2 + input_files.size() % 2;
    std::vector<std::unique_ptr<File>> output_files(num_output_files);

    bool last_merge = num_output_files == 1;

    for (size_t i = 0; i < num_output_files; i++) {
      // In this case, we can pair two files. Merge them.
      if (2 * i + 1 < input_files.size()) {
        
        std::unique_ptr<File> temp_file = File::make_temporary_file();
        // If we are doing the last merge, write result to output file.
        File &temp_file_ref = last_merge ? output : (*temp_file);

        merge_two_files(std::move(input_files[2 * i]),
                        std::move(input_files[2 * i + 1]), temp_file_ref);
        
        // If we are not doing the last merge, we need to keep this output file
        // because it is one of the input files of the next round.
        if (num_output_files != 1) {
          output_files[i] = std::move(temp_file);
        }
      }
      // Otherwise, put the last file directly in output_files;
      else {
        output_files[i] = std::move(input_files[2 * i]);
      }
    }
    input_files = std::move(output_files);
  }
}


void merge_two_files(std::unique_ptr<File> file1, std::unique_ptr<File> file2,
                    File &output_file) {

  File* two_files[2] = {file1.get(), file2.get()};
  size_t file_sizes[2] = {file1->size(), file2->size()};
  size_t read_offset[2] = {0, 0};

  size_t write_offset = 0;
  output_file.resize(file_sizes[0] + file_sizes[1]);

  // A utility function that reads the next value in the specified numbered file.
  auto next_val = [&](size_t file_no) {
    uint64_t val;
    two_files[file_no]->read_block(
      read_offset[file_no], sizeof(uint64_t), reinterpret_cast<char*>(&val));
    read_offset[file_no] += 8;
    return val;
  };

  // A utility function that writes value to the output file.
  auto write_to_output = [&output_file, &write_offset] (uint64_t val) {
    output_file.write_block(
      reinterpret_cast<char*>(&val), write_offset, sizeof(uint64_t));
    write_offset += 8;
  };

  // 0 or 1, indicating which file we are reading from.
  size_t lane;
  // value slot for the two file. (since they are files, we
  // want to read the leading values into memory first to do comparison)
  uint64_t values[2] = { next_val(0), next_val(1) };
  
  // merge
  while (true) {
    lane = values[0] < values[1] ? 0 : 1;
    write_to_output(values[lane]);

    if (read_offset[lane] >= file_sizes[lane]) {
      break;
    }
    values[lane] = next_val(lane);
  }
  // Write another value in the slot to output.
  write_to_output(values[1 - lane]);

  if (read_offset[0] == file_sizes[0] && read_offset[1] == file_sizes[1]) {
    return;
  }

  // Write the remain values
  size_t which_has_remain = read_offset[0] >= file_sizes[0] ? 1 : 0;

  while (read_offset[which_has_remain] < file_sizes[which_has_remain]) {
    uint64_t val = next_val(which_has_remain);
    write_to_output(val);
  }

}