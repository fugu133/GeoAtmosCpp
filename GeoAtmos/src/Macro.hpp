/**
 * @file Macro.hpp
 * @author fugu133
 * @brief マクロ魔術
 * @version 0.1
 * @date 2023-04-30
 * @copyright Copyright (c) 2023
 */

#pragma once

// clang-format off
#define GEOATMOS_VERSION_MAJOR 0
#define GEOATMOS_VERSION_MINOR 1
#define GEOATMOS_VERSION_PATCH 0

#define GEOATMOS_TO_STRING_EX(x) #x
#define GEOATMOS_TO_STRING(x) GEOATMOS_TO_STRING_EX(x)

#define GEOATMOS_VERSION_CONCAT_EX(major, minor, patch) v ## major.minor.patch
#define GEOATMOS_VERSION_CONCAT(major, minor, patch) GEOATMOS_VERSION_CONCAT_EX(major, minor, patch)

#define GEOATMOS_VERSION_EX \
	GEOATMOS_VERSION_CONCAT(GEOATMOS_VERSION_MAJOR, \
				      GEOATMOS_VERSION_MINOR, \
			              GEOATMOS_VERSION_PATCH)
#define GEOATMOS_VERSION GEOATMOS_VERSION_EX
#define GEOATMOS_VERSION_STR_EX GEOATMOS_TO_STRING(GEOATMOS_VERSION)
#define GEOATMOS_VERSION_STR GEOATMOS_VERSION_STR_EX

#define GEOATMOS_NAMESPACE_VERSION_TAG_CONCAT_EX(major, minor, patch) v ## major ## _ ## minor ## _ ## patch
#define GEOATMOS_NAMESPACE_VERSION_TAG_CONCAT(major, minor, patch) GEOATMOS_NAMESPACE_VERSION_TAG_CONCAT_EX(major, minor, patch)
#define GEOATMOS_NAMESPACE_VERSION_TAG_EX \
	GEOATMOS_NAMESPACE_VERSION_TAG_CONCAT(GEOATMOS_VERSION_MAJOR, \
						    GEOATMOS_VERSION_MINOR, \
					            GEOATMOS_VERSION_PATCH)
#define GEOATMOS_NAMESPACE_BASE_TAG geoatmos
#define GEOATMOS_NAMESPACE_VERSION_TAG GEOATMOS_NAMESPACE_VERSION_TAG_EX
#define GEOATMOS_NAMESPACE_BEGIN \
	namespace GEOATMOS_NAMESPACE_BASE_TAG {                   \
		inline namespace GEOATMOS_NAMESPACE_VERSION_TAG {
#define GEOATMOS_NAMESPACE_END \
	}                                               \
}

#define GEOATMOS_REQUEST_VERSION_CHECK(major, minor, patch) \
	(GEOATMOS_VERSION_MAJOR >= major && GEOATMOS_VERSION_MINOR >= minor && GEOATMOS_VERSION_PATCH >= patch)

#define GEOATMOS_REQUEST_VERSION_ASSERTION_MSG_STR(major, minor, patch) "GeoAtmos-Lib must be has version higher than " GEOATMOS_TO_STRING(GEOATMOS_VERSION_CONCAT(major, minor, patch))

#ifndef GEOATMOS_NO_ASSERTION
#define GEOATMOS_ASSRET_CONVERTER_REQUEST_VERSION(major, minor, patch) \
	static_assert(GEOATMOS_REQUEST_VERSION_CHECK(major, minor, patch), GEOATMOS_REQUEST_VERSION_ASSERTION_MSG_STR(major, minor, patch))
#else
	TELEMETRY_ASSRET_CONVERTER_REQUEST_VERSION(major, minor, patch)
#endif

