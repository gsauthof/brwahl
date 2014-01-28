/* Collection of date calculations related to the German
 * Betriebsratswahlgesetz.
 *
 * 2014-01-26, Georg Sauthoff <mail@georg.so>
 *
 * GPLv3+
 */

#include <iostream>
#include <string>
#include <utility>
using namespace std;

// currently not needed:
// #include <boost/date_time/posix_time/posix_time.hpp>
// using namespace boost::posix_time;
// boost::posix_time::second_clock::local_time()
// boost::posix_time::ptime
// boost::posix_time::time_period period(start, now);
// long secs = period.length().total_seconds();

#include <boost/date_time/gregorian/gregorian.hpp>
// no i/o
//#include <boost/date_time/gregorian/gregorian_types.hpp>
using namespace boost::gregorian;

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "feiertage.h"

namespace OPT {
// work around option display bug:
// https://svn.boost.org/trac/boost/ticket/7256
//static const char BR_END[] = "br-end";
static const char BR_END[] = "brend";
static const char BR_BEGIN[] = "brbegin";
static const char WAHLTAG[] = "wahltag";
static const char WORK_DAY[] = "workday";
static const char WAHLAUSSCHREIBEN[] = "wahlausschreiben";
static const char LAND[] = "land";
static const char FEIERTAG[] = "ft";
static const char SHOW_FEIERTAG[] = "showft";
static const char HELP_S[] = "help,h";
static const char HELP[] = "help";
}

static ostream &operator<<(ostream &o, const date &d)
{
  o << to_iso_extended_string(d);
  if (!d.is_not_a_date())
    o << "_(" << d.day_of_week() << ")";
  return o;
}


// overloaded validate function must be in the same namespace
// as the class ...
namespace boost {
  namespace gregorian {

    void validate(boost::any &v, 
        const std::vector<std::string> &values,
        date*, int)
    {
      using namespace boost::program_options;
      validators::check_first_occurrence(v);
      const string &s = validators::get_single_string(values);
      try {
        v = boost::any(from_string(s));
      } catch (boost::bad_lexical_cast) {
        throw validation_error(validation_error::invalid_option_value);
      }
    }

  }
}

void validate(boost::any &v, 
    const std::vector<std::string> &values,
    Bundesland*, int)
{
  using namespace boost::program_options;
  validators::check_first_occurrence(v);
  const string &s = validators::get_single_string(values);
  try {
    v = boost::any(str_to_bundesland(s));
  } catch (range_error) {
    throw validation_error(validation_error::invalid_option_value);
  }
}


struct Options {
  date br_end;
  date br_begin;
  date wahltag;
  date wahlausschreiben;
  date work_day;
  std::set<boost::gregorian::date> sd;
  Bundesland land;
  vector<date> add_feiertag;
  bool show_feiertag { false };

  ostream &print(ostream &o) const
  {
    o << "Betriebsratbegin: " << br_begin << '\n'
      << "Betriebsratende:  " << br_end << '\n'
      << "Wahltag:          " << wahltag << '\n'
      << "Wahlauschreiben:  " << wahlausschreiben << '\n'
      ;
    return o;
  }

  void parse(int argc, char **argv)
  {
    po::options_description desc("Options");
    desc.add_options()
      (OPT::HELP_S, "this help screen")
      (OPT::BR_END, po::value<date>(&br_end)->value_name("YYYY-MM-DD"),
       "Datum Ablauf der Amtszeit des alten Betriebsrats")
      (OPT::BR_BEGIN, po::value<date>(&br_begin)->value_name("YYYY-MM-DD"),
       "Datum Beginn Amtszeit des alten Betriebsrats")
      (OPT::WAHLTAG, po::value<date>(&wahltag)->value_name("YYYY-MM-DD"),
       "Tag der Wahl")
      (OPT::WAHLAUSSCHREIBEN, po::value<date>(&wahlausschreiben)
       ->value_name("YYYY-MM-DD"),
       "Tag Aushang Wahlausschreiben")
      (OPT::WORK_DAY, po::value<date>(&work_day)->value_name("YYYY-MM-DD"),
       "Tested ob Datum ein Arbeitstag ist")
      (OPT::LAND, po::value<Bundesland>(&land)
       ->default_value(Bundesland::HE, "he(ssen)"),
       "Bundesland (by, he, nw, ...)")
      (OPT::FEIERTAG, po::value<vector<date> >(&add_feiertag)
       ->value_name("YYYY-MM-DD"),
       "weitere Feiertage")
      (OPT::SHOW_FEIERTAG,
       po::value<bool>(&show_feiertag)->default_value(false, "false")
       ->implicit_value(true, "true")->value_name("bool"),
       "print Feiertage");
      ;
    po::positional_options_description pdesc;
    // we want to error out on accidentally specified positional
    // options
    po::variables_map vm;
    //po::store(po::parse_command_line(argc, argv, desc), vm);
    po::store(po::command_line_parser(argc, argv)
        .options(desc).positional(pdesc).run(), vm);
    po::notify(vm);    
    if (vm.count(OPT::HELP)) {
      cout << desc << "\n";
      exit(0);
    }
    for (auto &i : add_feiertag)
        sd.insert(i);

    // if you don't want to overload the validate() function
    // you have to write boilerplate code like this ...
    // if (vm.count(OPT::BR_END))
    //   br_end = std::move(from_string(vm[OPT::BR_END].as<string>()));

  }
};


