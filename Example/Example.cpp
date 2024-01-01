#include <GeoAtmos/Core.hpp>
#include <fstream>
using namespace geoatmos;

int main() {
	std::ofstream ofs("atmos.csv");

	// 位置情報
	const auto dt = DateTime{"2023-12-31T00:00:00"};
	const auto longitude = Degree{135};
	const auto latitude = Degree{35};
	constexpr auto altitude_start_m = 0.0;
	constexpr auto altitude_end_m = 500e3;

	// 10.7 cm 太陽電波束 (F10.7)
	const auto f107_avg = 155.1;   // 中心81日平均値 (AU)
	const auto f107_daily = 135.1; // 恐らく前日観測値 (AU)

	// AP 指数
	const auto ap_0 = 0;													   // +0h
	const auto ap_a3 = 3;													   // +3h
	const auto ap_b3 = 0;													   // -3h
	const auto ap_b6 = 4;													   // -6h
	const auto ap_b9 = 4;													   // -9h
	const auto ap_b12_b33_avg = (double)(3 + 5 + 9 + 4 + 9 + 5 + 3 + 3) / 8.0; // -12h ~ -33h (平均値)
	const auto ap_b36_b57_avg = (double)(3 + 0 + 6 + 4 + 5 + 5 + 9 + 6) / 8.0; // -36h ~ -57h (平均値)
	const auto ap = MagneticIndex{ap_0, ap_a3, ap_b3, ap_b6, ap_b9, ap_b12_b33_avg, ap_b36_b57_avg};

	ofs << "Altitude [km], Density [g/cm^3]" << std::endl;
	auto atmos = GeoAtmosDensity{};
	for (auto alt = altitude_start_m; alt <= altitude_end_m; alt += 1e3) {
		auto p = atmos(Wgs84{dt, longitude, latitude, alt}, f107_avg, f107_daily, ap);
		ofs << alt * 1e-3 << ", " << p.density.atmosphere << std::endl;
	}
	ofs.close();
}