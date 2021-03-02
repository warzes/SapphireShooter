#pragma once

// подавление предупреждения warning C4191 (каст PROC)
// https://stackoverflow.com/questions/4192058/how-to-address-c4191-warning-around-calls-to-getprocaddress-with-farproc
template<typename Result, typename Original>
inline constexpr Result function_cast(Original fptr) noexcept
{
	return reinterpret_cast<Result>(reinterpret_cast<void*>(fptr));
}

template <typename T, size_t N>
constexpr size_t countof(T const (&)[N]) noexcept
{
	return N;
}