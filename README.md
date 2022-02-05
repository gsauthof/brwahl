Collection of date calculations related to the German Betriebsratswahl.

(the English translation of 'Betriebsratswahl': works council election)

The `feiertage` translation unit contains some generic helper functions for
date calculations that involve German special days (work holidays, days of
observance). This is not an entirely trivial area, because 1) there are moving
special days and 2) which set of special days are used is governed by state law
(the Federal Republic of Germany consists of 16 states). Most moving special
days can be computed using an offset to the [easter date][ed]. Thus, there is
an implementation of an easter formula as well (cf. `easter_sunday()`).

For the Betriebsratswahl, some important deadlines can be computed
backwards from the chosen day of the election. For most computations
special days need to be considered, since a deadline must not end on
a special day.

Apart from the Betriebsratswahl topic, the code might be of interest as real
world example to show how to use the well-designed [Boost Date Time
Library][dt] and the [Boost Program Options Library][po]. The option parsing
code uses Program Options library features that are often overlooked or not
included in toy examples (and/or are not really extensively documented).

Since the topic is inherently a German one, some terms and
help descriptions are not translated.

## Dependencies

- C++11 compiler (e.g. [GCC][gcc] 4.8)
- [CMake][cm]
- the [Boost Library][bo] (especially Boost Date Time)

## Build Instructions

    mkdir build
    cd build
    cmake ..
    make

## License

[GPLv3+][gpl]

## Contact

Don't hesitate to send me comments and other feedback via email:

    Georg Sauthoff <mail@georg.so>

[dt]:  http://www.boost.org/doc/libs/1_55_0/doc/html/date_time.html
[po]:  http://www.boost.org/doc/libs/1_55_0/doc/html/program_options.html
[ed]:  http://en.wikipedia.org/wiki/Easter
[cm]:  http://www.cmake.org/
[bo]:  http://www.boost.org/
[gpl]: http://www.gnu.org/licenses/gpl.html
[gcc]: http://gcc.gnu.org/


