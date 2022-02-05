// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: © 2014 Georg Sauthoff <mail@gms.tf>

#include "mitglieder.h"

#include <cassert>
#include <array>
using namespace std;


// http://www.gesetze-im-internet.de/betrvg/__9.html

static const array<unsigned, 2> wb_limit = {
// 5 bis 20 wahlberechtigten Arbeitnehmern aus einer Person,
  20,
//21 bis 50 wahlberechtigten Arbeitnehmern aus 3 Mitgliedern,
  50
};

static const array<unsigned , 16>  an_limit = {
//51 wahlberechtigten Arbeitnehmern
// bis 100 Arbeitnehmern aus 5 Mitgliedern,
100,
//101 bis 200 Arbeitnehmern aus 7 Mitgliedern,
200,
//201 bis 400 Arbeitnehmern aus 9 Mitgliedern,
400,
//401 bis 700 Arbeitnehmern aus 11 Mitgliedern,
700,
//701 bis 1.000 Arbeitnehmern aus 13 Mitgliedern,
1000,
//1.001 bis 1.500 Arbeitnehmern aus 15 Mitgliedern,
1500,
//1.501 bis 2.000 Arbeitnehmern aus 17 Mitgliedern,
2000,
//2.001 bis 2.500 Arbeitnehmern aus 19 Mitgliedern,
2500,
//2.501 bis 3.000 Arbeitnehmern aus 21 Mitgliedern,
3000,
//3.001 bis 3.500 Arbeitnehmern aus 23 Mitgliedern,
3500,
//3.501 bis 4.000 Arbeitnehmern aus 25 Mitgliedern,
4000,
//4.001 bis 4.500 Arbeitnehmern aus 27 Mitgliedern,
4500,
//4.501 bis 5.000 Arbeitnehmern aus 29 Mitgliedern,
5000,
//5.001 bis 6.000 Arbeitnehmern aus 31 Mitgliedern,
6000,
//6.001 bis 7.000 Arbeitnehmern aus 33 Mitgliedern,
7000,
//7.001 bis 9.000 Arbeitnehmern aus 35 Mitgliedern.
9000
};


unsigned br_size(unsigned wahlberechtigte, unsigned arbeitnehmer)
{
  if (wahlberechtigte < 5)
    return 0;

  // In Betrieben mit mehr als 9.000 Arbeitnehmern erhöht sich die Zahl der Mitglieder des Betriebsrats für je angefangene weitere 3.000 Arbeitnehmer um 2 Mitglieder.
  if (arbeitnehmer > 9000) {
    unsigned seats = 35;
    unsigned rest = arbeitnehmer - 9000;
    seats += rest/3000 * 2 + (rest%3000>0)*2;
    return seats;
  }

  unsigned seats = 1;
  for (auto l : wb_limit) {
    if (wahlberechtigte <= l)
      return seats;
    else
      seats += 2;
  }
  for (auto l : an_limit) {
    if (arbeitnehmer <= l)
      return seats;
    else
      seats += 2;
  }
  assert(0);
  return 0;
}
