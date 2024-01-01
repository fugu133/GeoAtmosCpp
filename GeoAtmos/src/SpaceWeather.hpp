/**
 * @file SpaceWeather.hpp
 * @author Kaiji Takeuchi
 * @brief 宇宙天気データベース
 * @ref https://celestrak.org/SpaceData/
 * @version 0.1
 * @date 2024-01-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Essential.hpp"

GEOATMOS_NAMESPACE_BEGIN

// struct SpaceWeatherData {
// 	DateTime date;
// 	double 	brsn;
//     double 	nd;
// };

/**
 * @brief 宇宙天気データベース
 *
 */

class SpaceWeather {
  public:
	SpaceWeather(const std::string& filename);
	SpaceWeather(std::ifstream& ifs);

  private:
	std::vector<std::string> m_lines;
}