#ifndef DATA_STORE__H
#define DATA_STORE__H

#include <vector>
#include <string>

#include "core/component.h"
#include "lib/geometry.h"

namespace components {

class DataStore : public core::Component {
public:
    DataStore(std::string &data);

    void parse(std::string &data);
    void parseJson(std::string &data);

private:
    geometry::Polygon board;
    std::vector<geometry::Polygon> piece_list;
};

} // namespace components

#endif // DATA_STORE__H