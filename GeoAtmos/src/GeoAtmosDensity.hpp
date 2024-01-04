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

#include <limits>

#include "Coordinate.hpp"
#include "Essential.hpp"
#include "GeoAtmosType.hpp"
#include "Nrlmsise00.hpp"
#include "SpaceWeather.hpp"
GEOATMOS_NAMESPACE_BEGIN

namespace internal {
	template <class T>
	struct HasToWgs84 {
	  private:
		template <class U>
		static auto test(int) -> decltype(std::declval<U>().toWgs84(), std::true_type());

		template <class U>
		static auto test(...) -> decltype(std::false_type());

	  public:
		using type = decltype(test<T>(0));
		static constexpr bool value = type::value;
	};
} // namespace internal
/**
 * @brief 地球大気モデル
 *
 */
class GeoAtmosDensity : private internal::Nrlmsise {
  public:
	GeoAtmosDensity() : m_config(), m_nv_input(), m_nv_output(), m_nv_config(), temperature_offset(0) {}

	GeoAtmosDensity(DensityUnit d_unit, TemperatureUnit t_unit) : GeoAtmosDensity() { configureOutputUnit(d_unit, t_unit); }

	GeoAtmosDensity(const ModelConfig &config) : GeoAtmosDensity() { m_config = config; }

	template <class T>
	auto operator()(const T &pos, double f107_average, double f107_daily, double ap) ->
	  typename std::enable_if_t<internal::HasToWgs84<T>::value, AtmosphericParameters> {
		return gtd7Interface(pos.toWgs84(), f107_average, f107_daily, ap, nullptr);
	}

	template <class T>
	auto operator()(const T &pos, double f107_average, double f107_daily, const MagneticIndex &ap) ->
	  typename std::enable_if_t<internal::HasToWgs84<T>::value, AtmosphericParameters> {
		return gtd7Interface(pos.toWgs84(), f107_average, f107_daily, ap.ap[0], ap.ap);
	}

	template <class T>
	auto operator()(const T &pos, const SpaceWeather &db) ->
	  typename std::enable_if_t<internal::HasToWgs84<T>::value, AtmosphericParameters> {
		double f107_average = 0, f107_daily = 0;
		MagneticIndex ap{};

		pickOutSpDbF107Average(db, pos.epoch(), f107_average);
		pickOutSpDbF107Daily(db, pos.epoch(), f107_daily);
		pickOutSpDbAp(db, pos.epoch(), ap);

		return operator()(pos.toWgs84(), f107_average, f107_daily, ap);
	}

	// void configureModel(const ModelConfig &config) { m_config = config; }

	void configureOutputUnit(DensityUnit d_unit, TemperatureUnit t_unit) {
		switch (d_unit) {
			case DensityUnit::GramPerCm3:
			case DensityUnit::Cgs: {
				m_config.mks_unit_conversion = SwitchStatus::Off;
			} break;

			case DensityUnit::KgPerM3:
			case DensityUnit::Mks:
			case DensityUnit::Si: {
				m_config.mks_unit_conversion = SwitchStatus::On;
			} break;

			default: {
				m_config.mks_unit_conversion = SwitchStatus::Off;
			} break;
		}

		if (t_unit != TemperatureUnit::Kelvin) {
			m_config.degc_unit_conversion = SwitchStatus::On;
			temperature_offset = constant::temperature_0degc_in_kelvin;
		} else {
			m_config.degc_unit_conversion = SwitchStatus::Off;
			temperature_offset = 0;
		}
	}

  private:
	ModelConfig m_config;
	internal::NrlmsiseInput m_nv_input;
	internal::NrlmsiseOutput m_nv_output;
	internal::NrlmsiseConfig m_nv_config;
	double temperature_offset;

