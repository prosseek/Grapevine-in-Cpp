#include <stdexcept>

class Error : public std::runtime_error
{
  public:
    Error (const std::string &message)
      : std::runtime_error(message)
    {}
};

// int value()
// {
//     std::stringstream ss;
//     ss << "Issues at " << __FILE__ << " on line " << __LINE__;
//     throw Error(ss.str());
// }