#include "MengerCubeFragmentsCache.h"

const std::list<std::array<unsigned, 3>>& MengerCubeFragmentsCache::getFragmentsCoords() const
{
	return fragmentsCoords;
}

unsigned int MengerCubeFragmentsCache::getN() const {
	return n;
}

void MengerCubeFragmentsCache::generatePatternArrays()
{
	patterns.clear();
	for (unsigned int i = 0; i < n; ++i)
	{
		patterns.emplace_back();
		const auto off = static_cast<unsigned int>(pow(3, i));
		const auto dblOff = off * 2;
		const unsigned int fragmentCoords[] = {0, off, dblOff};
		const unsigned int fragmentCoordsDim = sizeof(fragmentCoords) / sizeof(int);
		
		for (unsigned int j = 0; j < fragmentCoordsDim; ++j)
		{
			for (unsigned int k = 0; k < fragmentCoordsDim; ++k)
			{
				for (unsigned int l = 0; l < fragmentCoordsDim; ++l)
				{
					if (!(
						(j == 1 && l == 1) ||
						(j == 1 && k == 1) ||
						(k == 1 && l == 1) ||
						(j == 1 && k == 1 && l == 1)
						))
					{
						patterns.back().push_back({{fragmentCoords[j], fragmentCoords[k], fragmentCoords[l]}});
					}
				}
			}
		}
	}
}

void MengerCubeFragmentsCache::generateFragmentsCoords(const unsigned n, const unsigned x, const unsigned y, const unsigned z)
{
	auto patternIt = std::next(patterns.begin(), n);
	
	if (n == 0)
	{
		for (auto i = 0; i < patternIt->size(); ++i)
		{
			auto offset = *std::next(patternIt->begin(), i);
			fragmentsCoords.push_back({{x + offset[0], y + offset[1], z + offset[2]}});
		}

		return;
	}

	for (auto i = 0; i < patternIt->size(); ++i)
	{
		auto offset = *std::next(patternIt->begin(), i);
		generateFragmentsCoords(n - 1, x + offset[0], y + offset[1], z + offset[2]);
	}
}

MengerCubeFragmentsCache::MengerCubeFragmentsCache(const unsigned int n)
	: n(n)
{
	generatePatternArrays();
	generateFragmentsCoords(n - 1, 0, 0, 0);
}
