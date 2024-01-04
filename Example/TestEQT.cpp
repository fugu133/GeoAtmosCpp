/**
 * @file TestEQT.cpp
 * @author Kaiji Takeuchi
 * @brief
 * @version 0.1
 * @date 2024-01-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <GeoAtmos/Core.hpp>
#include <fstream>
#include <iostream>
using namespace geoatmos;

int main() {
	std::ofstream ofs("eqt.csv");
	DateTime start_dt{"2024-01-01"};
	DateTime end_dt{"2025-01-01"};
	TimeSpan delta_t{Seconds(69)};

	ofs << "Days, AST [h], EOT [h]" << std::endl;
	for (auto dt = start_dt; dt < end_dt; dt += Days(1)) {
		ofs << (dt - start_dt).totalDays() + 1 << ", " << dt.greenwichSolarTime(delta_t).toString(AngleUnit::Hms) << ", "
			<< dt.equationOfTime(delta_t).toString(AngleUnit::Hms) << std::endl;
	}
	ofs.close();
}
