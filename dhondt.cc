// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: © 2014 Georg Sauthoff <mail@gms.tf>

#include "dhondt.h"

#include <algorithm>
#include <random>
#include <array>
#include <vector>
#include <utility>
#include <ctime>
#include <boost/multi_array.hpp>

/* Not really needed with C++11 STL ...

#include <boost/range/algorithm/random_shuffle.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/range/iterator_range.hpp>

  boost::mt19937 engine(static_cast<uint32_t>(std::time(0)));
  uniform_int<> dist(0, t.size()+1);
  variate_generator<boost::mt19937&, uniform_int<> > gen(engine, dist);
  random_shuffle(eq, gen);
*/

using namespace std;
using namespace boost;


  /* below function object as lambda
    [&a](const pair<unsigned, unsigned> &x, const pair<unsigned, unsigned> &y)
        {
          return a[x.first][x.second].first > a[y.first][y.second].first;
        }
        );
        */

struct Cmp {
  const multi_array<pair<double, unsigned>, 2> &a;
  Cmp(const multi_array<pair<double, unsigned>, 2> &a)
    : a(a)
  {
  }
  bool operator()(const pair<unsigned, unsigned> &x, const pair<unsigned, unsigned> &y) const
  {
    return a[x.first][x.second].first > a[y.first][y.second].first;
  }
};

// 'Losentscheid'
static void shuffle_tail(
  vector<pair<unsigned, unsigned> > &t,
  const multi_array<pair<double, unsigned>, 2> &a,
  unsigned seats)
{
  if (!seats)
    return;
  if (seats > t.size())
    return;
  auto eq = equal_range(t.begin(), t.end(), t[seats-1], Cmp(a));
  std::mt19937 g(static_cast<uint32_t>(std::time(0)));
  std::shuffle(eq.first, eq.second, g);
}

// http://de.wikipedia.org/wiki/D%E2%80%99Hondt-Verfahren
void dhondt(const vector<unsigned> &distribution,
    unsigned seats,
    multi_array<pair<double, unsigned>, 2> &out,
    vector<unsigned> &result)
{
  const std::array<size_t, 2> dims {seats, distribution.size()};
  multi_array<pair<double, unsigned>, 2> a(
      //extents[seats][distribution.size()]);
      dims);
  vector<pair<unsigned, unsigned> > t;
  for (unsigned i = 0; i < seats; ++i)
    for (unsigned j = 0; j < distribution.size(); ++j) {
      a[i][j].first = distribution[j]/double(i+1);
      t.push_back(make_pair(i, j));
    }

  sort(t.begin(), t.end(), Cmp(a));

  shuffle_tail(t, a, seats);

  result.resize(distribution.size());
  for (unsigned i = 0; i < seats && i < t.size(); ++i) {
    a[t[i].first][t[i].second].second = i;
    ++result[t[i].second];
  }
  for (unsigned i = seats; i < t.size(); ++i)
    a[t[i].first][t[i].second].second = i;
  out.resize(dims);
  out = std::move(a);
}
