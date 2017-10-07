
#pragma once
#include "picojson.h"
#include <fstream>

std::vector<std::vector<Position>> getFrameShape () {
        std::vector<std::vector<Position>>ret;
        std::ifstream fs;

        fs.open ("frame.json", std::ios::binary);
        assert (!fs.fail ());

        picojson::value val;
        fs >> val;	fs.close ();

        auto shapeList = val.get<picojson::object> ()["shapes"].get<picojson::array> ();

        for (auto itr = shapeList.begin (); itr != shapeList.end (); ++itr) {
                auto vertexList = itr->get<picojson::array> ();
                std::vector<Position> tmp;
                for (auto _itr = vertexList.begin (); _itr != vertexList.end (); ++_itr) {
                        int x = (int)_itr->get<picojson::object> ()["x"].get<double> ();
                        int y = (int)_itr->get<picojson::object> ()["y"].get<double> ();
                        tmp.push_back (Position (x, y));
                }
                ret.push_back (tmp);
        }
        return ret;
}

std::vector<std::vector<Position>> getPiecesShape () {
        std::vector<std::vector<Position>>ret;
        std::ifstream fs;

        fs.open ("piece.json", std::ios::binary);
        assert (!fs.fail ());

        picojson::value val;
        fs >> val;	fs.close ();


        auto pieceList = val.get<picojson::object> ()["pieces"].get<picojson::array> ();
        for (auto __itr = pieceList.begin (); __itr != pieceList.end (); ++__itr) {
                auto shapeList = __itr->get<picojson::object> ()["shapes"].get<picojson::array> ();
                for (auto itr = shapeList.begin (); itr != shapeList.end (); ++itr) {
                        std::vector<Position> tmp;
                        auto vertexList = itr->get<picojson::array> ();
                        for (auto _itr = vertexList.begin (); _itr != vertexList.end (); ++_itr) {
                                int x = (int)_itr->get<picojson::object> ()["x"].get<double> ();
                                int y = (int)_itr->get<picojson::object> ()["y"].get<double> ();
                                tmp.push_back (Position (x, y));
                        }
                        ret.push_back (tmp);
                }
        }
        return ret;
}

std::vector<std::vector<Position>> getLayout () {
        std::vector<std::vector<Position>>ret;
        std::ifstream fs;

        fs.open ("place.json", std::ios::binary);
        if (!fs.fail ())
                return ret;

        picojson::value val;
        fs >> val;	fs.close ();


        auto pieceList = val.get<picojson::object> ()["pieces"].get<picojson::array> ();
        for (auto __itr = pieceList.begin (); __itr != pieceList.end (); ++__itr) {
                auto shapeList = __itr->get<picojson::object> ()["shapes"].get<picojson::array> ();
                for (auto itr = shapeList.begin (); itr != shapeList.end (); ++itr) {
                        std::vector<Position> tmp;
                        auto vertexList = itr->get<picojson::array> ();
                        for (auto _itr = vertexList.begin (); _itr != vertexList.end (); ++_itr) {
                                int x = (int)_itr->get<picojson::object> ()["x"].get<double> ();
                                int y = (int)_itr->get<picojson::object> ()["y"].get<double> ();
                                tmp.push_back (Position (x, y));
                        }
                        ret.push_back (tmp);
                }
        }
        return ret;
}
