#ifndef FEIERTAGE_H
#define FEIERTAGE_H

// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: © 2014 Georg Sauthoff <mail@gms.tf>

//#include <unordered_set>
#include <set>
#include <string>
#include <boost/date_time/gregorian/gregorian_types.hpp>

enum class Bundesland
{
  BW, // Baden-Württemberg
  BY, // Bayern
  BE, // Berlin
  BB, // Brandenburg
  HB, // Bremen
  HH, // Hamburg
  HE, // Hessen
  MV, // Mecklenburg-Vorpommern
  NI, // Niedersachen
  NW, // Nordrhein-Westfalen
  RP, // Rheinland-Pfalz
  SL, // Saarland
  SN, // Sachsen
  ST, // Sachsen-Anhalt
  SH, // Schleswig-Holstein
  TH,  // Thüringen
  END
};

Bundesland str_to_bundesland(const std::string &s);

enum class Feiertag
{
  Neujahr,
  Drei_Koenige,
  Karfreitag,
  Ostermontag,
  Tag_der_Arbeit,
  Himmelfahrt,
  Pfingstmontag,
  Fronleichnam,
  Mariae,
  Einheit,
  Reformationstag,
  Allerheiligen,
  BBtag,
  Weihnachtstag_1,
  Weihnachtstag_2,
  END
};

void feiertage(boost::gregorian::greg_year year, Bundesland bundesland,
    std::set<boost::gregorian::date> &dates);

bool is_work_day(const boost::gregorian::date &d,
    const std::set<boost::gregorian::date> &feiertage);

boost::gregorian::date previous_work_day(const boost::gregorian::date &d,
    const std::set<boost::gregorian::date> &feiertage);
boost::gregorian::date next_work_day(const boost::gregorian::date &d,
    const std::set<boost::gregorian::date> &feiertage);

#endif
