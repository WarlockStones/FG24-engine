#pragma once
#include <cstdio>

namespace FG24 {
class IWritable {
public:
	virtual bool WriteTo(FILE* file) const = 0;
	virtual bool ReadFrom(FILE* file) = 0;
};
}