#define GEOATMOS_CODE_GEN_CONCAT_EX(tag, type) tag ## _ ## type
#define GEOATMOS_CODE_GEN_CONCAT(tag, type) GEOATMOS_CODE_GEN_CONCAT_EX(tag, type)
#define GEOATMOS_CODE_GEN_TAG koyoh_acs_GEOATMOS_code_gen
#define GEOATMOS_CODE_GEN_RESULT_FUNCTION_NAME(x) GEOATMOS_CODE_GEN_CONCAT(GEOATMOS_CODE_GEN_TAG, x)
#define GEOATMOS_CODE_GEN_ARG_STR_T GEOATMOS_CODE_GEN_CONCAT(GEOATMOS_NAMESPACE_BASE_TAG, str_t)
#define GEOATMOS_CODE_GEN_ARG_OBJ_T GEOATMOS_CODE_GEN_CONCAT(GEOATMOS_NAMESPACE_BASE_TAG, obj_t)
#define GEOATMOS_CODE_GEN_ARG_PTR_T GEOATMOS_CODE_GEN_CONCAT(GEOATMOS_NAMESPACE_BASE_TAG, ptr_t)
#define GEOATMOS_CODE_GEN_ARG_OFS_T GEOATMOS_CODE_GEN_CONCAT(GEOATMOS_NAMESPACE_BASE_TAG, ofs_t)
#define GEOATMOS_CODE_GEN_ARG_IPT_T GEOATMOS_CODE_GEN_CONCAT(GEOATMOS_NAMESPACE_BASE_TAG, ipt_t)
#define GEOATMOS_CODE_GEN_ARG_OPT_T GEOATMOS_CODE_GEN_CONCAT(GEOATMOS_NAMESPACE_BASE_TAG, opt_t)
#define GEOATMOS_CODE_GEN_TEMPLATE_TYPE Type
#define GEOATMOS_CODE_GEN_TARGET_OBJ_NAME GEOATMOS_CODE_GEN_CONCAT(GEOATMOS_NAMESPACE_BASE_TAG, obj_name)
#define GEOATMOS_CODE_GEN_ARG_EXPAND( x ) x
#define GEOATMOS_CODE_GEN_ARG_GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, NAME,...) NAME
#define GEOATMOS_CODE_GEN_ARG_PASTE(...) GEOATMOS_CODE_GEN_ARG_EXPAND(GEOATMOS_CODE_GEN_ARG_GET_MACRO(__VA_ARGS__, \
        GEOATMOS_CODE_GEN_ARG_PASTE64, \
        GEOATMOS_CODE_GEN_ARG_PASTE63, \
        GEOATMOS_CODE_GEN_ARG_PASTE62, \
        GEOATMOS_CODE_GEN_ARG_PASTE61, \
        GEOATMOS_CODE_GEN_ARG_PASTE60, \
        GEOATMOS_CODE_GEN_ARG_PASTE59, \
        GEOATMOS_CODE_GEN_ARG_PASTE58, \
        GEOATMOS_CODE_GEN_ARG_PASTE57, \
        GEOATMOS_CODE_GEN_ARG_PASTE56, \
        GEOATMOS_CODE_GEN_ARG_PASTE55, \
        GEOATMOS_CODE_GEN_ARG_PASTE54, \
        GEOATMOS_CODE_GEN_ARG_PASTE53, \
        GEOATMOS_CODE_GEN_ARG_PASTE52, \
        GEOATMOS_CODE_GEN_ARG_PASTE51, \
        GEOATMOS_CODE_GEN_ARG_PASTE50, \
        GEOATMOS_CODE_GEN_ARG_PASTE49, \
        GEOATMOS_CODE_GEN_ARG_PASTE48, \
        GEOATMOS_CODE_GEN_ARG_PASTE47, \
        GEOATMOS_CODE_GEN_ARG_PASTE46, \
        GEOATMOS_CODE_GEN_ARG_PASTE45, \
        GEOATMOS_CODE_GEN_ARG_PASTE44, \
        GEOATMOS_CODE_GEN_ARG_PASTE43, \
        GEOATMOS_CODE_GEN_ARG_PASTE42, \
        GEOATMOS_CODE_GEN_ARG_PASTE41, \
        GEOATMOS_CODE_GEN_ARG_PASTE40, \
        GEOATMOS_CODE_GEN_ARG_PASTE39, \
        GEOATMOS_CODE_GEN_ARG_PASTE38, \
        GEOATMOS_CODE_GEN_ARG_PASTE37, \
        GEOATMOS_CODE_GEN_ARG_PASTE36, \
        GEOATMOS_CODE_GEN_ARG_PASTE35, \
        GEOATMOS_CODE_GEN_ARG_PASTE34, \
        GEOATMOS_CODE_GEN_ARG_PASTE33, \
        GEOATMOS_CODE_GEN_ARG_PASTE32, \
        GEOATMOS_CODE_GEN_ARG_PASTE31, \
        GEOATMOS_CODE_GEN_ARG_PASTE30, \
        GEOATMOS_CODE_GEN_ARG_PASTE29, \
        GEOATMOS_CODE_GEN_ARG_PASTE28, \
        GEOATMOS_CODE_GEN_ARG_PASTE27, \
        GEOATMOS_CODE_GEN_ARG_PASTE26, \
        GEOATMOS_CODE_GEN_ARG_PASTE25, \
        GEOATMOS_CODE_GEN_ARG_PASTE24, \
        GEOATMOS_CODE_GEN_ARG_PASTE23, \
        GEOATMOS_CODE_GEN_ARG_PASTE22, \
        GEOATMOS_CODE_GEN_ARG_PASTE21, \
        GEOATMOS_CODE_GEN_ARG_PASTE20, \
        GEOATMOS_CODE_GEN_ARG_PASTE19, \
        GEOATMOS_CODE_GEN_ARG_PASTE18, \
        GEOATMOS_CODE_GEN_ARG_PASTE17, \
        GEOATMOS_CODE_GEN_ARG_PASTE16, \
        GEOATMOS_CODE_GEN_ARG_PASTE15, \
        GEOATMOS_CODE_GEN_ARG_PASTE14, \
        GEOATMOS_CODE_GEN_ARG_PASTE13, \
        GEOATMOS_CODE_GEN_ARG_PASTE12, \
        GEOATMOS_CODE_GEN_ARG_PASTE11, \
        GEOATMOS_CODE_GEN_ARG_PASTE10, \
        GEOATMOS_CODE_GEN_ARG_PASTE9, \
        GEOATMOS_CODE_GEN_ARG_PASTE8, \
        GEOATMOS_CODE_GEN_ARG_PASTE7, \
        GEOATMOS_CODE_GEN_ARG_PASTE6, \
        GEOATMOS_CODE_GEN_ARG_PASTE5, \
        GEOATMOS_CODE_GEN_ARG_PASTE4, \
        GEOATMOS_CODE_GEN_ARG_PASTE3, \
        GEOATMOS_CODE_GEN_ARG_PASTE2, \
        GEOATMOS_CODE_GEN_ARG_PASTE1)(__VA_ARGS__))
