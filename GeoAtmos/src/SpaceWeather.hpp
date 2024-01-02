/**
 * @file SpaceWeather.hpp
 * @author fugu133
 * @brief 宇宙天気データベース
 * @ref https://celestrak.org/SpaceData/
 * @version 0.1
 * @date 2024-01-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <charconv>
#include <filesystem>
#include <fstream>
#include <map>
#include <string>
#include <type_traits>

#include "Essential.hpp"
#include "GeoAtmosType.hpp"

GEOATMOS_NAMESPACE_BEGIN

enum class F107Type { Observed, Predicted, Mixed };

struct SpaceWeatherData {
	std::uint16_t bsrn;		 // Bartels Solar Rotation Number from 1832 Feb 8.
	std::uint16_t nd;		 // Number of days in the month
	std::uint8_t kp[8];		 // K-index 0o, 0+, 1-, 1o, 1+, 2-, 2o, 2+, ..., 8o, 8+, 9-, 9o (1/3 step, 29 steps)
	std::int16_t kp_sum;	 // K-index sum
	std::int16_t ap[8];		 // A-index a.k.a linear K-index (0, 1, , ..., 399, 400)
	std::int16_t ap_avg;	 // A-index average
	float cp;				 // C-index
	std::uint8_t c9;		 // C9-index (0 to 9)
	std::uint16_t isn;		 // International Sunspot Number
	float f107_obs;			 // 10.7cm Solar Radio Flux
	float f107_adj;			 // 10.7cm Solar Radio Flux adjusted for 1 AU
	F107Type f107_type;		 // F10.7 type (Observed, Predicted, Mixed)
	float f107_obs_center81; // 81-day centered average of F10.7
	float f107_obs_last81;	 // 81-day average of F10.7
	float f107_adj_center81; // 81-day centered average of F10.7 adjusted for 1 AU
	float f107_adj_last81;	 // 81-day average of F10.7 adjusted for 1 AU

	SpaceWeatherData()
	  : bsrn(0),
		nd(0),
		kp{0},
		kp_sum(0),
		ap{0},
		ap_avg(0),
		cp(0),
		c9(0),
		isn(0),
		f107_obs(0),
		f107_adj(0),
		f107_type(F107Type::Observed),
		f107_obs_center81(0),
		f107_obs_last81(0),
		f107_adj_center81(0),
		f107_adj_last81(0) {}
};

/**
 * @brief 宇宙天気データベース
 *
 */
class SpaceWeather {
  public:
	SpaceWeather(const std::filesystem::path& path);
	SpaceWeather(const char* filename) : SpaceWeather(std::filesystem::path(filename)) {}
	SpaceWeather(const std::string& filename) : SpaceWeather(std::filesystem::path(filename)) {}
	SpaceWeather(std::ifstream& ifs);
	~SpaceWeather() { m_data.clear(); }

	const SpaceWeatherData& at(const DateTime& dt) const { return m_data.at(toKey(dt)); }

	int dailyApIndex(const DateTime& dt) const { return m_data.at(toKey(dt)).ap_avg; }

	int apIndex(const DateTime& dt) const { return m_data.at(toKey(dt)).ap[getIndex(dt)]; }

	int apIndex(const DateTime& dt, TimeSpan span) const {
		auto adj_dt = dt + span;
		return m_data.at(toKey(adj_dt)).ap[getIndex(adj_dt)];
	}

	int kpIndex(const DateTime& dt) const { return m_data.at(toKey(dt)).kp[getIndex(dt)]; }

	int kpIndex(const DateTime& dt, TimeSpan span) const {
		auto adj_dt = dt + span;
		return m_data.at(toKey(adj_dt)).kp[getIndex(adj_dt)];
	}

	double cpIndex(const DateTime& dt) const { return (double)m_data.at(toKey(dt)).cp; }

	double adjustedF107(const DateTime& dt) const { return (double)m_data.at(toKey(dt)).f107_adj; }

	double observedF107(const DateTime& dt) const { return (double)m_data.at(toKey(dt)).f107_obs; }

	double adjustedF107Center81(const DateTime& dt) const { return (double)m_data.at(toKey(dt)).f107_adj_center81; }

	double observedF107Center81(const DateTime& dt) const { return (double)m_data.at(toKey(dt)).f107_obs_center81; }

	double adjustedF107Last81(const DateTime& dt) const { return (double)m_data.at(toKey(dt)).f107_adj_last81; }

	double observedF107Last81(const DateTime& dt) const { return (double)m_data.at(toKey(dt)).f107_obs_last81; }

