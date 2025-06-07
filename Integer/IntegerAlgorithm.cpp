#include <bits/stdc++.h>

struct IntegerAlgorithm
{
	static inline long long mod(long long a, long long m)
	{
		return (a % m + m) % m;
	}

	//O(sqrt(N)),not fast
	static std::vector<std::pair<long long, long long>> soinsu_bunkai(long long N)
	{
		std::vector<std::pair<long long, long long>> res;
		for (int i = 2; i <= sqrtl(N); i++)
		{
			if (N % i == 0)
			{
				long long p = 0;
				while (N % i == 0)
				{
					N /= i;
					p++;
				}
				res.emplace_back(i, p);
			}
		}
		if (N != 1)
			res.emplace_back(N, 1);
		return res;
	}
	
	

	static long long extGCD(long long a, long long b, long long &x, long long &y)
	{
		if (b == 0)
		{
			x = 1;
			y = 0;
			return a;
		}
		long long d = extGCD(b, a % b, y, x);
		y -= a / b * x;
		return d;
	}

	static std::pair<long long, long long> ChineseRem(long long b1, long long m1, long long b2, long long m2)
	{
		long long p, q;
		long long d = extGCD(m1, m2, p, q); // p is inv of m1/d (mod. m2/d)
		if ((b2 - b1) % d != 0)
			return std::make_pair(0, -1);
		long long m = m1 * (m2 / d); // lcm of (m1, m2)
		long long tmp = (b2 - b1) / d * p % (m2 / d);
		long long r = mod(b1 + m1 * tmp, m);
		return std::make_pair(r, m);
	}

	static std::pair<long long, long long> ChineseRem(const std::vector<long long> &b, const std::vector<long long> &m)
	{
		long long r = 0, M = 1;
		for (int i = 0; i < (int)b.size(); ++i)
		{
			long long p, q;
			long long d = extGCD(M, m[i], p, q); // p is inv of M/d (mod. m[i]/d)
			if ((b[i] - r) % d != 0)
				return std::make_pair(0, -1);
			long long tmp = (b[i] - r) / d * p % (m[i] / d);
			r += M * tmp;
			M *= m[i] / d;
		}
		return std::make_pair(mod(r, M), M);
	}

	static long long modinv(long long a, long long m)
	{
		long long b = m, u = 1, v = 0;
		while (b)
		{
			long long t = a / b;
			a -= t * b;
			std::swap(a, b);
			u -= t * v;
			std::swap(u, v);
		}
		u %= m;
		if (u < 0)
			u += m;
		return u;
	}

	static long long modpow(long long a, long long n, long long mod)
	{
		long long res = 1;
		while (n > 0)
		{
			if (n & 1)
				res = res * a % mod;
			a = a * a % mod;
			n >>= 1;
		}
		return res;
	}

	

	static long long mypow(long long a, long long p)
	{
		if (p == 0)
			return 1;
		long long b = mypow(a, p / 2);
		return b * b * (p % 2 + 1);
	}
};
