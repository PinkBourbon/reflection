﻿#pragma once

#include <cstdint>

//namespace detail
//{
//	// FNV-1a 32bit hashing algorithm.
//	constexpr uint32_t fnv1a_32(const char* s, size_t count)
//	{
//		return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
//	}
//
//	constexpr uint32_t fnv1a_32(const wchar_t* s, size_t count)
//	{
//		return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
//	}
//}    // namespace detail

//constexpr uint32_t operator"" _hash(char const* s, size_t count)
//{
//	return detail::fnv1a_32(s, count);
//}
//
//constexpr uint32_t check = "0123456789ABCDEF"_hash;

namespace xx
{
	struct xxh64 {
		static constexpr uint64_t hash(const char* p, uint64_t len, uint64_t seed) 
		{
			return finalize((len >= 32 ? h32bytes(p, len, seed) : seed + PRIME5) + len, p + (len & ~0x1F), len & 0x1F);
		}

	private:
		static constexpr uint64_t PRIME1 = 11400714785074694791ULL;
		static constexpr uint64_t PRIME2 = 14029467366897019727ULL;
		static constexpr uint64_t PRIME3 = 1609587929392839161ULL;
		static constexpr uint64_t PRIME4 = 9650029242287828579ULL;
		static constexpr uint64_t PRIME5 = 2870177450012600261ULL;

		static constexpr uint64_t rotl(uint64_t x, int r) 
		{
			return ((x << r) | (x >> (64 - r)));
		}
		static constexpr uint64_t mix1(const uint64_t h, const uint64_t prime, int rshift) 
		{
			return (h ^ (h >> rshift)) * prime;
		}
		static constexpr uint64_t mix2(const uint64_t p, const uint64_t v = 0) 
		{
			return rotl(v + p * PRIME2, 31) * PRIME1;
		}
		static constexpr uint64_t mix3(const uint64_t h, const uint64_t v) 
		{
			return (h ^ mix2(v)) * PRIME1 + PRIME4;
		}
#ifdef XXH64_BIG_ENDIAN
		static constexpr uint32_t endian32(const char* v) 
		{
			return uint32_t(uint8_t(v[3])) | (uint32_t(uint8_t(v[2])) << 8)
				| (uint32_t(uint8_t(v[1])) << 16) | (uint32_t(uint8_t(v[0])) << 24);
		}
		static constexpr uint64_t endian64(const char* v)
		{
			return uint64_t(uint8_t(v[7])) | (uint64_t(uint8_t(v[6])) << 8)
				| (uint64_t(uint8_t(v[5])) << 16) | (uint64_t(uint8_t(v[4])) << 24)
				| (uint64_t(uint8_t(v[3])) << 32) | (uint64_t(uint8_t(v[2])) << 40)
				| (uint64_t(uint8_t(v[1])) << 48) | (uint64_t(uint8_t(v[0])) << 56);
		}
#else
		static constexpr uint32_t endian32(const char* v) 
		{
			return uint32_t(uint8_t(v[0])) | (uint32_t(uint8_t(v[1])) << 8)
				| (uint32_t(uint8_t(v[2])) << 16) | (uint32_t(uint8_t(v[3])) << 24);
		}
		static constexpr uint64_t endian64(const char* v) 
		{
			return uint64_t(uint8_t(v[0])) | (uint64_t(uint8_t(v[1])) << 8)
				| (uint64_t(uint8_t(v[2])) << 16) | (uint64_t(uint8_t(v[3])) << 24)
				| (uint64_t(uint8_t(v[4])) << 32) | (uint64_t(uint8_t(v[5])) << 40)
				| (uint64_t(uint8_t(v[6])) << 48) | (uint64_t(uint8_t(v[7])) << 56);
		}
#endif
		static constexpr uint64_t fetch64(const char* p, const uint64_t v = 0) 
		{
			return mix2(endian64(p), v);
		}
		static constexpr uint64_t fetch32(const char* p) 
		{
			return uint64_t(endian32(p)) * PRIME1;
		}
		static constexpr uint64_t fetch8(const char* p) 
		{
			return uint8_t(*p) * PRIME5;
		}
		static constexpr uint64_t finalize(const uint64_t h, const char* p, uint64_t len) 
		{
			return (len >= 8) ? (finalize(rotl(h ^ fetch64(p), 27) * PRIME1 + PRIME4, p + 8, len - 8)) :
				((len >= 4) ? (finalize(rotl(h ^ fetch32(p), 23) * PRIME2 + PRIME3, p + 4, len - 4)) :
					((len > 0) ? (finalize(rotl(h ^ fetch8(p), 11) * PRIME1, p + 1, len - 1)) :
						(mix1(mix1(mix1(h, PRIME2, 33), PRIME3, 29), 1, 32))));
		}
		static constexpr uint64_t h32bytes(const char* p, uint64_t len, const uint64_t v1, const uint64_t v2, const uint64_t v3, const uint64_t v4) 
		{
			return (len >= 32) ? h32bytes(p + 32, len - 32, fetch64(p, v1), fetch64(p + 8, v2), fetch64(p + 16, v3), fetch64(p + 24, v4)) :
				mix3(mix3(mix3(mix3(rotl(v1, 1) + rotl(v2, 7) + rotl(v3, 12) + rotl(v4, 18), v1), v2), v3), v4);
		}
		static constexpr uint64_t h32bytes(const char* p, uint64_t len, const uint64_t seed) 
		{
			return h32bytes(p, len, seed + PRIME1 + PRIME2, seed + PRIME2, seed, seed - PRIME1);
		}
	};


