/**
 * @file CheckCalcAtmosDensFromSpDataSet.cpp
 * @author fugu133
 * @brief 宇宙天気情報から大気密度を計算する
 * @version 0.1
 * @date 2024-01-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <GeoAtmos/Core.hpp>

using namespace geoatmos;

constexpr auto altitude_start_km = 0.0;
constexpr auto altitude_end_km = 600.0;

const auto dt = DateTime{"2023-12-31T00:00:00"};
const auto longitude = Degree{135};
const auto latitude = Degree{35};

int main() {
	auto ofs = std::ofstream{"atmos.csv"};
	auto sw_dataset = SpaceWeather{"SW-Last5Years.csv"};
	auto atmos = GeoAtmosDensity{DensityUnit::GramPerCm3, TemperatureUnit::Celsius};

	ofs << "Altitude [km], Density [g/cm^3], Temperature [deg C]" << std::endl;
	for (auto alt = altitude_start_km; alt <= altitude_end_km; alt += 1.0) {
		auto p = atmos(Wgs84{dt, longitude, latitude, alt * 1e3}, sw_dataset);
		ofs << alt << ", " << p.density.atmosphere << ", " << p.temperature.at_altitude << std::endl;
	}
	ofs.close();
}