#define GEOATMOS_CODE_GEN_ARG_PASTE1
#define GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) operator_function(v1)
#define GEOATMOS_CODE_GEN_ARG_PASTE3(operator_function, v1, v2) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v2)
#define GEOATMOS_CODE_GEN_ARG_PASTE4(operator_function, v1, v2, v3) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE3(operator_function, v2, v3)
#define GEOATMOS_CODE_GEN_ARG_PASTE5(operator_function, v1, v2, v3, v4) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE4(operator_function, v2, v3, v4)
#define GEOATMOS_CODE_GEN_ARG_PASTE6(operator_function, v1, v2, v3, v4, v5) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE5(operator_function, v2, v3, v4, v5)
#define GEOATMOS_CODE_GEN_ARG_PASTE7(operator_function, v1, v2, v3, v4, v5, v6) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE6(operator_function, v2, v3, v4, v5, v6)
#define GEOATMOS_CODE_GEN_ARG_PASTE8(operator_function, v1, v2, v3, v4, v5, v6, v7) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE7(operator_function, v2, v3, v4, v5, v6, v7)
#define GEOATMOS_CODE_GEN_ARG_PASTE9(operator_function, v1, v2, v3, v4, v5, v6, v7, v8) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE8(operator_function, v2, v3, v4, v5, v6, v7, v8)
#define GEOATMOS_CODE_GEN_ARG_PASTE10(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE9(operator_function, v2, v3, v4, v5, v6, v7, v8, v9)
#define GEOATMOS_CODE_GEN_ARG_PASTE11(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE10(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10)
#define GEOATMOS_CODE_GEN_ARG_PASTE12(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE11(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)
#define GEOATMOS_CODE_GEN_ARG_PASTE13(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE12(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12)
#define GEOATMOS_CODE_GEN_ARG_PASTE14(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE13(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13)
#define GEOATMOS_CODE_GEN_ARG_PASTE15(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE14(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14)
#define GEOATMOS_CODE_GEN_ARG_PASTE16(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE15(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15)
#define GEOATMOS_CODE_GEN_ARG_PASTE17(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE16(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16)
#define GEOATMOS_CODE_GEN_ARG_PASTE18(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE17(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17)
#define GEOATMOS_CODE_GEN_ARG_PASTE19(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE18(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18)
#define GEOATMOS_CODE_GEN_ARG_PASTE20(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE19(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19)
#define GEOATMOS_CODE_GEN_ARG_PASTE21(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE20(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20)
#define GEOATMOS_CODE_GEN_ARG_PASTE22(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE21(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21)
#define GEOATMOS_CODE_GEN_ARG_PASTE23(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE22(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22)
#define GEOATMOS_CODE_GEN_ARG_PASTE24(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE23(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23)
#define GEOATMOS_CODE_GEN_ARG_PASTE25(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE24(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24)
#define GEOATMOS_CODE_GEN_ARG_PASTE26(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE25(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25)
#define GEOATMOS_CODE_GEN_ARG_PASTE27(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE26(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26)
#define GEOATMOS_CODE_GEN_ARG_PASTE28(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE27(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27)
#define GEOATMOS_CODE_GEN_ARG_PASTE29(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE28(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28)
#define GEOATMOS_CODE_GEN_ARG_PASTE30(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE29(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29)
#define GEOATMOS_CODE_GEN_ARG_PASTE31(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE30(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30)
#define GEOATMOS_CODE_GEN_ARG_PASTE32(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE31(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31)
#define GEOATMOS_CODE_GEN_ARG_PASTE33(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE32(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32)
#define GEOATMOS_CODE_GEN_ARG_PASTE34(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE33(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33)
#define GEOATMOS_CODE_GEN_ARG_PASTE35(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE34(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34)
#define GEOATMOS_CODE_GEN_ARG_PASTE36(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE35(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35)
#define GEOATMOS_CODE_GEN_ARG_PASTE37(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE36(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36)
#define GEOATMOS_CODE_GEN_ARG_PASTE38(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE37(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37)
#define GEOATMOS_CODE_GEN_ARG_PASTE39(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE38(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38)
#define GEOATMOS_CODE_GEN_ARG_PASTE40(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE39(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39)
#define GEOATMOS_CODE_GEN_ARG_PASTE41(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE40(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40)
#define GEOATMOS_CODE_GEN_ARG_PASTE42(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE41(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41)
#define GEOATMOS_CODE_GEN_ARG_PASTE43(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE42(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42)
#define GEOATMOS_CODE_GEN_ARG_PASTE44(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE43(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43)
#define GEOATMOS_CODE_GEN_ARG_PASTE45(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE44(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44)
#define GEOATMOS_CODE_GEN_ARG_PASTE46(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE45(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45)
#define GEOATMOS_CODE_GEN_ARG_PASTE47(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE46(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46)
#define GEOATMOS_CODE_GEN_ARG_PASTE48(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE47(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47)
#define GEOATMOS_CODE_GEN_ARG_PASTE49(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE48(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48)
#define GEOATMOS_CODE_GEN_ARG_PASTE50(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE49(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49)
#define GEOATMOS_CODE_GEN_ARG_PASTE51(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE50(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50)
#define GEOATMOS_CODE_GEN_ARG_PASTE52(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE51(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51)
#define GEOATMOS_CODE_GEN_ARG_PASTE53(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE52(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52)
#define GEOATMOS_CODE_GEN_ARG_PASTE54(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE53(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53)
#define GEOATMOS_CODE_GEN_ARG_PASTE55(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE54(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54)
#define GEOATMOS_CODE_GEN_ARG_PASTE56(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE55(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55)
#define GEOATMOS_CODE_GEN_ARG_PASTE57(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE56(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56)
#define GEOATMOS_CODE_GEN_ARG_PASTE58(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE57(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57)
#define GEOATMOS_CODE_GEN_ARG_PASTE59(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE58(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58)
#define GEOATMOS_CODE_GEN_ARG_PASTE60(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE59(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59)
#define GEOATMOS_CODE_GEN_ARG_PASTE61(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE60(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60)
#define GEOATMOS_CODE_GEN_ARG_PASTE62(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE61(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61)
#define GEOATMOS_CODE_GEN_ARG_PASTE63(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61, v62) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE62(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61, v62)
#define GEOATMOS_CODE_GEN_ARG_PASTE64(operator_function, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61, v62, v63) GEOATMOS_CODE_GEN_ARG_PASTE2(operator_function, v1) GEOATMOS_CODE_GEN_ARG_PASTE63(operator_function, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61, v62, v63)

// clang-format on