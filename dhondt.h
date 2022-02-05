#ifndef DHONDT_H
#define DHONDT_H

// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: © 2014 Georg Sauthoff <mail@gms.tf>

#include <utility>
#include <vector>
#include <boost/multi_array.hpp>

void dhondt(const std::vector<unsigned> &distribution,
    unsigned seats,
    boost::multi_array<std::pair<double, unsigned>, 2> &mass_rank,
    std::vector<unsigned> &result
    );

#endif
