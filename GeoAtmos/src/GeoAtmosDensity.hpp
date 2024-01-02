/**
 * @file GeoAtmosDensity.hpp
 * @author fugu133
 * @brief
 * @version 0.1
 * @date 2024-01-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Coordinate.hpp"
#include "Essential.hpp"
#include "GeoAtmosType.hpp"
#include "Nrlmsise00.hpp"
#include "SpaceWeather.hpp"

GEOATMOS_NAMESPACE_BEGIN

/**
 * @brief 地球大気モデル
 *
 */
class GeoAtmosDensity : private internal::Nrlmsise {
  public:
	GeoAtmosDensity() : Nrlmsise(), m_config() {}

	GeoAtmosDensity(DensityUnit unit) : GeoAtmosDensity() { configureDensityUnit(unit); }

	GeoAtmosDensity(const ModelConfig &config) : Nrlmsise(), m_config(config) {}

	AtmosphericParameters operator()(const Eci &pos, double f107_average, double f107_daily, const MagneticIndex &ap) {
		return operator()(pos.toWgs84(), f107_average, f107_daily, ap);
	}

	AtmosphericParameters operator()(const Eci &pos, double f107_average, double f107_daily, double ap) {
		return operator()(pos.toWgs84(), f107_average, f107_daily, ap);
	}

	AtmosphericParameters operator()(const Eci &pos, const SpaceWeather &db) { return operator()(pos.toWgs84(), db); }

	AtmosphericParameters operator()(const Ecef &pos, double f107_average, double f107_daily, const MagneticIndex &ap) {
		return operator()(pos.toWgs84(), f107_average, f107_daily, ap);
	}

	AtmosphericParameters operator()(const Ecef &pos, double f107_average, double f107_daily, double ap) {
		return operator()(pos.toWgs84(), f107_average, f107_daily, ap);
	}

	AtmosphericParameters operator()(const Ecef &pos, const SpaceWeather &db) { return operator()(pos.toWgs84(), db); }

	AtmosphericParameters operator()(const Wgs84 &pos, double f107_average, double f107_daily, const MagneticIndex &ap);

	AtmosphericParameters operator()(const Wgs84 &pos, double f107_average, double f107_daily, double ap);

	AtmosphericParameters operator()(const Wgs84 &pos, const SpaceWeather &db);

	void configureModel(const ModelConfig &config) { m_config = config; }

	void configureDensityUnit(DensityUnit unit) {
		switch (unit) {
			case DensityUnit::GramPerCm3: m_config.unit_conversion = SwitchStatus::Off; break;
			case DensityUnit::KgPerM3: m_config.unit_conversion = SwitchStatus::On; break;
			case DensityUnit::Cgs: m_config.unit_conversion = SwitchStatus::Off; break;
			case DensityUnit::Si: m_config.unit_conversion = SwitchStatus::On; break;
			default: break;
		}
	}

  private:
	ModelConfig m_config;
};

AtmosphericParameters GeoAtmosDensity::operator()(const Wgs84 &pos, double f107_average, double f107_daily, const MagneticIndex &ap) {
	internal::NrlmsiseInput input;
	internal::NrlmsiseOutput output;
	internal::NrlmsiseConfig config;

	{
		m_config.daily_ap = SwitchStatus::Specific;
		config = m_config.convertNativeStatus();
	}

	{
		input.alt = pos.altitude() * 1e-3; // m -> km
		input.g_lat = pos.latitude().degrees();
		input.g_long = pos.longitude().degrees();
		input.year = pos.epoch().year();
		input.doy = pos.epoch().dayOfYear();
		input.sec = pos.epoch().secondsOfDay();
		input.lst = input.sec / 3600.0 + input.g_long / 15.0; // TODO: まともに実装する
		input.f107A = f107_average;
		input.f107 = f107_daily;
		std::copy(ap.ap, ap.ap + std::size(ap.ap), input.ap_a.a);
	}

	gtd7(input, config, output);

	return AtmosphericParameters(
	  Density(output.d[6], output.d[0], output.d[7], output.d[1], output.d[4], output.d[2], output.d[3], output.d[8], output.d[5]),
	  Temperature(output.t[0], output.t[1]));
}

AtmosphericParameters GeoAtmosDensity::operator()(const Wgs84 &pos, double f107_average, double f107_daily, double ap) {
	internal::NrlmsiseInput input;
	internal::NrlmsiseOutput output;
	internal::NrlmsiseConfig config;

	{
		if (m_config.daily_ap == SwitchStatus::Specific) m_config.daily_ap = SwitchStatus::On;
		config = m_config.convertNativeStatus();
	}

	{
		input.alt = pos.altitude() * 1e-3; // m -> km
		input.g_lat = pos.latitude().degrees();
		input.g_long = pos.longitude().degrees();
		input.year = pos.epoch().year();
		input.doy = pos.epoch().dayOfYear();
		input.sec = pos.epoch().secondsOfDay();
		input.lst = input.sec / 3600.0 + input.g_long / 15.0; // TODO: まともに実装する
		input.f107A = f107_average;
		input.f107 = f107_daily;
		input.ap = ap;
	}

	gtd7(input, config, output);

	return AtmosphericParameters(
	  Density(output.d[6], output.d[0], output.d[7], output.d[1], output.d[4], output.d[2], output.d[3], output.d[8], output.d[5]),
	  Temperature(output.t[0], output.t[1]));
}

AtmosphericParameters GeoAtmosDensity::operator()(const Wgs84 &pos, const SpaceWeather &db) {
	const auto &dt = pos.epoch();

	double f107_average = 0;
	try {
		f107_average = db.adjustedF107Center81(dt);
	} catch (const std::out_of_range &e) {
		f107_average = 150;
	}

	double f107_daily = 0;
	try {
		f107_daily = db.adjustedF107(dt - Days(1));
	} catch (const std::out_of_range &e) {
		f107_daily = 150;
	}

	MagneticIndex ap{0, 0, 0, 0, 0, 0, 0};
	try {
		constexpr double time_step = -3.0;	  // -3h
		constexpr int avg_times = 8;		  // 8 times
		constexpr int m12_m33_start = 12 / 3; // -12h
		constexpr int m12_m33_end = 33 / 3;	  // -33h
		constexpr int m36_m57_start = 36 / 3; // -36h
		constexpr int m36_m57_end = 57 / 3;	  // -57h

		// Daily AP
		ap.ap[0] = db.dailyApIndex(dt);

		// 3h AP from 0h, -3h, -6h, -9h
		for (int i = 1; i < 5; i++) ap.ap[i] = db.apIndex(dt, Hours{time_step * (i - 1)});

		// Average 3h AP from -12h ~ -33h
		for (int i = m12_m33_start; i < m12_m33_end; i++) ap.ap[5] += (double)db.apIndex(dt, Hours{time_step * i});
		ap.ap[5] /= (double)avg_times;

		// Average 3h AP from -36h ~ -57h
		for (int i = m36_m57_start; i < m36_m57_end; i++) ap.ap[6] += (double)db.apIndex(dt, Hours{time_step * i});
		ap.ap[6] /= (double)avg_times;
	} catch (const std::out_of_range &e) {
		ap = MagneticIndex{};
	}

	return operator()(pos, f107_average, f107_daily, ap);
}
GEOATMOS_NAMESPACE_END