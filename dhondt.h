#ifndef DHONDT_H
#define DHONDT_H
#include <boost/multi_array.hpp>

void dhondt(const vector<unsigned> &distribution,
    unsigned seats,
    boost::multi_array<pair<double, unsigned>, 2> &out,
    vector<unsigned> &result);
#endif
