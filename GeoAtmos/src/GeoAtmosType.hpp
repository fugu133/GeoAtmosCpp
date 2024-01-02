/**
 * @file GeoAtmosType.hpp
 * @author fugu133
 * @brief
 * @version 0.1
 * @date 2024-01-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Essential.hpp"

GEOATMOS_NAMESPACE_BEGIN

namespace internal {

	/**
	 * @brief NRLMSISE-00 Atmosphere Model Configuration Switches
	 *
	 */
	struct NrlmsiseConfig {
		int switches[24];
		double sw[24];
		double swc[24];
	};

	/**
	 * @brief NRLMSISE-00 Magnetic Activity aka AP
	 *
	 */
	struct ApArray {
		double a[7];
	};

	/**
	 * @brief NRLMSISE-00 Atmosphere Model Input
	 *
	 */
	struct NrlmsiseInput {
		int year;
		int doy;
		double sec;
		double alt;
		double g_lat;
		double g_long;
		double lst;
		double f107A;
		double f107;
		double ap;
		ApArray ap_a;
	};

	/**
	 * @brief NRLMSISE-00 Atmosphere Model Output
	 *
	 */
	struct NrlmsiseOutput {
		double d[9]; /* densities */
		double t[2]; /* temperatures */
	};
} // namespace internal

/**
 * @brief スイッチの状態
 *
 */
enum class SwitchStatus : std::int8_t { Off, On, Specific };

/**
 * @brief NRLMSISE-00 のモデル設定
 *
 */
struct ModelConfig {
	SwitchStatus unit_conversion;
	SwitchStatus f107_effect;
	SwitchStatus time_independent;
	SwitchStatus symmetrical_annual;
	SwitchStatus symmetrical_semiannual;
	SwitchStatus asymmetrical_annual;
	SwitchStatus asymmetrical_semiannual;
	SwitchStatus diurnal;
	SwitchStatus semidiurnal;
	SwitchStatus daily_ap;
	SwitchStatus all_ut_long_effect;
	SwitchStatus longitudinal;
	SwitchStatus ut_and_mixed_ut_long;
	SwitchStatus mixed_ap_ut_long;
	SwitchStatus terdiurnal;
	SwitchStatus departures_from_diff_equ;
	SwitchStatus var_all_exospheric_temp;
	SwitchStatus var_all_from_120km_temp;
	SwitchStatus var_low_thermosphere_temp;
	SwitchStatus var_all_120km_grad;
	SwitchStatus var_up_stratosphere_temp;
	SwitchStatus var_all_120km;
	SwitchStatus var_all_low_mesosphere_temp;
	SwitchStatus var_turbopause_scale_height;

	ModelConfig()
	  : unit_conversion(SwitchStatus::Off),
		f107_effect(SwitchStatus::On),
		time_independent(SwitchStatus::On),
		symmetrical_annual(SwitchStatus::On),
		symmetrical_semiannual(SwitchStatus::On),
		asymmetrical_annual(SwitchStatus::On),
		asymmetrical_semiannual(SwitchStatus::On),
		diurnal(SwitchStatus::On),
		semidiurnal(SwitchStatus::On),
		daily_ap(SwitchStatus::On),
		all_ut_long_effect(SwitchStatus::On),
		longitudinal(SwitchStatus::On),
		ut_and_mixed_ut_long(SwitchStatus::On),
		mixed_ap_ut_long(SwitchStatus::On),
		terdiurnal(SwitchStatus::On),
		departures_from_diff_equ(SwitchStatus::On),
		var_all_exospheric_temp(SwitchStatus::On),
		var_all_from_120km_temp(SwitchStatus::On),
		var_low_thermosphere_temp(SwitchStatus::On),
		var_all_120km_grad(SwitchStatus::On),
		var_up_stratosphere_temp(SwitchStatus::On),
		var_all_120km(SwitchStatus::On),
		var_all_low_mesosphere_temp(SwitchStatus::On),
		var_turbopause_scale_height(SwitchStatus::On) {}

