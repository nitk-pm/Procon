#include "core/solver.h"
#include "core/datastore.h"
#include "core/loader.h"

#include <iostream>
using namespace std;
using namespace core;

int main(void) {
    Solver solver;

    auto datastore = solver.addComponent<Datastore>();
    auto loader    = solver.addComponent<Loader>();

    loader->parseToFile("sample.txt");

    cout << "number of piece: " << datastore->count << endl;
    for (int i = 0; i < datastore->count; i++) {
        auto &polygon = datastore->polygons[i];
        cout << "piece" << i << endl;
        for (int j = 0; j < polygon.size(); j++) {
            cout << polygon[j].x() << " " << polygon[j].y() << " ";
        }
        cout << endl;
    }
}
