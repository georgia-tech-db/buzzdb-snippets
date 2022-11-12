

#include <gtest/gtest.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <random>
#include <utility>
#include <vector>
#include <thread>

#include "utils/tutorial.h"

namespace {

TEST(TutorialTests, BasicTest) {

    auto course1 = buzzdb::utils::Course(1, "math");
    auto course2 = buzzdb::utils::Course(2, "science");
    auto course3 = buzzdb::utils::Course(3, "art");
    auto course4 = buzzdb::utils::Course(4, "music");
    auto course5 = buzzdb::utils::Course(5, "literature");

    auto student1 = buzzdb::utils::UndergradStudent(1, "alice", 3);
    auto student2 = buzzdb::utils::GraduateStudent(2, "rahul", 55);

    EXPECT_EQ(student1.add_course(course1), true);
    EXPECT_EQ(student1.add_course(course2), true);
    EXPECT_EQ(student1.add_course(course3), true);

    EXPECT_EQ(student1.add_course(course4), false);

    std::cout << student1;

    EXPECT_EQ(student2.add_course(course1), true);

    std::cout << student2;

}

TEST(TutorialTests, RWLatchTest) {
  EXPECT_EQ(2, 1 + 1);
  atomic<size_t> value = 0;
  vector<thread> threads;
  buzzdb::utils::RWLatch obj;
  size_t num_of_threads = 50;
  /*
  threads.emplace_back(
    [&value, &obj] {
      size_t result = obj.get();
      EXPECT_GE(result, value);
      value = result;
    }
  );

  threads.emplace_back(
    [&value, &obj] {
      size_t result = obj.increment();
      EXPECT_EQ(result, value + 1);
      value = result;
    }
  );

  threads.emplace_back(
    [&value, &obj] {
      size_t result = obj.increment();
      EXPECT_EQ(result, value + 1);
      value = result;
    }
  );
  */
  for (size_t i = 0; i < num_of_threads; ++i) {
    threads.emplace_back(
      [i, &value, &obj] {
        size_t result = 0;
        if (i % 3 == 0) {
          // writer
          result = obj.increment();
          EXPECT_EQ(result, value + 1);
        } else {
          // reader
          result = obj.get();
          EXPECT_GE(result, value);
        }
        value = result;
      }
    );
  }

  for (size_t i = 0; i < num_of_threads; ++i) {
    threads[i].join();
  }
}

}  // namespace

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