	struct xxh32 
	{
		static constexpr uint32_t hash(const char* input, uint32_t len, uint32_t seed) 
		{
			return finalize((len >= 16 ? h16bytes(input, len, seed) : seed + PRIME5) + len, (input)+(len & ~0xF), len & 0xF);
		}

	private:
		static constexpr uint32_t PRIME1 = 0x9E3779B1U;
		static constexpr uint32_t PRIME2 = 0x85EBCA77U;
		static constexpr uint32_t PRIME3 = 0xC2B2AE3DU;
		static constexpr uint32_t PRIME4 = 0x27D4EB2FU;
		static constexpr uint32_t PRIME5 = 0x165667B1U;

		// 32-bit rotate left.
		static constexpr uint32_t rotl(uint32_t x, int r) 
		{
			return ((x << r) | (x >> (32 - r)));
		}
		// Normal stripe processing routine.
		static constexpr uint32_t round(uint32_t acc, const uint32_t input) 
		{
			return rotl(acc + (input * PRIME2), 13) * PRIME1;
		}

		static constexpr uint32_t avalanche_step(const uint32_t h, const int rshift, const uint32_t prime) 
		{
			return (h ^ (h >> rshift)) * prime;
		}
		// Mixes all bits to finalize the hash.
		static constexpr uint32_t avalanche(const uint32_t h) 
		{
			return avalanche_step(avalanche_step(avalanche_step(h, 15, PRIME2), 13, PRIME3), 16, 1);
		}

#ifdef XXH32_BIG_ENDIAN
		static constexpr uint32_t endian32(const char* v) 
		{
			return uint32_t(uint8_t(v[3])) | (uint32_t(uint8_t(v[2])) << 8)
				| (uint32_t(uint8_t(v[1])) << 16) | (uint32_t(uint8_t(v[0])) << 24);
		}
#else
		static constexpr uint32_t endian32(const char* v) 
		{
			return uint32_t(uint8_t(v[0])) | (uint32_t(uint8_t(v[1])) << 8)
				| (uint32_t(uint8_t(v[2])) << 16) | (uint32_t(uint8_t(v[3])) << 24);
		}
#endif // XXH32_BIG_ENDIAN

		static constexpr uint32_t fetch32(const char* p, const uint32_t v) 
		{
			return round(v, endian32(p));
		}

		// Processes the last 0-15 bytes of p.
		static constexpr uint32_t finalize(const uint32_t h, const char* p, uint32_t len) 
		{
			return
				(len >= 4) ? finalize(rotl(h + (endian32(p) * PRIME3), 17) * PRIME4, p + 4, len - 4) :
				(len > 0) ? finalize(rotl(h + (uint8_t(*p) * PRIME5), 11) * PRIME1, p + 1, len - 1) :
				avalanche(h);
		}

		static constexpr uint32_t h16bytes(const char* p, uint32_t len, const uint32_t v1, const uint32_t v2, const uint32_t v3, const uint32_t v4) 
		{
			return
				(len >= 16) ? h16bytes(p + 16, len - 16, fetch32(p, v1), fetch32(p + 4, v2), fetch32(p + 8, v3), fetch32(p + 12, v4)) :
				rotl(v1, 1) + rotl(v2, 7) + rotl(v3, 12) + rotl(v4, 18);
		}
		static constexpr uint32_t h16bytes(const char* p, uint32_t len, const uint32_t seed) 
		{
			return h16bytes(p, len, seed + PRIME1 + PRIME2, seed + PRIME2, seed, seed - PRIME1);
		}
	};
}


//int main()
//{
//	// Compare against precomputed value.
//	static_assert(check == 141695047u, "bad hash value");
//	static_assert(check == 141695046u, "bad hash value");
//	return 0;
//}