	bool find(const DateTime& dt) {
		if (m_is_exist && m_is_loaded) {
			if (m_data.count(toKey(dt)) > 0) {
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	}

  private:
	static constexpr char c_separator = ',';
	std::map<DateTime, SpaceWeatherData> m_data;
	bool m_is_exist;
	bool m_is_loaded;
	DateTime m_min_dt;
	DateTime m_max_dt;

	DateTime toKey(const DateTime& dt) const { return DateTime((dt.ticks() / constant::ticks_per_day) * constant::ticks_per_day); }

	bool load(std::ifstream& ifs);

	bool exist(const std::filesystem::path& path) {
		namespace fs = std::filesystem;

		if (fs::exists(path)) {
			m_is_exist = true;
		} else {
			m_is_exist = false;
			m_is_loaded = false;
		}
		return m_is_exist;
	}

	bool findBlock(const std::string& line, std::size_t s_pos, std::size_t& e_pos) {
		for (std::size_t i = s_pos; i < line.size(); i++) {
			if (line[i] == c_separator || line[i] == '\n' || line[i] == '\r') {
				e_pos = i;
				return e_pos != s_pos + 1;
			}
		}
		return false;
	}

	template <typename T>
	bool readBlock(const std::string& line, std::size_t s_pos, std::size_t& e_pos, T& value) {
		if constexpr (std::is_integral_v<T>) {
			auto p = std::from_chars(line.data() + s_pos, line.data() + line.size(), value);
			if (p.ec == std::errc()) {
				e_pos = p.ptr - line.data();
				return true;
			} else {
				throw std::runtime_error("Failed to read block.");
				return false;
			}
		} else if constexpr (std::is_floating_point_v<T>) {
			if (findBlock(line, s_pos, e_pos)) {
				value = std::stof(line.substr(s_pos, e_pos - s_pos));
				return true;
			} else {
				throw std::runtime_error("Failed to read block.");
				return false;
			}
		} else if constexpr (std::is_same_v<T, F107Type>) {
			std::string tmp;
			if (findBlock(line, s_pos, e_pos)) {
				tmp = line.substr(s_pos, e_pos - s_pos);
				value = (tmp == "OBS") ? F107Type::Observed : F107Type::Predicted;
				return true;
			} else {
				throw std::runtime_error("Failed to read block.");
				return false;
			}
		} else {
			// static_assert(false, "Not supported type.");
			return false;
		}
	}

	std::size_t getIndex(DateTime dt) const {
		// (previous day)
		// 0000UT - 0300UT: 0
		// 0300UT - 0600UT: 1
		// 0600UT - 0900UT: 2
		// 0900UT - 1200UT: 3
		// 1200UT - 1500UT: 4
		// 1500UT - 1800UT: 5
		// 1800UT - 2100UT: 6
		// 2100UT - 0000UT: 7
		// (next day)

		auto t = dt.ticks() % constant::ticks_per_day;
		std::size_t idx = t / (3 * constant::ticks_per_hour);
		return idx;
	}
};

SpaceWeather::SpaceWeather(const std::filesystem::path& f_path)
  : m_is_exist(false), m_is_loaded(false), m_min_dt(DateTime::max()), m_max_dt(DateTime::min()) {
	namespace fs = std::filesystem;

	if (exist(f_path)) {
		std::ifstream ifs(f_path);
		m_is_loaded = load(ifs);
	}
}

SpaceWeather::SpaceWeather(std::ifstream& ifs) {
	if (ifs.is_open()) {
		m_is_exist = true;
		m_is_loaded = load(ifs);
	} else {
		m_is_exist = false;
		m_is_loaded = false;
	}
}

bool SpaceWeather::load(std::ifstream& ifs) {
	if (ifs.is_open()) {
		std::size_t s_pos = 0, e_pos = 0;
		std::string line;
		DateTime dt;
		SpaceWeatherData data;
		m_min_dt = DateTime::max();
		m_max_dt = DateTime::min();

		while (std::getline(ifs, line)) {
			try {
				dt = DateTime(line.substr(s_pos, 10));
				m_min_dt = std::min(m_min_dt, dt);
				m_max_dt = std::max(m_max_dt, dt);
				e_pos = 10;
				readBlock(line, e_pos + 1, e_pos, data.bsrn);
				readBlock(line, e_pos + 1, e_pos, data.nd);
				for (std::size_t i = 0; i < 8; i++) {
					readBlock(line, e_pos + 1, e_pos, data.kp[i]);
				}
				readBlock(line, e_pos + 1, e_pos, data.kp_sum);
				for (std::size_t i = 0; i < 8; i++) {
					readBlock(line, e_pos + 1, e_pos, data.ap[i]);
				}
				readBlock(line, e_pos + 1, e_pos, data.ap_avg);
				readBlock(line, e_pos + 1, e_pos, data.cp);
				readBlock(line, e_pos + 1, e_pos, data.c9);
				readBlock(line, e_pos + 1, e_pos, data.isn);
				readBlock(line, e_pos + 1, e_pos, data.f107_obs);
				readBlock(line, e_pos + 1, e_pos, data.f107_adj);
				readBlock(line, e_pos + 1, e_pos, data.f107_type);
				readBlock(line, e_pos + 1, e_pos, data.f107_obs_center81);
				readBlock(line, e_pos + 1, e_pos, data.f107_obs_last81);
				readBlock(line, e_pos + 1, e_pos, data.f107_adj_center81);
				readBlock(line, e_pos + 1, e_pos, data.f107_adj_last81);
				m_data.emplace(dt, data);
			} catch (const std::exception& e) {
				continue;
			}
		}

		return true;
	}

	return false;
}
GEOATMOS_NAMESPACE_END