ostream &operator<<(ostream &o, const Options &opts)
{
  return opts.print(o);
}

static void ende_br(ostream &o, Options &opts)
{
  o << "Amtszeit des alten Betriebsrat endet am: ";
  if (opts.br_begin.is_not_a_date()) {
    o << "--" << OPT::BR_BEGIN << " not specified\n";
    return;
  }
  date d(opts.br_begin + years(4));
  o << d << '\n';;
  opts.br_end = d;
}

ostream &pp_move_prev(ostream &o, const date &r, const set<date> &sd)
{
  date c(previous_work_day(r, sd));
  if (c == r)
    o << r;
  else
    o << c << " (MOVED from " << r << ')';
  return o;
}

ostream &pp_move_next(ostream &o, const date &r, const set<date> &sd)
{
  date c(next_work_day(r, sd));
  if (c == r)
    o << r;
  else
    o << c << " (MOVED from " << r << ')';
  return o;
}

static void einsetzung_wahlvorstand(ostream &o, const Options &opts)
{
  date d(opts.br_end);
  if (d.is_not_a_date()) {
    o << "Einsetzung des Wahlvorstands spätestens am: ";
    o << "--" << OPT::BR_END << " not specified\n";
    return;
  }
  date r(d-weeks(10)-days(1));
  o << "Einsetzung des Wahlvorstands spätestens am: ";
  pp_move_prev(o, r, opts.sd);
  o  << '\n';
}

static void wahlausschreiben(ostream &o, const Options &opts)
{
  date d(opts.wahltag);
  o << "Aushang Wahlausschreiben spätestens am: ";
  if (d.is_not_a_date()) {
    o << "--wahltag not specified\n";
    return;
  }
  date r(d-weeks(6)-days(1));
  pp_move_prev(o, r, opts.sd);
  o  << '\n';
}

static void wahlvorschlaege_bekanntmachung(ostream &o, const Options &opts)
{
  date d(opts.wahltag);
  o << "Bekanntmachung Wahlvorschläge spätestens am: ";
  if (d.is_not_a_date()) {
    o << "--wahltag not specified\n";
    return;
  }
  date r(d-weeks(1)-days(1));
  pp_move_prev(o, r, opts.sd);
  o  << '\n';
}

static void konstituierende(ostream &o, const Options &opts)
{
  date d(opts.wahltag);
  o << "Konstituierende Sitzung (Amtszeitbeginn) des neuen Betriebsrats "
    "spätestens am: ";
  if (d.is_not_a_date()) {
    o << "--wahltag not specified\n";
    return;
  }
  date r(d+weeks(1)-days(1));
  pp_move_next(o, r, opts.sd);
  o  << '\n';
}

static void wahlvorschlaege(ostream &o, const Options &opts)
{
  date d(opts.wahlausschreiben);
  o << "Annahme von Wahlvorschlägen/Einspruch-Wählerliste vom " << d
    << " bis spätestens am: ";
  if (d.is_not_a_date()) {
    o << "--wahlausschreiben not specified\n";
    return;
  }
  date r(d+weeks(2));
  pp_move_next(o, r, opts.sd);
  o  << '\n';
}

static void test_work_day(ostream &o, const Options &opts)
{
  date d(opts.work_day);
  if (d.is_not_a_date())
    return;
  o << d << " ist ";
  bool b = is_work_day(d, opts.sd);
  if (b)
    o << "ein";
  else
    o << "KEIN";
  o << " Arbeitstag.\n";
}

static void print_feiertage(ostream &o, const Options &opts)
{
  if (!opts.show_feiertag)
    return;
  o << "Berücksichtigte Feiertage:\n";
  for (auto &i : opts.sd) {
    o << i << '\n';
  }
  o << '\n';
}

int main(int argc, char **argv)
{
  Options opts;
  opts.parse(argc, argv);
  date today(day_clock::local_day());
  feiertage(today.year(), opts.land, opts.sd);
  feiertage((today - years(1)).year(), opts.land, opts.sd);
  print_feiertage(cout, opts);
  cout << "Options:\n\n" << opts << "\n";
  test_work_day(cout, opts);
  ende_br(cout, opts);
  einsetzung_wahlvorstand(cout, opts);
  wahlausschreiben(cout, opts);
  wahlvorschlaege(cout, opts);
  wahlvorschlaege_bekanntmachung(cout, opts);
  konstituierende(cout, opts);

  return 0;
}
