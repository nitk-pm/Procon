#ifndef LOADER__H
#define LOADER__H

#include "core/component.h"

#include <string>

namespace core {

class Loader : public Component {
public:
    void parseToFile(std::string filename);
    void parse(std::string data);
    void parseToJson(std::string data);
};

} // namespace core

#endif // LOADER__H