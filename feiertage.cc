/* Collection of date calculations related to the German special days.
 *
 * 2014-01-26, Georg Sauthoff <mail@georg.so>
 *
 * GPLv3+
 */
#include "feiertage.h"

#include <boost/date_time/gregorian/gregorian.hpp>
using namespace boost::gregorian;

#include <cassert>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <cstring>

using namespace std;

static const bool ftmap[static_cast<unsigned>(Feiertag::END)]
                       [static_cast<unsigned>(Bundesland::END)] =
{
   // BW   , BY    , BE    , BB    , HB    , HH    , HE    , MV    , NI    , NW    , RP    , SL    , SN    , ST    , SH    , TH       ,
   { true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  }  , // Neujahr
   { true  , true  , false , false , false , false , false , false , false , false , false , false , false , true  , false , false }  , // Drei_K
   { true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  }  , // Karfrei
   { true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  }  , // Osterm
   { true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  }  , // TdArbeit
   { true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  }  , // Himmelf
   { true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  }  , // Pfingstm
   { true  , true  , false , false , false , false , true  , false , false , true  , true  , true  , false , false , false , false }  , // Fronl
   { false , false , false , false , false , false , false , false , false , false , false , true  , false , false , false , false }  , // Mariae
   { true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  }  , // Einheit
   { false , false , false , true  , false , false , false , true  , false , false , false , false , true  , true  , false , true  }  , // Reform
   { true  , true  , false , false , false , false , false , false , false , true  , true  , true  , false , false , false , false }  , // Allerh
   { false , false , false , false , false , false , false , false , false , false , false , false , true  , false , false , false }  , // BBtag
   { true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  }  , // 1. Weih
   { true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  , true  }    // 2. Weih
};

static bool is_feiertag(Feiertag f, Bundesland b)
{
  unsigned b_u = static_cast<unsigned>(b);
  unsigned f_u = static_cast<unsigned>(f);
  assert(b_u < static_cast<unsigned>(Bundesland::END));
  assert(f_u < static_cast<unsigned>(Feiertag::END));
  return ftmap[f_u][b_u];
}

struct Fester_Tag {
  Feiertag feiertag;
  unsigned short month;
  unsigned short day;
};

static const Fester_Tag fester_tag[] = {
   { Feiertag::Neujahr         , 1  , 1  }  ,
   { Feiertag::Drei_Koenige    , 1  , 6  }  ,
   { Feiertag::Tag_der_Arbeit  , 5  , 1  }  ,
   { Feiertag::Mariae          , 8  , 15 }  ,
   { Feiertag::Einheit         , 10 , 3  }  ,
   { Feiertag::Reformationstag , 10 , 31 }  ,
   { Feiertag::Allerheiligen   , 11 , 1  }  ,
   { Feiertag::Weihnachtstag_1 , 12 , 25 }  ,
   { Feiertag::Weihnachtstag_1 , 12 , 26 }  ,
   { Feiertag::END , 0 , 0 }
};

/*
namespace std {
  template <>
    struct hash<date> {
      size_t operator ()(const date &d) const { return std::hash<string>()(to_iso_string(d)); }
    };
}
*/

