#include<string>

namespace buzzdb {
namespace utils {

class RunLengthEncoding {

  private:
    std::string s;

  public:

    RunLengthEncoding(std::string str){
        s = str;
    }

    std::string compress();
};
}  // namespace utils
}  // namespace buzzdb