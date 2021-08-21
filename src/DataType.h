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
		AsciiString,
		UnicodeString,
		String,
		Null,
    };
}