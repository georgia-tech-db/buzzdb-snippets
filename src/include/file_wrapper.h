#pragma once

#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#include <memory>
#include <system_error>

class File {
 public:
  File(int fd, size_t size): fd(fd), file_size(size) {}

  File(const char* filename) {
    fd = ::open(filename, O_RDWR | O_CREAT | O_SYNC, 0666);
    
    if (fd < 0) {
      throw std::system_error{errno, std::system_category()};
    }
    file_size = read_size();
  }

  ~File() {
    ::close(fd);
  };

  /// Returns the current size of the file in bytes.
  size_t size() const {
    return file_size;
  }

  /// Resizes the file to `new_size`. If `new_size` is smaller than `size()`,
  /// the file is cut off at the end. Otherwise zero bytes are appended at
  /// the end.
  void resize(size_t new_size) {
    if (new_size == file_size) {
      return;
    }
    if (::ftruncate(fd, new_size) < 0) {
      throw std::system_error{errno, std::system_category()};
    }
    file_size = new_size;
  }

  /// Reads a block of the file. `offset + size` must not be larger than
  /// `size()`.
  /// @param[in]  offset The offset in the file from which the block should
  ///                    be read.
  /// @param[in]  size   The size of the block.
  /// @param[out] block  A pointer to memory where the block is written to.
  ///                    Must be able to hold at least `size` bytes.
  void read_block(size_t offset, size_t size, char* block) {
    size_t total_bytes_read = 0;
    while (total_bytes_read < size) {
      ssize_t bytes_read =
          ::pread(fd, block + total_bytes_read, size - total_bytes_read,
                  offset + total_bytes_read);
      if (bytes_read <= 0) {
        throw std::system_error{errno, std::system_category()};
      }
      total_bytes_read += static_cast<size_t>(bytes_read);
    }
  }

  /// Writes a block to the file. `offset + size` must not be larger than
  /// `size()`. If you want to write past the end of the file, use
  /// `resize()` first.
  /// @param[in] block  A pointer to memory that will be written to the
  ///                   file. Must hold at least `size` bytes.
  /// @param[in] offset The offset in the file at which the block should be
  ///                   written.
  /// @param[in] size   The size of the block.
  void write_block(const char* block, size_t offset, size_t size) {
    size_t total_bytes_written = 0;
    while (total_bytes_written < size) {
      ssize_t bytes_written =
          ::pwrite(fd, block + total_bytes_written, size - total_bytes_written,
                   offset + total_bytes_written);
      if (bytes_written <= 0) {
        throw std::system_error{errno, std::system_category()};
      }
      total_bytes_written += static_cast<size_t>(bytes_written);
    }
  }

  /// Opens a file. Existing files are never overwritten.
  /// @param[in] filename Path to the file.
  static std::unique_ptr<File> open_file(const char* filename) {
    return std::make_unique<File>(filename);
  }

  /// Opens a temporary file in `WRITE` mode. The file will be deleted
  /// automatically after use.
  static std::unique_ptr<File> make_temporary_file() {
    char file_template[] = ".tmpfile-XXXXXX";
    int fd = ::mkstemp(file_template);
    if (fd < 0) {
      throw std::system_error{errno, std::system_category()};
    }
    return std::make_unique<File>(fd, 0);
  }

 private:
  int fd;
  size_t file_size;

  size_t read_size() {
    struct ::stat file_stat;
    if (::fstat(fd, &file_stat) < 0) {
      throw std::system_error{errno, std::system_category()};
    }
    return file_stat.st_size;
  }
};