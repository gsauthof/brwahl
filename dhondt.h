#ifndef DHONDT_H
#define DHONDT_H

#include <utility>
#include <vector>
#include <boost/multi_array.hpp>

void dhondt(const std::vector<unsigned> &distribution,
    unsigned seats,
    boost::multi_array<std::pair<double, unsigned>, 2> &mass_rank,
    std::vector<unsigned> &result
    );

#endif
