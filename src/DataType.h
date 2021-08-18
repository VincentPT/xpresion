#pragma once

namespace xpression {
    enum class DataType : unsigned int {
        Unknown,
        Void,
		Integer,
		Long,
		Float,
		Double,
		Boolean,
		Char,
		Wchar,
		String,
		Wstring,
		Null,
    };
}