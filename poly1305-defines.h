#if defined(_MSC_VER)
	#define COMPILER_MSVC
#endif
#if defined(__ICC)
	#define COMPILER_INTEL
#endif
#if defined(__GNUC__)
	#define COMPILER_GCC
#endif

#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__ ) || defined(_M_X64)
	#define CPU_X86_64
#elif defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__) || defined(_M_IX86) || defined(__X86__) || defined(_X86_) || defined(__I86__)
	#define CPU_X86
#endif


#if defined(CPU_X86_64) || defined(__ia64__) || defined(_IA64) || defined(__IA64__) || defined(_M_IA64) || \
	defined(__ia64) || defined(__sparcv9) || defined(__64BIT__) || defined(__LP64__) || defined(_LP64) || (_MIPS_SZLONG==64)

	#define CPU_64BITS
#endif

#define mul32x32_64(a,b) (((uint64_t)(a))*(b))

#if defined(COMPILER_MSVC)
	#include <intrin.h>
	#if !defined(_DEBUG)
		#undef mul32x32_64		
		#define mul32x32_64(a,b) __emulu(a,b)
	#endif
	#undef inline
	#define inline __forceinline
	typedef unsigned int uint32_t;
	typedef unsigned __int64 uint64_t;
	typedef signed __int64 int64_t;
#else
	#include <stdint.h>
	#include <sys/param.h>
	#undef inline
	#define inline __attribute__((always_inline))
#endif

// uint128_t
#if defined(CPU_64BITS)
	#if defined(COMPILER_MSVC)
		struct uint128 {
			uint64_t lo, hi;
		};
		typedef struct uint128 uint128_t;
		#define mul64x64_128(out,a,b) out.lo = _umul128(a,b,&out.hi);
		#define shr128_pair(hi,lo,shift) __shiftright128(lo, hi, shift)
		#define shr128(in,shift) shr128_pair(in.hi, in.lo, shift)
		#define add128(a,b) { uint64_t p = a.lo; a.lo += b.lo; a.hi += b.hi + (a.lo < p); }
		#define add128_64(a,b) { uint64_t p = a.lo; a.lo += b; a.hi += (a.lo < p); }
		#define lo128(a) (a.lo)
		

	#elif defined(COMPILER_GCC)
		typedef unsigned uint128_t __attribute__((mode(TI)));

		#define mul64x64_128(out,a,b) out = (uint128_t)a * b;
		#define shr128_pair(hi,lo,shift) (uint64_t)((((uint128_t)hi << 64) | lo) >> shift)
		#define shr128(in,shift) (uint64_t)(in >> shift)
		#define add128(a,b) a += b;
		#define add128_64(a,b) a += b;
		#define lo128(a) ((uint64_t)a)
	#else
		need 128bit define for this compiler
	#endif
#endif

// endian
#if ((defined(__BYTE_ORDER) && defined(__LITTLE_ENDIAN) && __BYTE_ORDER == __LITTLE_ENDIAN) || (defined(CPU_X86) || defined(CPU_X86_64)) || (defined(vax) || defined(MIPSEL)))
	static inline uint32_t U8TO32_LE(const unsigned char *p) { return *(const uint32_t *)p; }
	static inline void U32TO8_LE(unsigned char *p, const uint32_t v) { *(uint32_t *)p = v; }
	static inline uint64_t U8TO64_LE(const unsigned char *p) { return *(const uint64_t *)p; }
#else
	static inline uint32_t U8TO32_LE(const unsigned char *p) {
		return
		(((uint32_t)(p[0])      ) | 
		 ((uint32_t)(p[1]) <<  8) |
		 ((uint32_t)(p[2]) << 16) |
		 ((uint32_t)(p[3]) << 24));
	}

	static inline void U32TO8_LE(unsigned char *p, const uint32_t v) {
		p[0] = (unsigned char)(v      );
		p[1] = (unsigned char)(v >>  8);
		p[2] = (unsigned char)(v >> 16);
		p[3] = (unsigned char)(v >> 24);
	}

	static inline uint64_t U8TO64_LE(const unsigned char *p) {
		return
		(((uint64_t)((p)[0])      ) |
		 ((uint64_t)((p)[1]) <<  8) |
		 ((uint64_t)((p)[2]) << 16) |
		 ((uint64_t)((p)[3]) << 24) |
		 ((uint64_t)((p)[4]) << 32) |
		 ((uint64_t)((p)[5]) << 40) |
		 ((uint64_t)((p)[6]) << 48) |
		 ((uint64_t)((p)[7]) << 56));
	}
#endif

#define DONNA_INLINE inline
