#include "core/solver.h"
#include "core/loader.h"
#include "core/datastore.h"
#include "lib/geometry.h"
#include "lib/picojson.h"

#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <sstream>

using namespace core;

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        if (!item.empty()) elems.push_back(item);
    }
    return elems;
}

void Loader::parseToFile(std::string filename) {
    std::ifstream ifs(filename);
    if (ifs.fail()) {
        std::cerr << filename << " failed to load" << std::endl;
        return;
    }
    std::istreambuf_iterator<char> it(ifs);
    std::istreambuf_iterator<char> last;

    std::string data(it, last);
    if (split(filename, '.').back() == "json") parseToJson(data);
    else parse(data);
}

void Loader::parse(std::string data) {
    auto datastore = parent()->getComponent<Datastore>();
    auto piece_list = split(data, ':');
    int  index = 0;
    std::cout << "check" << std::endl;

    datastore->count = std::stoi(piece_list[0]);

    std::cout <<  datastore->count << std::endl;
    for (; index < datastore->count; index++) {
        std::cout << piece_list[index + 1] << std::endl;
        auto piece = split(piece_list[index + 1], ' ');
        int  count = std::stoi(piece[0]) * 2;
        datastore->polygons[index].clear();
        for (int p = 1; p < count; p += 2) {
            datastore->polygons[index].add(std::stod(piece[p]), std::stod(piece[p + 1]));
        }
    }

    std::cout << "check" << std::endl;

    auto piece = split(piece_list[index + 1], ' ');
    int count = std::stoi(piece[0]) * 2;
    datastore->frame.clear();
    for (int p = 1; p < count; p += 2) {
        datastore->frame.add(std::stod(piece[p]), std::stod(piece[p + 1]));
    }
}

void Loader::parseToJson(std::string data) {
    auto datastore = parent()->getComponent<Datastore>();
}