/**
 * @file Nrlmsise00.hpp
 * @author fugu133
 * @brief NRLMSISE-00 Atmosphere Model
 * @ref https://map.nrl.navy.mil/map/pub/nrl/NRLMSIS/NRLMSISE-00/
 * @ref https://ccmc.gsfc.nasa.gov/models/NRLMSIS~00/
 * @version 0.1
 * @date 2023-12-26
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "AngleHelper.hpp"
#include "Coordinate.hpp"
#include "Essential.hpp"
#include "GeoAtmosType.hpp"
#include "Model.hpp"

GEOATMOS_NAMESPACE_BEGIN

namespace internal {
	/**
	 * @brief NRLMSISE-00 Atmosphere Model
	 *
	 */
	class Nrlmsise : public ModelSet {
	  public:
		Nrlmsise();

	  private:
		/* PARMB */
		double gsurf;
		double re;

		/* GTS3C */
		double dd;

		/* DMIX */
		double dm04, dm16, dm28, dm32, dm40, dm01, dm14;

		/* MESO7 */
		double meso_tn1[5];
		double meso_tn2[4];
		double meso_tn3[5];
		double meso_tgn1[2];
		double meso_tgn2[2];
		double meso_tgn3[2];

		/* LPOLY */
		double dfa;
		double plg[4][9];
		double ctloc, stloc;
		double c2tloc, s2tloc;
		double s3tloc, c3tloc;
		double apdf, apt[4];

		static double zn1[5];
		static double zn2[4];
		static double zn3[5];
		static double alpha[9];
		static double altl[8];

	  private:
		void tselec(NrlmsiseConfig &flags);
		void glatf(double lat, double &gv, double &reff);
		double ccor(double alt, double r, double h1, double zh);
		double ccor2(double alt, double r, double h1, double zh, double h2);
		double scalh(double alt, double xm, double temp);
		double dnet(double dd, double dm, double zhm, double xmm, double xm);
		void splini(double *xa, double *ya, double *y2a, int n, double x, double &y);
		void splint(double *xa, double *ya, double *y2a, int n, double x, double &y);
		void spline(double *x, double *y, int n, double yp1, double ypn, double *y2);
		double densm(double alt, double d0, double xm, double &tz, int mn3, double *zn3, double *tn3, double *tgn3, int mn2, double *zn2,
					 double *tn2, double *tgn2);
		double densu(double alt, double dlb, double tinf, double tlb, double xm, double alpha, double &tz, double zlb, double s2, int mn1,
					 double *zn1, double *tn1, double *tgn1);
		double globe7(double *p, NrlmsiseInput &input, NrlmsiseConfig &flags);
		double glob7s(double *p, NrlmsiseInput &input, NrlmsiseConfig &flags);
		void gtd7d(NrlmsiseInput &input, NrlmsiseConfig &flags, NrlmsiseOutput &output);
		void ghp7(NrlmsiseInput &input, NrlmsiseConfig &flags, NrlmsiseOutput &output, double press);
		void gts7(NrlmsiseInput &input, NrlmsiseConfig &flags, NrlmsiseOutput &output);

		double zeta(double zz, double zl) { return ((zz - zl) * (re + zl) / (re + zz)); }

		double g0(double a, const double *p) {
			return (a - 4.0 +
					(p[25] - 1.0) * (a - 4.0 + (std::exp(-std::sqrt(p[24] * p[24]) * (a - 4.0)) - 1.0) / std::sqrt(p[24] * p[24])));
		}

		double sumex(double ex) { return (1.0 + (1.0 - std::pow(ex, 19.0)) / (1.0 - ex) * std::pow(ex, 0.5)); }

		double sg0(double ex, const double *p, double *ap) {
			return (g0(ap[1], p) +
					(g0(ap[2], p) * ex + g0(ap[3], p) * ex * ex + g0(ap[4], p) * std::pow(ex, 3.0) +
					 (g0(ap[5], p) * std::pow(ex, 4.0) + g0(ap[6], p) * std::pow(ex, 12.0)) * (1.0 - std::pow(ex, 8.0)) / (1.0 - ex))) /
				   sumex(ex);
		}

	  protected:
		void gtd7(NrlmsiseInput &input, NrlmsiseConfig &flags, NrlmsiseOutput &output);
	};

	Nrlmsise::Nrlmsise() {}

	void Nrlmsise::tselec(NrlmsiseConfig &flags) {
		for (int i = 0; i < 24; i++) {
			if (i != 9) {
				if (flags.switches[i] == 1)
					flags.sw[i] = 1;
				else
					flags.sw[i] = 0;

				if (flags.switches[i] > 0)
					flags.swc[i] = 1;
				else
					flags.swc[i] = 0;
			} else {
				flags.sw[i] = flags.switches[i];
				flags.swc[i] = flags.switches[i];
			}
		}
	}

	void Nrlmsise::glatf(double lat, double &gv, double &reff) {
		const double c2 = std::cos(2.0 * AngleHelper::degreeToRadian(lat));
		gv = 980.616 * (1.0 - 0.0026373 * c2);
		reff = 2.0 * (gv) / (3.085462E-6 + 2.27E-9 * c2) * 1.0E-5;
	}

	double Nrlmsise::ccor(double alt, double r, double h1, double zh) {
		const double e = (alt - zh) / h1;
		if (e > 70) return std::exp(0);
		if (e < -70) return std::exp(r);
		return std::exp(r / (1.0 + std::exp(e)));
	}

	double Nrlmsise::ccor2(double alt, double r, double h1, double zh, double h2) {

		const double e1 = (alt - zh) / h1;
		const double e2 = (alt - zh) / h2;

		if ((e1 > 70) || (e2 > 70)) return std::exp(0);
		if ((e1 < -70) && (e2 < -70)) return std::exp(r);

		const double ccor2v = r / (1.0 + 0.5 * (std::exp(e1) + std::exp(e2)));
		return exp(ccor2v);
	}

	double Nrlmsise::scalh(double alt, double xm, double temp) {
		constexpr double rgas = 831.4;
		double g = gsurf / (std::pow((1.0 + alt / re), 2.0));
		return rgas * temp / (g * xm);
	}

	double Nrlmsise::dnet(double dd, double dm, double zhm, double xmm, double xm) {

		if (!((dm > 0) && (dd > 0))) {
			throw AtmosModelException("Argument x of function log(x) is 0 or negative", AtmosModelException::MathmaticalError);
			if ((dd == 0) && (dm == 0)) dd = 1;
			if (dm == 0) return dd;
			if (dd == 0) return dm;
		}

		double a = zhm / (xmm - xm);
		double ylog = a * std::log(dm / dd);
		if (ylog < -10) return dd;
		if (ylog > 10) return dm;
		a = dd * std::pow((1.0 + std::exp(ylog)), (1.0 / a));
		return a;
	}

	void Nrlmsise::splini(double *xa, double *ya, double *y2a, int n, double x, double &y) {
		double yi = 0;

		int klo = 0;
		int khi = 1;

		double xx, h, a, b, a2, b2;

		while ((x > xa[klo]) && (khi < n)) {
			xx = x;

			if (khi < (n - 1)) {
				xx = (x < xa[khi]) ? x : xa[khi];
			}

			h = xa[khi] - xa[klo];
			a = (xa[khi] - xx) / h;
			b = (xx - xa[klo]) / h;
			a2 = a * a;
			b2 = b * b;
			yi += ((1.0 - a2) * ya[klo] / 2.0 + b2 * ya[khi] / 2.0 +
				   ((-(1.0 + a2 * a2) / 4.0 + a2 / 2.0) * y2a[klo] + (b2 * b2 / 4.0 - b2 / 2.0) * y2a[khi]) * h * h / 6.0) *
				  h;
			klo++;
			khi++;
		}

		y = yi;
	}

	void Nrlmsise::splint(double *xa, double *ya, double *y2a, int n, double x, double &y) {
		int klo = 0;
		int khi = n - 1;
		int k;

		while ((khi - klo) > 1) {
			k = (khi + klo) / 2;
			if (xa[k] > x) {
				khi = k;
			} else {
				klo = k;
			}
		}

		double h = xa[khi] - xa[klo];
		if (h == 0.0) throw AtmosModelException("Interpolation step is invalid.", AtmosModelException::MathmaticalError);

		double a = (xa[khi] - x) / h;
		double b = (x - xa[klo]) / h;
		double yi = a * ya[klo] + b * ya[khi] + ((a * a * a - a) * y2a[klo] + (b * b * b - b) * y2a[khi]) * h * h / 6.0;

		y = yi;
	}

	void Nrlmsise::spline(double *x, double *y, int n, double yp1, double ypn, double *y2) {
		std::vector<double> u(n);

		double sig, p, qn, un;

		if (yp1 > 0.99E30) {
			y2[0] = 0;
			u[0] = 0;
		} else {
			y2[0] = -0.5;
			u[0] = (3.0 / (x[1] - x[0])) * ((y[1] - y[0]) / (x[1] - x[0]) - yp1);
		}

		for (int i = 1; i < (n - 1); i++) {
			sig = (x[i] - x[i - 1]) / (x[i + 1] - x[i - 1]);
			p = sig * y2[i - 1] + 2.0;
			y2[i] = (sig - 1.0) / p;
			u[i] = (6.0 * ((y[i + 1] - y[i]) / (x[i + 1] - x[i]) - (y[i] - y[i - 1]) / (x[i] - x[i - 1])) / (x[i + 1] - x[i - 1]) -
					sig * u[i - 1]) /
				   p;
		}

		if (ypn > 0.99E30) {
			qn = 0;
			un = 0;
		} else {
			qn = 0.5;
			un = (3.0 / (x[n - 1] - x[n - 2])) * (ypn - (y[n - 1] - y[n - 2]) / (x[n - 1] - x[n - 2]));
		}

		y2[n - 1] = (un - qn * u[n - 2]) / (qn * y2[n - 2] + 1.0);

		for (int k = n - 2; k >= 0; k--) y2[k] = y2[k] * y2[k + 1] + u[k];
	}

	double Nrlmsise::densm(double alt, double d0, double xm, double &tz, int mn3, double *zn3, double *tn3, double *tgn3, int mn2,
						   double *zn2, double *tn2, double *tgn2) {

		/*      Calculate Temperature and Density Profiles for lower atmos.  */

		double xs[10], ys[10], y2out[10];
		constexpr double rgas = 831.4;
		double z, z1, z2, t1, t2, zg, zgdif;
		double yd1, yd2;
		double x, y, yi;
		double expl, gamm, glb;
		double densm_tmp;
		int mn;

		densm_tmp = d0;
		if (alt > zn2[0]) {
			if (xm == 0.0) {
				return tz;
			} else {
				return d0;
			}
		}

		/* Stratosphere/Mesosphere temperature */
		z = (alt > zn2[mn2 - 1]) ? alt : zn2[mn2 - 1];
		mn = mn2;
		z1 = zn2[0];
		z2 = zn2[mn - 1];
		t1 = tn2[0];
		t2 = tn2[mn - 1];
		zg = zeta(z, z1);
		zgdif = zeta(z2, z1);

		/* set up spline nodes */
		for (int k = 0; k < mn; k++) {
			xs[k] = zeta(zn2[k], z1) / zgdif;
			ys[k] = 1.0 / tn2[k];
		}

		yd1 = -tgn2[0] / (t1 * t1) * zgdif;
		yd2 = -tgn2[1] / (t2 * t2) * zgdif * (std::pow(((re + z2) / (re + z1)), 2.0));

		/* calculate spline coefficients */
		spline(xs, ys, mn, yd1, yd2, y2out);
		x = zg / zgdif;
		splint(xs, ys, y2out, mn, x, y);

		/* temperature at altitude */
		tz = 1.0 / y;
		if (xm != 0.0) {
			/* calculate stratosphere / mesosphere density */
			glb = gsurf / (std::pow((1.0 + z1 / re), 2.0));
			gamm = xm * glb * zgdif / rgas;

			/* Integrate temperature profile */
			splini(xs, ys, y2out, mn, x, yi);
			expl = gamm * yi;
			if (expl > 50.0) expl = 50.0;

			/* Density at altitude */
			densm_tmp = densm_tmp * (t1 / tz) * std::exp(-expl);
		}

		if (alt > zn3[0]) {
			if (xm == 0.0) {
				return tz;
			} else {
				return densm_tmp;
			}
		}

		/* troposphere / stratosphere temperature */
		z = alt;
		mn = mn3;
		z1 = zn3[0];
		z2 = zn3[mn - 1];
		t1 = tn3[0];
		t2 = tn3[mn - 1];
		zg = zeta(z, z1);
		zgdif = zeta(z2, z1);

		/* set up spline nodes */
		for (int k = 0; k < mn; k++) {
			xs[k] = zeta(zn3[k], z1) / zgdif;
			ys[k] = 1.0 / tn3[k];
		}

		yd1 = -tgn3[0] / (t1 * t1) * zgdif;
		yd2 = -tgn3[1] / (t2 * t2) * zgdif * (std::pow(((re + z2) / (re + z1)), 2.0));

		/* calculate spline coefficients */
		spline(xs, ys, mn, yd1, yd2, y2out);
		x = zg / zgdif;
		splint(xs, ys, y2out, mn, x, y);

		/* temperature at altitude */
		tz = 1.0 / y;
		if (xm != 0.0) {
			/* calaculate tropospheric / stratosphere density */
			glb = gsurf / (std::pow((1.0 + z1 / re), 2.0));
			gamm = xm * glb * zgdif / rgas;

			/* Integrate temperature profile */
			splini(xs, ys, y2out, mn, x, yi);
			expl = gamm * yi;
			if (expl > 50.0) expl = 50.0;

			/* Density at altitude */
			densm_tmp = densm_tmp * (t1 / tz) * std::exp(-expl);
		}

		if (xm == 0.0)
			return tz;
		else
			return densm_tmp;
	}

	double Nrlmsise::densu(double alt, double dlb, double tinf, double tlb, double xm, double alpha, double &tz, double zlb, double s2,
						   int mn1, double *zn1, double *tn1, double *tgn1) {
		constexpr double rgas = 831.4;
		double yd2, yd1, x, y;
		double densu_temp = 1.0;
		double za, z, zg2, tt, ta;
		double dta, z1, z2, t1, t2, zg, zgdif;
		int mn;
		double glb;
		double expl;
		double yi;
		double densa;
		double gamma, gamm;
		double xs[5], ys[5], y2out[5];

		/* joining altitudes of Bates and spline */
		za = zn1[0];
		z = (alt > za) ? alt : za;

		/* geo-potential altitude difference from ZLB */
		zg2 = zeta(z, zlb);

		/* Bates temperature */
		tt = tinf - (tinf - tlb) * std::exp(-s2 * zg2);
		ta = tt;
		tz = tt;
		densu_temp = tz;

		if (alt < za) {
			/* calculate temperature below ZA temperature gradient at ZA from Bates profile */
			dta = (tinf - ta) * s2 * std::pow(((re + zlb) / (re + za)), 2.0);
			tgn1[0] = dta;
			tn1[0] = ta;
			z = (alt > zn1[mn1 - 1]) ? alt : zn1[mn1 - 1];
			mn = mn1;
			z1 = zn1[0];
			z2 = zn1[mn - 1];
			t1 = tn1[0];
			t2 = tn1[mn - 1];

			/* geo-potential difference from z1 */
			zg = zeta(z, z1);
			zgdif = zeta(z2, z1);

			/* set up spline nodes */
			for (int k = 0; k < mn; k++) {
				xs[k] = zeta(zn1[k], z1) / zgdif;
				ys[k] = 1.0 / tn1[k];
			}

			/* end node derivatives */
			yd1 = -tgn1[0] / (t1 * t1) * zgdif;
			yd2 = -tgn1[1] / (t2 * t2) * zgdif * std::pow(((re + z2) / (re + z1)), 2.0);

			/* calculate spline coefficients */
			spline(xs, ys, mn, yd1, yd2, y2out);
			x = zg / zgdif;
			splint(xs, ys, y2out, mn, x, y);
			/* temperature at altitude */
			tz = 1.0 / y;
			densu_temp = tz;
		}

		if (xm == 0) return densu_temp;

		/* calculate density above za */
		glb = gsurf / std::pow((1.0 + zlb / re), 2.0);
		gamma = xm * glb / (s2 * rgas * tinf);
		expl = std::exp(-s2 * gamma * zg2);
		if (expl > 50.0) expl = 50.0;
		if (tt <= 0) expl = 50.0;

		/* density at altitude */
		densa = dlb * std::pow((tlb / tt), ((1.0 + alpha + gamma))) * expl;
		densu_temp = densa;
		if (alt >= za) return densu_temp;

		/* calculate density below za */
		glb = gsurf / std::pow((1.0 + z1 / re), 2.0);
		gamm = xm * glb * zgdif / rgas;

		/* integrate spline temperatures */
		splini(xs, ys, y2out, mn, x, yi);
		expl = gamm * yi;
		if (expl > 50.0) expl = 50.0;
		if (tz <= 0) expl = 50.0;

		/* density at altitude */
		densu_temp = densu_temp * std::pow((t1 / tz), (1.0 + alpha)) * std::exp(-expl);
		return densu_temp;
	}

	double Nrlmsise::globe7(double *p, NrlmsiseInput &input, NrlmsiseConfig &flags) {
		double t[15];
		double apd;
		double tloc;
		double c, s, c2, c4, s2;
		double cd32, cd18, cd14, cd39;
		double df;
		double f1, f2;
		double tinf;

		tloc = input.lst;
		std::fill_n(t, 14, 0);

		/* calculate legendre polynomials */
		c = Degree(input.g_lat).sin();
		s = Degree(input.g_lat).cos();
		c2 = c * c;
		c4 = c2 * c2;
		s2 = s * s;

		plg[0][1] = c;
		plg[0][2] = 0.5 * (3.0 * c2 - 1.0);
		plg[0][3] = 0.5 * (5.0 * c * c2 - 3.0 * c);
		plg[0][4] = (35.0 * c4 - 30.0 * c2 + 3.0) / 8.0;
		plg[0][5] = (63.0 * c2 * c2 * c - 70.0 * c2 * c + 15.0 * c) / 8.0;
		plg[0][6] = (11.0 * c * plg[0][5] - 5.0 * plg[0][4]) / 6.0;
		/*      plg[0][7] = (13.0*c*plg[0][6] - 6.0*plg[0][5])/7.0; */
		plg[1][1] = s;
		plg[1][2] = 3.0 * c * s;
		plg[1][3] = 1.5 * (5.0 * c2 - 1.0) * s;
		plg[1][4] = 2.5 * (7.0 * c2 * c - 3.0 * c) * s;
		plg[1][5] = 1.875 * (21.0 * c4 - 14.0 * c2 + 1.0) * s;
		plg[1][6] = (11.0 * c * plg[1][5] - 6.0 * plg[1][4]) / 5.0;
		/*      plg[1][7] = (13.0*c*plg[1][6]-7.0*plg[1][5])/6.0; */
		/*      plg[1][8] = (15.0*c*plg[1][7]-8.0*plg[1][6])/7.0; */
		plg[2][2] = 3.0 * s2;
		plg[2][3] = 15.0 * s2 * c;
		plg[2][4] = 7.5 * (7.0 * c2 - 1.0) * s2;
		plg[2][5] = 3.0 * c * plg[2][4] - 2.0 * plg[2][3];
		plg[2][6] = (11.0 * c * plg[2][5] - 7.0 * plg[2][4]) / 4.0;
		plg[2][7] = (13.0 * c * plg[2][6] - 8.0 * plg[2][5]) / 5.0;
		plg[3][3] = 15.0 * s2 * s;
		plg[3][4] = 105.0 * s2 * s * c;
		plg[3][5] = (9.0 * c * plg[3][4] - 7. * plg[3][3]) / 2.0;
		plg[3][6] = (11.0 * c * plg[3][5] - 8. * plg[3][4]) / 3.0;

		if (!(((flags.sw[7] == 0) && (flags.sw[8] == 0)) && (flags.sw[14] == 0))) {
			stloc = HourAngle(tloc).sin();
			ctloc = HourAngle(tloc).cos();
			s2tloc = HourAngle(2.0 * tloc).sin();
			c2tloc = HourAngle(2.0 * tloc).cos();
			s3tloc = HourAngle(3.0 * tloc).sin();
			c3tloc = HourAngle(3.0 * tloc).cos();
		}

		cd32 = DoyAngle(input.doy - p[31]).cos();
		cd18 = DoyAngle(2.0 * (input.doy - p[17])).cos();
		cd14 = DoyAngle(input.doy - p[13]).cos();
		cd39 = DoyAngle(2.0 * (input.doy - p[38])).cos();

		/* F10.7 EFFECT */
		df = input.f107 - input.f107A;
		dfa = input.f107A - 150.0;
		t[0] = p[19] * df * (1.0 + p[59] * dfa) + p[20] * df * df + p[21] * dfa + p[29] * std::pow(dfa, 2.0);
		f1 = 1.0 + (p[47] * dfa + p[19] * df + p[20] * df * df) * flags.swc[1];
		f2 = 1.0 + (p[49] * dfa + p[19] * df + p[20] * df * df) * flags.swc[1];

		/*  TIME INDEPENDENT */
		t[1] = (p[1] * plg[0][2] + p[2] * plg[0][4] + p[22] * plg[0][6]) + (p[14] * plg[0][2]) * dfa * flags.swc[1] + p[26] * plg[0][1];

		/*  SYMMETRICAL ANNUAL */
		t[2] = p[18] * cd32;

		/*  SYMMETRICAL SEMIANNUAL */
		t[3] = (p[15] + p[16] * plg[0][2]) * cd18;

		/*  ASYMMETRICAL ANNUAL */
		t[4] = f1 * (p[9] * plg[0][1] + p[10] * plg[0][3]) * cd14;

		/*  ASYMMETRICAL SEMIANNUAL */
		t[5] = p[37] * plg[0][1] * cd39;

		/* DIURNAL */
		if (flags.sw[7]) {
			double t71, t72;
			t71 = (p[11] * plg[1][2]) * cd14 * flags.swc[5];
			t72 = (p[12] * plg[1][2]) * cd14 * flags.swc[5];
			t[6] = f2 * ((p[3] * plg[1][1] + p[4] * plg[1][3] + p[27] * plg[1][5] + t71) * ctloc +
						 (p[6] * plg[1][1] + p[7] * plg[1][3] + p[28] * plg[1][5] + t72) * stloc);
		}

		/* SEMIDIURNAL */
		if (flags.sw[8]) {
			double t81, t82;
			t81 = (p[23] * plg[2][3] + p[35] * plg[2][5]) * cd14 * flags.swc[5];
			t82 = (p[33] * plg[2][3] + p[36] * plg[2][5]) * cd14 * flags.swc[5];
			t[7] = f2 * ((p[5] * plg[2][2] + p[41] * plg[2][4] + t81) * c2tloc + (p[8] * plg[2][2] + p[42] * plg[2][4] + t82) * s2tloc);
		}

		/* TERDIURNAL */
		if (flags.sw[14]) {
			t[13] = f2 * ((p[39] * plg[3][3] + (p[93] * plg[3][4] + p[46] * plg[3][6]) * cd14 * flags.swc[5]) * s3tloc +
						  (p[40] * plg[3][3] + (p[94] * plg[3][4] + p[48] * plg[3][6]) * cd14 * flags.swc[5]) * c3tloc);
		}

		/* magnetic activity based on daily ap */
		if (flags.sw[9] == -1) {
			ApArray &ap = input.ap_a;
			if (p[51] != 0) {
				double exp1;
				exp1 = std::exp(-10800.0 * std::sqrt(p[51] * p[51]) / (1.0 + p[138] * (45.0 - std::sqrt(input.g_lat * input.g_lat))));
				if (exp1 > 0.99999) exp1 = 0.99999;
				if (p[24] < 1.0E-4) p[24] = 1.0E-4;
				apt[0] = sg0(exp1, p, ap.a);
				/* apt[1]=sg2(exp1,p,ap.a);
				   apt[2]=sg0(exp2,p,ap.a);
				   apt[3]=sg2(exp2,p,ap.a);
				*/
				if (flags.sw[9]) {
					t[8] = apt[0] *
						   (p[50] + p[96] * plg[0][2] + p[54] * plg[0][4] +
							(p[125] * plg[0][1] + p[126] * plg[0][3] + p[127] * plg[0][5]) * cd14 * flags.swc[5] +
							(p[128] * plg[1][1] + p[129] * plg[1][3] + p[130] * plg[1][5]) * flags.swc[7] * HourAngle(tloc - p[131]).cos());
				}
			}
		} else {
			double p44, p45;
			apd = input.ap - 4.0;
			p44 = p[43];
			p45 = p[44];
			if (p44 < 0) p44 = 1.0E-5;
			apdf = apd + (p45 - 1.0) * (apd + (std::exp(-p44 * apd) - 1.0) / p44);
			if (flags.sw[9]) {
				t[8] =
				  apdf * (p[32] + p[45] * plg[0][2] + p[34] * plg[0][4] +
						  (p[100] * plg[0][1] + p[101] * plg[0][3] + p[102] * plg[0][5]) * cd14 * flags.swc[5] +
						  (p[121] * plg[1][1] + p[122] * plg[1][3] + p[123] * plg[1][5]) * flags.swc[7] * HourAngle(tloc - p[124]).cos());
			}
		}

		if ((flags.sw[10]) && (input.g_long > -1000.0)) {

			/* longitudinal */
			if (flags.sw[11]) {
				t[10] = (1.0 + p[80] * dfa * flags.swc[1]) *
						((p[64] * plg[1][2] + p[65] * plg[1][4] + p[66] * plg[1][6] + p[103] * plg[1][1] + p[104] * plg[1][3] +
						  p[105] * plg[1][5] + flags.swc[5] * (p[109] * plg[1][1] + p[110] * plg[1][3] + p[111] * plg[1][5]) * cd14) *
						   Degree(input.g_long).cos() +
						 (p[90] * plg[1][2] + p[91] * plg[1][4] + p[92] * plg[1][6] + p[106] * plg[1][1] + p[107] * plg[1][3] +
						  p[108] * plg[1][5] + flags.swc[5] * (p[112] * plg[1][1] + p[113] * plg[1][3] + p[114] * plg[1][5]) * cd14) *
						   Degree(input.g_long).sin());
			}

			/* ut and mixed ut, longitude */
			if (flags.sw[12]) {
				t[11] = (1.0 + p[95] * plg[0][1]) * (1.0 + p[81] * dfa * flags.swc[1]) * (1.0 + p[119] * plg[0][1] * flags.swc[5] * cd14) *
						((p[68] * plg[0][1] + p[69] * plg[0][3] + p[70] * plg[0][5]) *
						 NormalizedAngle((input.sec - p[71]) / constant::seconds_per_day).cos());
				t[11] += flags.swc[11] * (p[76] * plg[2][3] + p[77] * plg[2][5] + p[78] * plg[2][7]) *
						 (NormalizedAngle((input.sec - p[79]) / constant::seconds_per_day) + 2.0 * Degree(input.g_long)).cos() *
						 (1.0 + p[137] * dfa * flags.swc[1]);
			}

			/* ut, longitude magnetic activity */
			if (flags.sw[13]) {
				if (flags.sw[9] == -1) {
					if (p[51]) {
						t[12] = apt[0] * flags.swc[11] * (1. + p[132] * plg[0][1]) *
								  ((p[52] * plg[1][2] + p[98] * plg[1][4] + p[67] * plg[1][6]) * Degree(input.g_long - p[97]).cos()) +
								apt[0] * flags.swc[11] * flags.swc[5] * (p[133] * plg[1][1] + p[134] * plg[1][3] + p[135] * plg[1][5]) *
								  cd14 * Degree(input.g_long - p[136]).cos() +
								apt[0] * flags.swc[12] * (p[55] * plg[0][1] + p[56] * plg[0][3] + p[57] * plg[0][5]) *
								  NormalizedAngle((input.sec - p[58]) / constant::seconds_per_day).cos();
					}
				} else {
					t[12] = apdf * flags.swc[11] * (1.0 + p[120] * plg[0][1]) *
							  ((p[60] * plg[1][2] + p[61] * plg[1][4] + p[62] * plg[1][6]) * Degree(input.g_long - p[63]).cos()) +
							apdf * flags.swc[11] * flags.swc[5] * (p[115] * plg[1][1] + p[116] * plg[1][3] + p[117] * plg[1][5]) * cd14 *
							  Degree(input.g_long - p[118]).cos() +
							apdf * flags.swc[12] * (p[83] * plg[0][1] + p[84] * plg[0][3] + p[85] * plg[0][5]) *
							  NormalizedAngle((input.sec - p[75]) / constant::seconds_per_day).cos();
				}
			}
		}

		/* parms not used: 82, 89, 99, 139-149 */
		tinf = p[30];
		for (int i = 0; i < 14; i++) tinf = tinf + std::fabs(flags.sw[i + 1]) * t[i];
		return tinf;
	}

	double Nrlmsise::glob7s(double *p, NrlmsiseInput &input, NrlmsiseConfig &flags) {
		/*    VERSION OF GLOBE FOR LOWER ATMOSPHERE 10/26/99  */
		constexpr double pset = 2.0;
		double t[14];
		double tt;
		double cd32, cd18, cd14, cd39;

		/* confirm parameter set */
		if (p[99] == 0) p[99] = pset;
		if (p[99] != pset) {
			throw AtmosModelException("Incorrect Low Atmosphere Globe model settings.", AtmosModelException::InvalidValue);
			return -1;
		}

		std::fill_n(t, 14, 0.0);

		cd32 = DoyAngle(input.doy - p[31]).cos();
		cd18 = DoyAngle(2.0 * (input.doy - p[17])).cos();
		cd14 = DoyAngle(input.doy - p[13]).cos();
		cd39 = DoyAngle(2.0 * (input.doy - p[38])).cos();

		/* F10.7 */
		t[0] = p[21] * dfa;

		/* Time independent */
		t[1] = p[1] * plg[0][2] + p[2] * plg[0][4] + p[22] * plg[0][6] + p[26] * plg[0][1] + p[14] * plg[0][3] + p[59] * plg[0][5];

		/* Symmetrical annual */
		t[2] = (p[18] + p[47] * plg[0][2] + p[29] * plg[0][4]) * cd32;

		/* Symmetrical semiannual */
		t[3] = (p[15] + p[16] * plg[0][2] + p[30] * plg[0][4]) * cd18;

		/* Asymmetrical annual */
		t[4] = (p[9] * plg[0][1] + p[10] * plg[0][3] + p[20] * plg[0][5]) * cd14;

		/* Asymmetrical semiannual */
		t[5] = (p[37] * plg[0][1]) * cd39;

		/* Diurnal */
		if (flags.sw[7]) {
			double t71, t72;
			t71 = p[11] * plg[1][2] * cd14 * flags.swc[5];
			t72 = p[12] * plg[1][2] * cd14 * flags.swc[5];
			t[6] = ((p[3] * plg[1][1] + p[4] * plg[1][3] + t71) * ctloc + (p[6] * plg[1][1] + p[7] * plg[1][3] + t72) * stloc);
		}

		/* Semidiurnal */
		if (flags.sw[8]) {
			double t81, t82;
			t81 = (p[23] * plg[2][3] + p[35] * plg[2][5]) * cd14 * flags.swc[5];
			t82 = (p[33] * plg[2][3] + p[36] * plg[2][5]) * cd14 * flags.swc[5];
			t[7] = ((p[5] * plg[2][2] + p[41] * plg[2][4] + t81) * c2tloc + (p[8] * plg[2][2] + p[42] * plg[2][4] + t82) * s2tloc);
		}

		/* Terdiurnal effects */
		if (flags.sw[14]) {
			t[13] = p[39] * plg[3][3] * s3tloc + p[40] * plg[3][3] * c3tloc;
		}

		/* Magnetic activity */
		if (flags.sw[9]) {
			if (flags.sw[9] == 1) t[8] = apdf * (p[32] + p[45] * plg[0][2] * flags.swc[2]);
			if (flags.sw[9] == -1) t[8] = (p[50] * apt[0] + p[96] * plg[0][2] * apt[0] * flags.swc[2]);
		}

		/* longitudinal */
		if (!((flags.sw[10] == 0) || (flags.sw[11] == 0) || (input.g_long <= -1000.0))) {
			t[10] =
			  (1.0 +
			   plg[0][1] * (p[80] * flags.swc[5] * DoyAngle(input.doy - p[81]).cos() +
							p[85] * flags.swc[6] * DoyAngle(2.0 * (input.doy - p[86])).cos()) +
			   p[83] * flags.swc[3] * DoyAngle(input.doy - p[84]).cos() +
			   p[87] * flags.swc[4] * DoyAngle(2.0 * (input.doy - p[88])).cos()) *
			  ((p[64] * plg[1][2] + p[65] * plg[1][4] + p[66] * plg[1][6] + p[74] * plg[1][1] + p[75] * plg[1][3] + p[76] * plg[1][5]) *
				 Degree(input.g_long).cos() +
			   (p[90] * plg[1][2] + p[91] * plg[1][4] + p[92] * plg[1][6] + p[77] * plg[1][1] + p[78] * plg[1][3] + p[79] * plg[1][5]) *
				 Degree(input.g_long).sin());
		}
		tt = 0;
		for (int i = 0; i < 14; i++) tt += std::fabs(flags.sw[i + 1]) * t[i];
		return tt;
	}

	void Nrlmsise::gtd7d(NrlmsiseInput &input, NrlmsiseConfig &flags, NrlmsiseOutput &output) {
		gtd7(input, flags, output);

		output.d[5] = 1.66E-24 * (4.0 * output.d[0] + 16.0 * output.d[1] + 28.0 * output.d[2] + 32.0 * output.d[3] + 40.0 * output.d[4] +
								  output.d[6] + 14.0 * output.d[7] + 16.0 * output.d[8]);
		if (flags.sw[0]) output.d[5] /= 1000;
	}

	void Nrlmsise::gtd7(NrlmsiseInput &input, NrlmsiseConfig &flags, NrlmsiseOutput &output) {
		constexpr double zmix = 62.5;
		double zn2[4] = {72.5, 55.0, 45.0, 32.5};
		double zn3[5] = {32.5, 20.0, 15.0, 10.0, 0.0};

		NrlmsiseOutput soutput;

		/* Configure flags */
		tselec(flags);

		/* Latitude variation of gravity (none for sw[2]=0) */
		double xlat = (flags.sw[2] == 0) ? 45.0 : input.g_lat;
		glatf(xlat, gsurf, re);

		/* Thermosphere / mesosphere (above zn2[0]) */
		double altt = (input.alt > zn2[0]) ? input.alt : zn2[0];
		std::swap(input.alt, altt);
		gts7(input, flags, soutput);
		std::swap(input.alt, altt);

		output.t[0] = soutput.t[0];
		output.t[1] = soutput.t[1];

		if (input.alt >= zn2[0]) {
			std::copy(soutput.d, soutput.d + 9, output.d);
			return;
		}

		/**
		 * Lower mesosphere / upper stratosphere (between zn3[0] and zn2[0])
		 *  Temperature at nodes and gradients at end nodes
		 *  Inverse temperature a linear function of spherical harmonics
		 **/
		meso_tgn2[0] = meso_tgn1[1];
		meso_tn2[0] = meso_tn1[4];
		meso_tn2[1] = pma[0][0] * pavgm[0] / (1.0 - flags.sw[20] * glob7s(pma[0], input, flags));
		meso_tn2[2] = pma[1][0] * pavgm[1] / (1.0 - flags.sw[20] * glob7s(pma[1], input, flags));
		meso_tn2[3] = pma[2][0] * pavgm[2] / (1.0 - flags.sw[20] * flags.sw[22] * glob7s(pma[2], input, flags));
		meso_tgn2[1] = pavgm[8] * pma[9][0] * (1.0 + flags.sw[20] * flags.sw[22] * glob7s(pma[9], input, flags)) * meso_tn2[3] *
					   meso_tn2[3] / (std::pow((pma[2][0] * pavgm[2]), 2.0));
		meso_tn3[0] = meso_tn2[3];

		/**
		 * Lower stratosphere and troposphere
		 * Temperature at nodes and gradients at end nodes
		 * Inverse temperature a linear function of spherical harmonics
		 **/
		if (input.alt < zn3[0]) {
			meso_tgn3[0] = meso_tgn2[1];
			meso_tn3[1] = pma[3][0] * pavgm[3] / (1.0 - flags.sw[22] * glob7s(pma[3], input, flags));
			meso_tn3[2] = pma[4][0] * pavgm[4] / (1.0 - flags.sw[22] * glob7s(pma[4], input, flags));
			meso_tn3[3] = pma[5][0] * pavgm[5] / (1.0 - flags.sw[22] * glob7s(pma[5], input, flags));
			meso_tn3[4] = pma[6][0] * pavgm[6] / (1.0 - flags.sw[22] * glob7s(pma[6], input, flags));
			meso_tgn3[1] = pma[7][0] * pavgm[7] * (1.0 + flags.sw[22] * glob7s(pma[7], input, flags)) * meso_tn3[4] * meso_tn3[4] /
						   (pow((pma[6][0] * pavgm[6]), 2.0));
		}

		/* Linear transition to full mixing below zn2[0] */
		double dm28m = (flags.sw[0]) ? dm28 * 1.0E6 : dm28; // metric adjustment
		double dmc = (input.alt > zmix) ? 1.0 - (zn2[0] - input.alt) / (zn2[0] - zmix) : 0.0;
		double dz28 = soutput.d[2];
		double dmr = 0;
		double tz = 0;

		/* N2 density */
		dmr = soutput.d[2] / dm28m - 1.0;
		output.d[2] =
		  densm(input.alt, dm28m, pdm[2][4], tz, std::size(zn3), zn3, meso_tn3, meso_tgn3, std::size(zn2), zn2, meso_tn2, meso_tgn2);
		output.d[2] = output.d[2] * (1.0 + dmr * dmc);

		/* He density */
		dmr = soutput.d[0] / (dz28 * pdm[0][1]) - 1.0;
		output.d[0] = output.d[2] * pdm[0][1] * (1.0 + dmr * dmc);

		/* O density */
		output.d[1] = 0;
		output.d[8] = 0;

		/* O2 density */
		dmr = soutput.d[3] / (dz28 * pdm[3][1]) - 1.0;
		output.d[3] = output.d[2] * pdm[3][1] * (1.0 + dmr * dmc);

		/* Ar density */
		dmr = soutput.d[4] / (dz28 * pdm[4][1]) - 1.0;
		output.d[4] = output.d[2] * pdm[4][1] * (1.0 + dmr * dmc);

		/* Hydrogen density */
		output.d[6] = 0;

		/* Atomic nitrogen density */
		output.d[7] = 0;

		/* Total mass density */
		output.d[5] = 1.66E-24 * (4.0 * output.d[0] + 16.0 * output.d[1] + 28.0 * output.d[2] + 32.0 * output.d[3] + 40.0 * output.d[4] +
								  output.d[6] + 14.0 * output.d[7]);
		if (flags.sw[0]) output.d[5] = output.d[5] / 1000;

		/* temperature at altitude */
		dd = densm(input.alt, 1.0, 0, tz, std::size(zn3), zn3, meso_tn3, meso_tgn3, std::size(zn2), zn2, meso_tn2, meso_tgn2);
		output.t[1] = tz;
	}

	void Nrlmsise::ghp7(NrlmsiseInput &input, NrlmsiseConfig &flags, NrlmsiseOutput &output, double press) {
		constexpr double bm = 1.3806E-19;
		constexpr double rgas = 831.4;
		constexpr double test = 0.00043;
		constexpr double ltest = 12;

		double pl, p;
		double zi;
		double z;
		double cl, cl2;
		double ca, cd;
		double xn, xm, diff;
		double g, sh;

		pl = std::log10(press);
		if (pl >= -5.0) {
			if (pl > 2.5) {
				zi = 18.06 * (3.00 - pl);
			} else if ((pl > 0.075) && (pl <= 2.5)) {
				zi = 14.98 * (3.08 - pl);
			} else if ((pl > -1) && (pl <= 0.075)) {
				zi = 17.80 * (2.72 - pl);
			} else if ((pl > -2) && (pl <= -1)) {
				zi = 14.28 * (3.64 - pl);
			} else if ((pl > -4) && (pl <= -2)) {
				zi = 12.72 * (4.32 - pl);
			} else {
				zi = 25.3 * (0.11 - pl);
			}

			cl = input.g_lat / 90.0;
			cl2 = cl * cl;

			if (input.doy < 182)
				cd = (1.0 - (double)input.doy) / 91.25;
			else
				cd = ((double)input.doy) / 91.25 - 3.0;
			ca = 0;

			if ((pl > -1.11) && (pl <= -0.23)) ca = 1.0;
			if (pl > -0.23) ca = (2.79 - pl) / (2.79 + 0.23);
			if ((pl <= -1.11) && (pl > -3)) ca = (-2.93 - pl) / (-2.93 + 1.11);

			z = zi - 4.87 * cl * cd * ca - 1.64 * cl2 * ca + 0.31 * ca * cl;
		} else {
			z = 22.0 * std::pow((pl + 4.0), 2.0) + 110.0;
		}

		/* iteration  loop */
		double l = 0;

		do {
			l++;
			input.alt = z;

			gtd7(input, flags, output);

			z = input.alt;
			xn = output.d[0] + output.d[1] + output.d[2] + output.d[3] + output.d[4] + output.d[6] + output.d[7];
			p = bm * xn * output.t[1];

			if (flags.sw[0]) {
				p = p * 1.0E-6;
			}

			diff = pl - std::log10(p);
			if (std::sqrt(diff * diff) < test) return;

			if (l == ltest) {
				throw AtmosModelException("Iteration results do not converge.", AtmosModelException::MathmaticalError);
				return;
			}

			xm = output.d[5] / xn / 1.66E-24;
			if (flags.sw[0]) xm = xm * 1.0E3;

			g = gsurf / (std::pow((1.0 + z / re), 2.0));
			sh = rgas * output.t[1] / (xm * g);

			/* new altitude estimate using scale height */
			if (l < 6) {
				z = z - sh * diff * 2.302;
			} else {
				z = z - sh * diff;
			}
		} while (1 == 1);
	}

	void Nrlmsise::gts7(NrlmsiseInput &input, NrlmsiseConfig &flags, NrlmsiseOutput &output) {
		double za;
		double ddum, z;
		double zn1[5] = {120.0, 110.0, 100.0, 90.0, 72.5};
		double tinf;
		double g0;
		double tlb;
		double s;
		double db01, db04, db14, db16, db28, db32, db40;
		double zh28, zh04, zh16, zh32, zh40, zh01, zh14;
		double zhm28, zhm04, zhm16, zhm32, zhm40, zhm01, zhm14;
		double xmd;
		double b28, b04, b16, b32, b40, b01, b14;
		double tz;
		double g28, g4, g16, g32, g40, g1, g14;
		double zhf, xmm;
		double zc04, zc16, zc32, zc40, zc01, zc14;
		double hc04, hc16, hc32, hc40, hc01, hc14;
		double hcc16, hcc32, hcc01, hcc14;
		double zcc16, zcc32, zcc01, zcc14;
		double rc16, rc32, rc01, rc14;
		double rl;
		double g16h, db16h, tho, zsht, zmho, zsho;
		double alpha[9] = {-0.38, 0.0, 0.0, 0.0, 0.17, 0.0, -0.38, 0.0, 0.0};
		double altl[8] = {200.0, 300.0, 160.0, 250.0, 240.0, 450.0, 320.0, 450.0};
		double dd;
		double hc216, hcc232;

		za = pdl[1][15];
		zn1[0] = za;
		std::fill_n(output.d, std::size(output.d), 0);

		/* TINF variations not important belloe ZA or ZN[0] */
		if (input.alt > zn1[0])
			tinf = ptm[0] * pt[0] * (1.0 + flags.sw[16] * globe7(pt, input, flags));
		else
			tinf = ptm[0] * pt[0];
		output.t[0] = tinf;

		/* Gradient variations hot important bellow zn1[4] */
		if (input.alt > zn1[4])
			g0 = ptm[3] * ps[0] * (1.0 + flags.sw[19] * globe7(ps, input, flags));
		else
			g0 = ptm[3] * ps[0];
		tlb = ptm[1] * (1.0 + flags.sw[17] * globe7(pd[3], input, flags)) * pd[3][0];
		s = g0 / (tinf - tlb);

		/* Lower thermosphere temp variations not significant for density above 300 km */
		if (input.alt < 300.0) {
			meso_tn1[1] = ptm[6] * ptl[0][0] / (1.0 - flags.sw[18] * glob7s(ptl[0], input, flags));
			meso_tn1[2] = ptm[2] * ptl[1][0] / (1.0 - flags.sw[18] * glob7s(ptl[1], input, flags));
			meso_tn1[3] = ptm[7] * ptl[2][0] / (1.0 - flags.sw[18] * glob7s(ptl[2], input, flags));
			meso_tn1[4] = ptm[4] * ptl[3][0] / (1.0 - flags.sw[18] * flags.sw[20] * glob7s(ptl[3], input, flags));
			meso_tgn1[1] = ptm[8] * pma[8][0] * (1.0 + flags.sw[18] * flags.sw[20] * glob7s(pma[8], input, flags)) * meso_tn1[4] *
						   meso_tn1[4] / (std::pow((ptm[4] * ptl[3][0]), 2.0));
		} else {
			meso_tn1[1] = ptm[6] * ptl[0][0];
			meso_tn1[2] = ptm[2] * ptl[1][0];
			meso_tn1[3] = ptm[7] * ptl[2][0];
			meso_tn1[4] = ptm[4] * ptl[3][0];
			meso_tgn1[1] = ptm[8] * pma[8][0] * meso_tn1[4] * meso_tn1[4] / (std::pow((ptm[4] * ptl[3][0]), 2.0));
		}

		/* N2 variation factor at Zlb */
		g28 = flags.sw[21] * globe7(pd[2], input, flags);

		/* Varioation of turbopause height */
		zhf = pdl[1][24] * (1.0 + flags.sw[5] * pdl[0][24] * Degree(input.g_lat).sin() * DoyAngle(input.doy - pt[13]).cos());
		output.t[0] = tinf;
		xmm = pdm[2][4];
		z = input.alt;

		/* Molecular nitrogen (N2) density*/
		{

			/* Diffusive density at Zlb */
			db28 = pdm[2][0] * std::exp(g28) * pd[2][0];

			/* Diffusive density at Alt */
			output.d[2] = densu(z, db28, tinf, tlb, 28.0, alpha[2], output.t[1], ptm[5], s, std::size(zn1), zn1, meso_tn1, meso_tgn1);
			dd = output.d[2];

			/* Turbopause */
			zh28 = pdm[2][2] * zhf;
			zhm28 = pdm[2][3] * pdl[1][5];
			xmd = 28.0 - xmm;

			/* Mixed density at Zlb */
			b28 = densu(zh28, db28, tinf, tlb, xmd, (alpha[2] - 1.0), tz, ptm[5], s, std::size(zn1), zn1, meso_tn1, meso_tgn1);
			if ((flags.sw[15]) && (z <= altl[2])) {
				/* Mixed density at Alt */
				dm28 = densu(z, b28, tinf, tlb, xmm, alpha[2], tz, ptm[5], s, std::size(zn1), zn1, meso_tn1, meso_tgn1);
				/* Net density at Alt */
				output.d[2] = dnet(output.d[2], dm28, zhm28, xmm, 28.0);
			}
		}

		/* Atomic helium density */
		{
			/*   Density variation factor at Zlb */
			g4 = flags.sw[21] * globe7(pd[0], input, flags);

			/*  Diffusive density at Zlb */
			db04 = pdm[0][0] * std::exp(g4) * pd[0][0];

			/*  Diffusive density at Alt */
			output.d[0] = densu(z, db04, tinf, tlb, 4., alpha[0], output.t[1], ptm[5], s, std::size(zn1), zn1, meso_tn1, meso_tgn1);
			dd = output.d[0];
			if ((flags.sw[15]) && (z < altl[0])) {
				/*  Turbopause */
				zh04 = pdm[0][2];

				/*  Mixed density at Zlb */
				b04 =
				  densu(zh04, db04, tinf, tlb, 4. - xmm, alpha[0] - 1., output.t[1], ptm[5], s, std::size(zn1), zn1, meso_tn1, meso_tgn1);

				/*  Mixed density at Alt */
				dm04 = densu(z, b04, tinf, tlb, xmm, 0., output.t[1], ptm[5], s, std::size(zn1), zn1, meso_tn1, meso_tgn1);
				zhm04 = zhm28;

				/*  Net density at Alt */
				output.d[0] = dnet(output.d[0], dm04, zhm04, xmm, 4.);

				/*  Correction to specified mixing ratio at ground */
				rl = std::log(b28 * pdm[0][1] / b04);
				zc04 = pdm[0][4] * pdl[1][0];
				hc04 = pdm[0][5] * pdl[1][1];

				/*  Net density corrected at Alt */
				output.d[0] = output.d[0] * ccor(z, rl, hc04, zc04);
			}
		}

		/* Atomic oxygen (O) density */
		{
			/* Density variation factor at Zlb */
			g16 = flags.sw[21] * globe7(pd[1], input, flags);

			/* Diffusive density at Zlb */
			db16 = pdm[1][0] * std::exp(g16) * pd[1][0];

			/* Diffusive density at Alt */
			output.d[1] = densu(z, db16, tinf, tlb, 16., alpha[1], output.t[1], ptm[5], s, std::size(zn1), zn1, meso_tn1, meso_tgn1);
			dd = output.d[1];

			if ((flags.sw[15]) && (z <= altl[1])) {
				/* Turbopause */
				zh16 = pdm[1][2];

				/* Mixed density at Zlb */
				b16 = densu(zh16, db16, tinf, tlb, 16.0 - xmm, (alpha[1] - 1.0), output.t[1], ptm[5], s, std::size(zn1), zn1, meso_tn1,
							meso_tgn1);

				/* Mixed density at Alt */
				dm16 = densu(z, b16, tinf, tlb, xmm, 0., output.t[1], ptm[5], s, std::size(zn1), zn1, meso_tn1, meso_tgn1);
				zhm16 = zhm28;

				/* Net density at Alt */
				output.d[1] = dnet(output.d[1], dm16, zhm16, xmm, 16.);
				rl = pdm[1][1] * pdl[1][16] * (1.0 + flags.sw[1] * pdl[0][23] * (input.f107A - 150.0));
				hc16 = pdm[1][5] * pdl[1][3];
				zc16 = pdm[1][4] * pdl[1][2];
				hc216 = pdm[1][5] * pdl[1][4];
				output.d[1] = output.d[1] * ccor2(z, rl, hc16, zc16, hc216);

				/* Chemistry correction */
				hcc16 = pdm[1][7] * pdl[1][13];
				zcc16 = pdm[1][6] * pdl[1][12];
				rc16 = pdm[1][3] * pdl[1][14];

				/* Net density corrected at Alt */
				output.d[1] = output.d[1] * ccor(z, rc16, hcc16, zcc16);
			}
		}

		/* Molecular oxygen (O2) density */
		{
			/* Density variation factor at Zlb */
			g32 = flags.sw[21] * globe7(pd[4], input, flags);

			/* Diffusive density at Zlb */
			db32 = pdm[3][0] * std::exp(g32) * pd[4][0];

			/* Diffusive density at Alt */
			output.d[3] = densu(z, db32, tinf, tlb, 32., alpha[3], output.t[1], ptm[5], s, std::size(zn1), zn1, meso_tn1, meso_tgn1);
			dd = output.d[3];

			if (flags.sw[15]) {
				if (z <= altl[3]) {
					/* Turbopause */
					zh32 = pdm[3][2];

					/* Mixed density at Zlb */
					b32 = densu(zh32, db32, tinf, tlb, 32. - xmm, alpha[3] - 1., output.t[1], ptm[5], s, std::size(zn1), zn1, meso_tn1,
								meso_tgn1);

					/* Mixed density at Alt */
					dm32 = densu(z, b32, tinf, tlb, xmm, 0., output.t[1], ptm[5], s, std::size(zn1), zn1, meso_tn1, meso_tgn1);
					zhm32 = zhm28;

					/* Net density at Alt */
					output.d[3] = dnet(output.d[3], dm32, zhm32, xmm, 32.);

					/* Correction to specified mixing ratio at ground */
					rl = std::log(b28 * pdm[3][1] / b32);
					hc32 = pdm[3][5] * pdl[1][7];
					zc32 = pdm[3][4] * pdl[1][6];
					output.d[3] = output.d[3] * ccor(z, rl, hc32, zc32);
				}

				/* Correction for general departure from diffusive equilibrium above Zlb */
				hcc32 = pdm[3][7] * pdl[1][22];
				hcc232 = pdm[3][7] * pdl[0][22];
				zcc32 = pdm[3][6] * pdl[1][21];
				rc32 = pdm[3][3] * pdl[1][23] * (1. + flags.sw[1] * pdl[0][23] * (input.f107A - 150.));

				/* Net density corrected at Alt */
				output.d[3] = output.d[3] * ccor2(z, rc32, hcc32, zcc32, hcc232);
			}
		}

		/* Atomic argon (Ar) density */
		{
			/* Density variation factor at Zlb */
			g40 = flags.sw[21] * globe7(pd[5], input, flags);

			/* Diffusive density at Zlb */
			db40 = pdm[4][0] * exp(g40) * pd[5][0];

			/* Diffusive density at Alt */
			output.d[4] = densu(z, db40, tinf, tlb, 40., alpha[4], output.t[1], ptm[5], s, std::size(zn1), zn1, meso_tn1, meso_tgn1);
			dd = output.d[4];
			if ((flags.sw[15]) && (z <= altl[4])) {
				/* Turbopause */
				zh40 = pdm[4][2];

				/* Mixed density at Zlb */
				b40 =
				  densu(zh40, db40, tinf, tlb, 40. - xmm, alpha[4] - 1., output.t[1], ptm[5], s, std::size(zn1), zn1, meso_tn1, meso_tgn1);

				/* Mixed density at Alt */
				dm40 = densu(z, b40, tinf, tlb, xmm, 0., output.t[1], ptm[5], s, std::size(zn1), zn1, meso_tn1, meso_tgn1);
				zhm40 = zhm28;

				/* Net density at Alt */
				output.d[4] = dnet(output.d[4], dm40, zhm40, xmm, 40.);

				/* Correction to specified mixing ratio at ground */
				rl = std::log(b28 * pdm[4][1] / b40);
				hc40 = pdm[4][5] * pdl[1][9];
				zc40 = pdm[4][4] * pdl[1][8];

				/* Net density corrected at Alt */
				output.d[4] = output.d[4] * ccor(z, rl, hc40, zc40);
			}
		}

		/* Atomic hydrogen (H) density */
		{
			/* Density variation factor at Zlb */
			g1 = flags.sw[21] * globe7(pd[6], input, flags);

			/* Diffusive density at Zlb */
			db01 = pdm[5][0] * std::exp(g1) * pd[6][0];

			/* Diffusive density at Alt */
			output.d[6] = densu(z, db01, tinf, tlb, 1., alpha[6], output.t[1], ptm[5], s, std::size(zn1), zn1, meso_tn1, meso_tgn1);
			dd = output.d[6];
			if ((flags.sw[15]) && (z <= altl[6])) {
				/* Turbopause */
				zh01 = pdm[5][2];

				/* Mixed density at Zlb */
				b01 =
				  densu(zh01, db01, tinf, tlb, 1. - xmm, alpha[6] - 1., output.t[1], ptm[5], s, std::size(zn1), zn1, meso_tn1, meso_tgn1);

				/* Mixed density at Alt */
				dm01 = densu(z, b01, tinf, tlb, xmm, 0., output.t[1], ptm[5], s, std::size(zn1), zn1, meso_tn1, meso_tgn1);
				zhm01 = zhm28;

				/* Net density at Alt */
				output.d[6] = dnet(output.d[6], dm01, zhm01, xmm, 1.);

				/* Correction to specified mixing ratio at ground */
				rl = std::log(b28 * pdm[5][1] * std::sqrt(pdl[1][17] * pdl[1][17]) / b01);
				hc01 = pdm[5][5] * pdl[1][11];
				zc01 = pdm[5][4] * pdl[1][10];
				output.d[6] = output.d[6] * ccor(z, rl, hc01, zc01);

				/* Chemistry correction */
				hcc01 = pdm[5][7] * pdl[1][19];
				zcc01 = pdm[5][6] * pdl[1][18];
				rc01 = pdm[5][3] * pdl[1][20];

				/* Net density corrected at Alt */
				output.d[6] = output.d[6] * ccor(z, rc01, hcc01, zcc01);
			}
		}

		/* Atomic nitorogen (N) density */
		{

			/* Density variation factor at Zlb */
			g14 = flags.sw[21] * globe7(pd[7], input, flags);

			/* Diffusive density at Zlb */
			db14 = pdm[6][0] * std::exp(g14) * pd[7][0];

			/* Diffusive density at Alt */
			output.d[7] = densu(z, db14, tinf, tlb, 14., alpha[7], output.t[1], ptm[5], s, std::size(zn1), zn1, meso_tn1, meso_tgn1);
			dd = output.d[7];

			if ((flags.sw[15]) && (z <= altl[7])) {
				/* Turbopause */
				zh14 = pdm[6][2];

				/* Mixed density at Zlb */
				b14 =
				  densu(zh14, db14, tinf, tlb, 14. - xmm, alpha[7] - 1., output.t[1], ptm[5], s, std::size(zn1), zn1, meso_tn1, meso_tgn1);

				/* Mixed density at Alt */
				dm14 = densu(z, b14, tinf, tlb, xmm, 0., output.t[1], ptm[5], s, std::size(zn1), zn1, meso_tn1, meso_tgn1);
				zhm14 = zhm28;

				/* Net density at Alt */
				output.d[7] = dnet(output.d[7], dm14, zhm14, xmm, 14.);

				/* Correction to specified mixing ratio at ground */
				rl = std::log(b28 * pdm[6][1] * std::sqrt(pdl[0][2] * pdl[0][2]) / b14);
				hc14 = pdm[6][5] * pdl[0][1];
				zc14 = pdm[6][4] * pdl[0][0];
				output.d[7] = output.d[7] * ccor(z, rl, hc14, zc14);

				/* Chemistry correction */
				hcc14 = pdm[6][7] * pdl[0][4];
				zcc14 = pdm[6][6] * pdl[0][3];
				rc14 = pdm[6][3] * pdl[0][5];

				/* Net density corrected at Alt */
				output.d[7] = output.d[7] * ccor(z, rc14, hcc14, zcc14);
			}
		}

		/* Anomalous oxygen (Hot O, O2-) density */
		{
			g16h = flags.sw[21] * globe7(pd[8], input, flags);
			db16h = pdm[7][0] * std::exp(g16h) * pd[8][0];
			tho = pdm[7][9] * pdl[0][6];
			dd = densu(z, db16h, tho, tho, 16., alpha[8], output.t[1], ptm[5], s, std::size(zn1), zn1, meso_tn1, meso_tgn1);
			zsht = pdm[7][5];
			zmho = pdm[7][4];
			zsho = scalh(zmho, 16.0, tho);
			output.d[8] = dd * std::exp(-zsht / zsho * (std::exp(-(z - zmho) / zsht) - 1.));

			/* total mass density */
			output.d[5] = 1.66E-24 * (4.0 * output.d[0] + 16.0 * output.d[1] + 28.0 * output.d[2] + 32.0 * output.d[3] +
									  40.0 * output.d[4] + output.d[6] + 14.0 * output.d[7]);

			/* temperature */
			z = std::sqrt(input.alt * input.alt);
			ddum = densu(z, 1.0, tinf, tlb, 0.0, 0.0, output.t[1], ptm[5], s, std::size(zn1), zn1, meso_tn1, meso_tgn1);

			(void)ddum; // silence gcc

			/* Unit conversion */
			if (flags.sw[0]) {
				for (int i = 0; i < 9; i++) output.d[i] = output.d[i] * 1.0E6;
				output.d[5] = output.d[5] / 1000;
			}
		}
	}
} // namespace internal

GEOATMOS_NAMESPACE_END