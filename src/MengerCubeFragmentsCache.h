#pragma once

#include <array>
#include <list>

class MengerCubeFragmentsCache
{
	std::list<std::array<unsigned int, 3>> fragmentsCoords;
	std::list<std::list<std::array<unsigned int, 3>>> patterns;
	unsigned int n;

	void generatePatternArrays();
	void generateFragmentsCoords(const unsigned n, const unsigned x, const unsigned y, const unsigned z);

public:
	MengerCubeFragmentsCache(const unsigned int n);

	const std::list<std::array<unsigned int, 3>>& getFragmentsCoords() const;
    unsigned int getN() const;
};