date easter_sunday(boost::gregorian::greg_year year)
{
  unsigned x = year;
  // http://de.wikipedia.org/wiki/Gau%C3%9Fsche_Osterformel#Eine_erg.C3.A4nzte_Osterformel
  // 1. die Säkularzahl:                                       K(X) = X div 100
  unsigned k = x / 100;
  // 2. die säkulare Mondschaltung:                            M(K) = 15 + (3K + 3) div 4 − (8K + 13) div 25
  unsigned m = 15 + (3 * k + 3) / 4 - (8 * k + 13) / 25;
  // 3. die säkulare Sonnenschaltung:                          S(K) = 2 − (3K + 3) div 4
  unsigned s = 2 - (3 * k + 3) / 4;
  // 4. den Mondparameter:                                     A(X) = X mod 19
  unsigned a = x % 19;
  // 5. den Keim für den ersten Vollmond im Frühling:        D(A,M) = (19A + M) mod 30
  unsigned d = (19*a + m) % 30;
  // 6. die kalendarische Korrekturgröße:                    R(D,A) = (D + A div 11) div 29[12]
  unsigned r = (d + a / 11) / 29;
  // 7. die Ostergrenze:                                    OG(D,R) = 21 + D − R
  unsigned og = 21 + d - r;
  // 8. den ersten Sonntag im März:                         SZ(X,S) = 7 − (X + X div 4 + S) mod 7
  unsigned sz = 7 - (x + x / 4 + s) % 7;
  // 9. die Entfernung des Ostersonntags von der
  // Ostergrenze (Osterentfernung in Tagen):           OE(OG,SZ) = 7 − (OG − SZ) mod 7
  unsigned oe = 7 - (og - sz) % 7;
  // 10. das Datum des Ostersonntags als Märzdatum
  // (32. März = 1. April usw.):                              OS = OG + OE
  unsigned os = og + oe;

  date z(year, 3, 1);
  date result(z+days(os-1));
  return result;
}


void feiertage(boost::gregorian::greg_year year, Bundesland bundesland,
    std::set<boost::gregorian::date> &dates)
{
  for (const Fester_Tag *ft = fester_tag; ft->feiertag != Feiertag::END; ++ft) {
    if (!is_feiertag(ft->feiertag, bundesland))
      continue;
    date d(year, ft->month, ft->day);
    //cout << "fester: " << d << '\n';;
    dates.insert(move(d));
  }
  date es(easter_sunday(year));
  //cout << "Ostern: " << es << '\n';;
  if (is_feiertag(Feiertag::Ostermontag, bundesland)) {
    dates.insert(es + days(1));
  }
  if (is_feiertag(Feiertag::Himmelfahrt, bundesland)) {
    dates.insert(es + days(39));
  }
  if (is_feiertag(Feiertag::Pfingstmontag, bundesland)) {
    dates.insert(es + days(50));
  }
  if (is_feiertag(Feiertag::Fronleichnam, bundesland)) {
    dates.insert(es + days(60));
  }
  if (is_feiertag(Feiertag::BBtag, bundesland)) {
    date ref(year, 11, 23);
    date r(previous_weekday(ref, greg_weekday(Wednesday)));
    //cout << "bbtag: " << r << '\n';
    dates.insert(r);
  }
}

bool is_work_day(const boost::gregorian::date &d,
    const std::set<boost::gregorian::date> &feiertage)
{
  if (d.day_of_week() == Sunday || d.day_of_week() == Saturday)
    return false;
  auto i = feiertage.find(d);
  return i == feiertage.end();
}

boost::gregorian::date previous_work_day(const boost::gregorian::date &d,
    const std::set<boost::gregorian::date> &feiertage)
{
  day_iterator i(d);
  for ( ; !is_work_day(*i, feiertage); --i)
    ;
  return *i;
}

boost::gregorian::date next_work_day(const boost::gregorian::date &d,
    const std::set<boost::gregorian::date> &feiertage)
{
  day_iterator i(d);
  for ( ; !is_work_day(*i, feiertage); ++i)
    ;
  return *i;
}

static const char *str_map_bundesland[] = {
  "bw",
  "by",
  "be",
  "bb",
  "hb",
  "hh",
  "he",
  "mv",
  "ni",
  "nw",
  "rp",
  "sl",
  "sn",
  "st",
  "sh",
  "th",
  0
};

Bundesland str_to_bundesland(const std::string &s)
{
  for (unsigned i = 0; i < static_cast<unsigned>(Bundesland::END); ++i)
    if (!strcmp(str_map_bundesland[i], s.c_str()))
      return Bundesland(i);
  throw range_error("unbekanntes bundesland - try bw, by, be ...");
}

