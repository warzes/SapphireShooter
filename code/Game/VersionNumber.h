#pragma once

struct SVersionNumber
{
	SVersionNumber(uint32_t Maj = 1, uint32_t Min = 0, uint32_t Rev = 0, uint32_t Bld = 0) 
		: Major(Maj)
		, Minor(Min)
		, Revision(Rev)
		, Build(Bld)
	{
	}
	SVersionNumber(const SVersionNumber& Other) :
		Major(Other.Major),
		Minor(Other.Minor),
		Revision(Other.Revision),
		Build(Other.Build)
	{
	}

	bool operator==(const SVersionNumber& Other) const
	{
		return
			Major == Other.Major &&
			Minor == Other.Minor &&
			Revision == Other.Revision &&
			Build == Other.Build;
	}

	bool operator>(const SVersionNumber& Other) const
	{
		for (uint32_t i = 0; i < 4; ++i)
		{
			if ((*this)[i] > Other[i])
				return true;
			if ((*this)[i] < Other[i])
				return false;
		}
		return false;
	}

	bool operator>=(const SVersionNumber& Other) const
	{
		return *this > Other || *this == Other;
	}

	bool operator<(const SVersionNumber& Other) const
	{
		for (uint32_t i = 0; i < 4; ++i)
		{
			if ((*this)[i] > Other[i])
				return false;
			if ((*this)[i] < Other[i])
				return true;
		}
		return false;
	}

	bool operator<=(const SVersionNumber& Other) const
	{
		return *this < Other || *this == Other;
	}

	std::string GetString() const
	{
		return std::to_string(Major) + "." + std::to_string(Minor);
	}

	std::string GetStringDetailed() const
	{
		return GetString() + ", Rev. " + std::to_string(Revision) + ", Build. " + std::to_string(Build);
	}

	bool Valid() const
	{
		return (Major | Minor | Revision | Build) != 0;
	}

	uint32_t Major; 
	uint32_t Minor;
	uint32_t Revision;
	uint32_t Build;
};