	AtmosphericParameters gtd7Interface(const Wgs84 &pos, const double &f107_average, const double &f107_daily, const double &ap,
										const double *ap_array, const double &lst = std::numeric_limits<double>::infinity());
	bool pickOutSpDbF107Daily(const SpaceWeather &db, const DateTime &dt, double &f107_daily);
	bool pickOutSpDbF107Average(const SpaceWeather &db, const DateTime &dt, double &f107_average);
	bool pickOutSpDbAp(const SpaceWeather &db, const DateTime &dt, MagneticIndex &ap);
};

AtmosphericParameters GeoAtmosDensity::gtd7Interface(const Wgs84 &pos, const double &f107_average, const double &f107_daily,
													 const double &ap, const double *ap_array, const double &lst) {
	// 入出力設定
	if (ap_array) m_config.daily_ap = SwitchStatus::Specific;
	m_nv_config = m_config.convertNativeStatus();

	// 入力
	{
		m_nv_input.alt = pos.altitude() * 1e-3; // m -> km
		m_nv_input.g_lat = pos.latitude().degrees();
		m_nv_input.g_long = pos.longitude().degrees();
		m_nv_input.year = pos.epoch().year();
		m_nv_input.doy = pos.epoch().dayOfYear();
		m_nv_input.sec = pos.epoch().secondsOfDay();
		m_nv_input.lst = (std::isfinite(lst)) ? lst : m_nv_input.sec / 3600.0 + m_nv_input.g_long / 15.0;
		m_nv_input.f107A = f107_average;
		m_nv_input.f107 = f107_daily;
		if (ap_array) {
			std::copy(ap_array, ap_array + std::size(m_nv_input.ap_a.a), m_nv_input.ap_a.a);
		} else {
			m_nv_input.ap = ap;
		}
	}

	// モデル計算
	gtd7(m_nv_input, m_nv_config, m_nv_output);

	DensityUnit dens_unit = m_config.mks_unit_conversion == SwitchStatus::On ? DensityUnit::KgPerM3 : DensityUnit::GramPerCm3;
	TemperatureUnit temp_unit = m_config.degc_unit_conversion == SwitchStatus::On ? TemperatureUnit::Celsius : TemperatureUnit::Kelvin;

	// 出力
	return {{
			  m_nv_output.d[6],
			  m_nv_output.d[0],
			  m_nv_output.d[7],
			  m_nv_output.d[1],
			  m_nv_output.d[4],
			  m_nv_output.d[2],
			  m_nv_output.d[3],
			  m_nv_output.d[8],
			  m_nv_output.d[5],
			  dens_unit,
			},
			{m_nv_output.t[0] - temperature_offset, m_nv_output.t[1] - temperature_offset, temp_unit}};
}

bool GeoAtmosDensity::pickOutSpDbF107Daily(const SpaceWeather &db, const DateTime &dt, double &f107_daily) {
	try {
		f107_daily = db.adjustedF107(dt - Days(1));
		return true;
	} catch (const std::out_of_range &e) {
		f107_daily = 150;
		return false;
	}
}

bool GeoAtmosDensity::pickOutSpDbF107Average(const SpaceWeather &db, const DateTime &dt, double &f107_average) {
	try {
		f107_average = db.adjustedF107Center81(dt);
		return true;
	} catch (const std::out_of_range &e) {
		f107_average = 150;
		return false;
	}
}

bool GeoAtmosDensity::pickOutSpDbAp(const SpaceWeather &db, const DateTime &dt, MagneticIndex &ap) {
	constexpr double time_step = -3.0;	  // -3h
	constexpr int avg_times = 8;		  // 8 times
	constexpr int m12_m33_start = 12 / 3; // -12h
	constexpr int m12_m33_end = 33 / 3;	  // -33h
	constexpr int m36_m57_start = 36 / 3; // -36h
	constexpr int m36_m57_end = 57 / 3;	  // -57h

	try {
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

		return true;
	} catch (const std::out_of_range &e) {
		ap = MagneticIndex{};
		return false;
	}
}

GEOATMOS_NAMESPACE_END