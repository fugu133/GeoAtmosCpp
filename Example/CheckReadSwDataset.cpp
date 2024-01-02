/**
 * @file CheckReadSpDataSet.cpp
 * @author fugu133
 * @brief 宇宙天気データベースの読み込み確認
 * @version 0.1
 * @date 2024-01-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <GeoAtmos/Core.hpp>
#include <iostream>

using namespace geoatmos;

int main() {
	SpaceWeather sw_dataset("SW-Last5Years.csv");

	auto dt = DateTime{"2023-12-31T00:00:00"};

	std::cout << "AP +00h: " << sw_dataset.apIndex(dt, Hours{0}) << std::endl;
	std::cout << "AP -03h: " << sw_dataset.apIndex(dt, Hours{-3}) << std::endl;
	std::cout << "AP -06h: " << sw_dataset.apIndex(dt, Hours{-6}) << std::endl;
	std::cout << "AP -09h: " << sw_dataset.apIndex(dt, Hours{-9}) << std::endl;
	std::cout << "AP -12h: " << sw_dataset.apIndex(dt, Hours{-12}) << std::endl;
	std::cout << "AP -15h: " << sw_dataset.apIndex(dt, Hours{-15}) << std::endl;
	std::cout << "AP -18h: " << sw_dataset.apIndex(dt, Hours{-18}) << std::endl;
	std::cout << "AP -21h: " << sw_dataset.apIndex(dt, Hours{-21}) << std::endl;
	std::cout << "AP -24h: " << sw_dataset.apIndex(dt, Hours{-24}) << std::endl;
	std::cout << "AP -27h: " << sw_dataset.apIndex(dt, Hours{-27}) << std::endl;
	std::cout << "AP -30h: " << sw_dataset.apIndex(dt, Hours{-30}) << std::endl;
	std::cout << "AP -33h: " << sw_dataset.apIndex(dt, Hours{-33}) << std::endl;
	std::cout << "AP -36h: " << sw_dataset.apIndex(dt, Hours{-36}) << std::endl;
	std::cout << "AP -39h: " << sw_dataset.apIndex(dt, Hours{-39}) << std::endl;
	std::cout << "AP -42h: " << sw_dataset.apIndex(dt, Hours{-42}) << std::endl;
	std::cout << "AP -45h: " << sw_dataset.apIndex(dt, Hours{-45}) << std::endl;
	std::cout << "AP -48h: " << sw_dataset.apIndex(dt, Hours{-48}) << std::endl;
}