	internal::NrlmsiseConfig convertNativeStatus() const {
		internal::NrlmsiseConfig config;
		config.switches[0] = static_cast<int>(unit_conversion);
		config.switches[1] = static_cast<int>(f107_effect);
		config.switches[2] = static_cast<int>(time_independent);
		config.switches[3] = static_cast<int>(symmetrical_annual);
		config.switches[4] = static_cast<int>(symmetrical_semiannual);
		config.switches[5] = static_cast<int>(asymmetrical_annual);
		config.switches[6] = static_cast<int>(asymmetrical_semiannual);
		config.switches[7] = static_cast<int>(diurnal);
		config.switches[8] = static_cast<int>(semidiurnal);
		if (daily_ap == SwitchStatus::Specific) {
			config.switches[9] = -1;
		} else {
			config.switches[9] = static_cast<int>(daily_ap);
		}
		config.switches[10] = static_cast<int>(all_ut_long_effect);
		config.switches[11] = static_cast<int>(longitudinal);
		config.switches[12] = static_cast<int>(ut_and_mixed_ut_long);
		config.switches[13] = static_cast<int>(mixed_ap_ut_long);
		config.switches[14] = static_cast<int>(terdiurnal);
		config.switches[15] = static_cast<int>(departures_from_diff_equ);
		config.switches[16] = static_cast<int>(var_all_exospheric_temp);
		config.switches[17] = static_cast<int>(var_all_from_120km_temp);
		config.switches[18] = static_cast<int>(var_low_thermosphere_temp);
		config.switches[19] = static_cast<int>(var_all_120km_grad);
		config.switches[20] = static_cast<int>(var_up_stratosphere_temp);
		config.switches[21] = static_cast<int>(var_all_120km);
		config.switches[22] = static_cast<int>(var_all_low_mesosphere_temp);
		config.switches[23] = static_cast<int>(var_turbopause_scale_height);
		return config;
	}
};

/**
 * @brief 磁気指数 (Ap)
 *
 */
struct MagneticIndex {
	double ap[7];

	MagneticIndex() : ap{4, 4, 4, 4, 4, 4, 4} {}

	MagneticIndex(double ap_a, double ap_kp, double ap_ao, double ap_ap, double ap_ae, double ap_al, double ap_af)
	  : ap{ap_a, ap_kp, ap_ao, ap_ap, ap_ae, ap_al, ap_af} {}
};

enum class DensityUnit { GramPerCm3, KgPerM3, Cgs, Si };

/**
 * @brief 大気密度と温度
 *
 */
struct Density {
	double atomic_hydrogen;	   // 水素原子密度 [cm^-3 or /m^3]
	double atomic_helium;	   // ヘリウム原子密度 [cm^-3 or /m^3]
	double atomic_nitrogen;	   // 窒素原子密度 [cm^-3 or /m^3]
	double atomic_oxygen;	   // 酸素原子密度 [cm^-3 or /m^3]
	double atomic_argon;	   // アルゴン原子密度 [cm^-3 or /m^3]
	double molecular_nitrogen; // 窒素分子密度 [cm^-3 or /m^3]
	double molecular_oxygen;   // 酸素分子密度 [cm^-3 or /m^3]
	double anomalous_oxygen;   // 異常酸素密度 [cm^-3 or /m^3]
	double atmosphere;		   // 大気質量密度 [g/cm^3 or kg/m^3]

	Density()
	  : atomic_hydrogen(0),
		atomic_helium(0),
		atomic_nitrogen(0),
		atomic_oxygen(0),
		atomic_argon(0),
		molecular_nitrogen(0),
		molecular_oxygen(0),
		anomalous_oxygen(0),
		atmosphere(0) {}

	Density(double atomic_hydrogen, double atomic_helium, double atomic_nitrogen, double atomic_oxygen, double atomic_argon,
			double molecular_nitrogen, double molecular_oxygen, double anomalous_oxygen, double atmosphere)
	  : atomic_hydrogen(atomic_hydrogen),
		atomic_helium(atomic_helium),
		atomic_nitrogen(atomic_nitrogen),
		atomic_oxygen(atomic_oxygen),
		atomic_argon(atomic_argon),
		molecular_nitrogen(molecular_nitrogen),
		molecular_oxygen(molecular_oxygen),
		anomalous_oxygen(anomalous_oxygen),
		atmosphere(atmosphere) {}
};

/**
 * @brief 大気密度と温度
 *
 */
struct Temperature {
	double at_exosphere; // 外気圏での大気温度 [K]
	double at_altitude;	 // 高度での大気温度 [K]

	Temperature() : at_exosphere(0), at_altitude(0) {}
	Temperature(double at_exosphere, double at_altitude) : at_exosphere(at_exosphere), at_altitude(at_altitude) {}
};

/**
 * @brief 大気パラメータ
 *
 */
struct AtmosphericParameters {
	Density density;		 // 大気密度
	Temperature temperature; // 大気温度

	AtmosphericParameters() : density(), temperature() {}
	AtmosphericParameters(const Density &density, const Temperature &temperature) : density(density), temperature(temperature) {}
};

GEOATMOS_NAMESPACE_END