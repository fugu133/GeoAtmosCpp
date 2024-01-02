# GeoAtmosCpp

Calculate LEO atmospheric mass density by the NRLMSISE-00 model.

# Quick Start

```C++
#include <GeoAtmos/Core.hpp>
using namespace geoatmos;

int main() {
    // Define the position of the satellite.
    auto position = Wgs84{DateTime::now(), Degree{135}, Degree{35}, 500e3};

    // Calculate the atmospheric mass density.
    auto atmos = GeoAtmosDensity{}(position, 150, 150, MagneticIndex{});

    // Output the result to the console.
    std::cout << "At: " << position << std::endl;
    std::cout << "  Atmospheric mass density: " << atmos.density.atmosphere << " g/cm3" << std::endl;
    atd::cout << "  Temperature at altitude:  " << atmos.temperature.at_altitude << " K" << std::endl;
}
```

# How to use

## 1. Import this library into your project

All functions are available by including `Core.hpp`.

```C++
#include <GeoAtmos/Core.hpp>
```

All APIs are in the `geoatmos` namespace.

## 2. Time Definition

Time operations are performed in the `DateTime` class.
Only ISO-8601 Extended format (`YYYYY-MM-DDThh:mm:ss.ssss±hh:mm`) is supported as input format.
However, the following degrees of freedom are available.

1. If only `YYYYY-MM-DD` is used, it is interpreted as `YYYYY-MM-DDT00:00:00`.
1. Any single letter may be used for `T` in `<date>T<time>`.
1. There is no restriction on the length of the decimal part `.ssss` only the integer part `ss` is acceptable.
1. If time zone `±hh:mm` is not specified or `Z` is used, it is treated as UTC+0.

```C++
DateTime dt("2000-02-20T02:20:00.00+09:00");
```

### 2.1 Convert time system

Conversion of major time formats is performed as follows.
For example, UNIX time, Julian date, and modified Julian date (based on JD2000.0) are converted as follows

```C++
DateTime dt("2023-12-03T00:00:00");

std::cout << "UNIX" << dt.unixTime() << std::endl;
std::cout << "JD: " << dt.julianDay()  << std::endl;
std::cout << "MJD(JD2000.0): " << dt.j2000()  << std::endl;
```

Other conversions to Greenwich sidereal time, local sidereal time, and earth time (predicted) are also available.

### 2.2 Arithmetic evaluation

Arithmetic operations on time are performed using the dedicated `addXXX` member function or the `TimeSpan` class.
Standard arithmetic, relational, and equality operators are supported.

```C++
DateTime dt("2023-12-03T00:00:00");

std::cout << dt.addMinutes(1) << std::endl;
std::cout << dt + TimeSpan(1, TimeUnit::Minutes) << std::endl;
std::cout << dt <= DateTime("2024-12-03T00:00:00") << std::endl;
```

## 3 Angle Definition

Angles are represented by the `Angle` class.
This class holds the normalized angles and provides smooth conversion to individual angle units.

Angles can be expressed in arc radians, degrees, hours, arcminutes, and arcseconds.
HMS and DMS formats, which are commonly used in astronomy, are also supported.

```C++
Angle ang_deg = Degree(123.456);
Angle ang_rad = Radian(2.1547136813);
Angle ang_hms = Hms(8, 13, 49.44);
Angle ang_dms = Dms(123, 27, 21.6);
```

### 3.1 Convert angle unit

Angle unit conversion calls a dedicated member.

```C++
Angle ang = Degree(123.456);

std::cout << ang.radians() << std::endl;
std::cout << ang.arcmins() << std::endl;
```

### 3.2 Arithmetic evaluation

Standard arithmetic, relational, and equality operators are supported.
Trigonometric functions can be used without regard to angular units.

```C++
std::cout << Dms(124, 27, 21.6) - Degree(1) << std::endl;
std::cout << Degree(90).sin() << std::endl;
std::cout << Angle::acos(0.5).degrees() << std::endl;
```

### 4. Space Weather Information

Space weather information is provided by the `SpaceWeather` class.
Space weather data must be downloaded in advance.
Data must be entered in CSV format and can be downloaded from CelesTrak EOP and Space Weather Data.
You can also run the included DlSwDataset.py.

The AP index corresponding to the time information and the F10.7 solar radio flux can be obtained.

```C++
SpaceWeather sw("SW-Last5Years.csv");
DateTime dt("2023-12-31T00:00:00")

std::cout << sw.apIndex(dt) << std::endl;            // AP index at 2023-12-31T00:00:00
std::cout << sw.apIndex(dt, Hours{-3}) << std::endl; // AP index at 2023-12-30T21:00:00
std::cout << sw.adjustedF107(dt) << std::endl;       // F10.7 adjusted at 1 AU 2023-12-31
```

### 5. Atmospheric density and temperature

The `GeoAtmosDensity` class allows the NRLMSISE-00 model to easily calculate the atmospheric density and temperature at any location, and each parameter can be obtained with the `AtmosphericParameters` type.
Parameters such as magnetic activity and solar activity can be conveniently set from space weather information.

```C++
SpaceWeather sw("SW-Last5Years.csv");
Wgs84 position(DateTime::now(), Degree{135}, Degree{35}, 500e3);
GeoAtmosDensity atmos_dens();

// Calculate the atmospheric mass density.
auto p = atmos_dens(position, sw);
std::cout << p.density.atmosphere << std::endl; // Atmospheric mass density [g/cm3]
```

Other parameters that can be obtained include.

```C++
// Atomic density [cm-3]
std::cout << p.density.atomic_hydrogen << std::endl; // Atomic hydrogen
std::cout << p.density.atomic_helium << std::endl; // Atomic helium
std::cout << p.density.atomic_nitrogen << std::endl; // Molecular nitrogen
std::cout << p.density.atomic_oxygen << std::endl; // Molecular oxygen
std::cout << p.density.molecular_nitrogen << std::endl; // Molecular nitrogen
std::cout << p.density.molecular_oxygen << std::endl; // Molecular oxygen
std::cout << p.density.anomalous_oxygen << std::endl; // Anomalous oxygen

// Temperature [K]
std::cout << p.temperature.at_altitude << std::endl; // Temperature at altitude
std::cout << p.temperature.at_exospheric << std::endl; // Temperature at exospheric
```

# Reference

1. [Picone, J. M., et al. "NRLMSISE‐00 empirical model of the atmosphere: Statistical comparisons and scientific issues." Journal of Geophysical Research: Space Physics 107.A12 (2002): SIA-15.](https://agupubs.onlinelibrary.wiley.com/doi/full/10.1029/2002JA009430)
1. [Fortran code of NRLMSISE-00](https://ccmc.gsfc.nasa.gov/models/NRLMSIS~00/)

# SpecialThanks

Some linear algebra operations use [`Eigen`](https://eigen.tuxfamily.org/index.php?title=Main_Page).
