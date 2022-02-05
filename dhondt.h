#ifndef DHONDT_H
#define DHONDT_H

#include <utility>
#include <boost/multi_array.hpp>

void dhondt(const vector<unsigned> &distribution,
    unsigned seats,
    boost::multi_array<std::pair<double, unsigned>, 2> &mass_rank,
    vector<unsigned> &result
    );

#endif
