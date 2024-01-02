/**
 * @file Exception.hpp
 * @author fugu133
 * @brief
 * @version 0.1
 * @date 2023-11-26
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <stdexcept>
#include <string>

#include "Essential.hpp"
#include "Macro.hpp"

GEOATMOS_NAMESPACE_BEGIN

class BaseException : public std::runtime_error {
  public:
	BaseException() = delete;
	BaseException(const std::string& what_message, int error_code)
	  : std::runtime_error("[GeoAtmosException]: " + what_message), m_error_code(error_code) {}

	int getReturnCode() const { return m_error_code; }

  private:
	int m_error_code;
};

class DateTimeException : public BaseException {
  public:
	DateTimeException() = delete;
	DateTimeException(const std::string& what_message, int error_code) : BaseException(what_message, error_code) {}

	enum {
		InvalidYear,
		InvalidMonth,
		InvalidDay,
		InvalidHour,
		InvalidMinute,
		InvalidSecond,
		InvalidMicrosecond,
		InvalidDate,
		InvalidTime,
		InvalidDateTime,
		InvalidIso8601Format
	};
};

class AtmosModelException : public BaseException {
  public:
	AtmosModelException() = delete;
	AtmosModelException(const std::string& what_message, int error_code) : BaseException(what_message, error_code) {}

	enum {
		ValueOutOfRange,
		InvalidValue,
		MathmaticalError,
	};
};

GEOATMOS_NAMESPACE_END