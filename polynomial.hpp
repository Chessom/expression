#pragma once
#include<vector>
#include<utility>
#include<iostream>
#include<type_traits>
#include<initializer_list>
#include"expr_error.hpp"
#if __has_include(<boost/multiprecision/cpp_int.hpp>) || __has_include("boost/multiprecision/cpp_int.hpp>")
#define BOOST_MULTIPRECISION 1
#include<boost/multiprecision/cpp_int.hpp>
#else
#define BOOST_MULTIPRECISION 0
#endif
namespace expression {
	using std::istream;
	using std::ostream;
	using std::cin;
	using std::cout;
	using std::vector;
	using std::initializer_list;
#if BOOST_MULTIPRECISION
	using namespace boost::multiprecision;
	using default_type = cpp_rational;
#else
	using default_type = double;
#endif
	template<typename coefficient_type = default_type, typename variable_type = default_type>
	class polynomial {
		template<typename coefficient_type, typename variable_type>
		friend istream& operator>>(istream&, polynomial<coefficient_type, variable_type>&);
		template<typename coefficient_type, typename variable_type>
		friend ostream& operator<<(ostream&, const polynomial<coefficient_type, variable_type>&);
		using c_t = coefficient_type;
		using v_t = variable_type;
	public:
		polynomial() :v({}), var_name('x') {}
		polynomial(const vector<coefficient_type>& coefficient, const char variable_name = 'x') :v(coefficient), var_name(variable_name) {}
		polynomial(const initializer_list<coefficient_type>& coefficient, const char variable_name = 'x') :v(coefficient), var_name(variable_name) {}
		polynomial(const initializer_list<coefficient_type>&& coefficient, const char variable_name = 'x') :v(coefficient), var_name(variable_name) {}
		polynomial(const coefficient_type val) { v.push_back(val); var_name = 'x'; }
		template<typename number_type>
		polynomial(number_type num) {
			var_name = 'x';
			static_assert(std::is_constructible<coefficient_type, number_type>::value || std::is_convertible<number_type, coefficient_type>::value, "no such constructor or conversion from this type to coefficient type");
			if (std::is_constructible<coefficient_type, number_type>::value) {
				v.push_back(coefficient_type(num));
				return;
			}
			if (std::is_convertible<number_type, coefficient_type>::value) {
				v.push_back(static_cast<coefficient_type>(num));
				return;
			}
		}
		bool equal_zero()const {
			for (int i = 0; i < v.size(); ++i)
				if (v[i] != 0)return false;
			return true;
		}
		bool operator==(const polynomial& p) const {
			if (var_name != p.var_name)return false;
			if (p.v.size() != v.size())return false;
			for (int i = 0; i < v.size(); ++i) {
				if (p.v[i] != v[i])return false;
			}
			return true;
		}
		bool operator==(const polynomial&& p) const {
			if (var_name != p.var_name)return false;
			if (p.v.size() != v.size())return false;
			for (unsigned int i = 0; i < v.size(); ++i) {
				if (p.v[i] != v[i])return false;
			}
			return true;
		}
		bool operator!=(const polynomial& p) const {
			if (var_name != p.var_name)return true;
			return !(p == *this);
		}
		bool operator!=(const polynomial&& p) const {
			if (var_name != p.var_name)return true;
			return !(p == *this);
		}
		const polynomial& operator+=(const polynomial& p) {
			if (var_name != p.var_name)
				throw(std::runtime_error("variables' names are different"));
			if (p.v.size() > v.size())
				v.resize(p.v.size());
			for (size_t i = 0; i < std::min(v.size(), p.v.size()); ++i) {
				v[i] += p.v[i];
			}
			return *this;
		}
		const polynomial& operator-=(const polynomial& p) {
			if (var_name != p.var_name)
				throw(std::runtime_error("variables' names are different"));
			if (p.v.size() > v.size())
				v.resize(p.v.size());
			for (size_t i = 0; i < std::min(v.size(), p.v.size()); ++i) {
				v[i] -= p.v[i];
			}
			return *this;
		}
		const polynomial& operator*=(const polynomial& p) {
			if (var_name != p.var_name)
				throw(std::runtime_error("variables' names are different"));
			if (p == polynomial{0, 1}) {
				v.resize(v.size() + 1);
				for (unsigned int i = 0; i < v.size() - 1; ++i)
					v[i + 1] = v[i];
				return *this;
			}
			polynomial ans;
			ans.max_exponent(p.v.size() + v.size() - 1);
			for (unsigned int i = 0; i < p.v.size(); ++i) {
				for (unsigned int j = 0; j < v.size(); ++j)
					ans.v[i + j] += p.v[i] * v[j];
			}
			return *this = ans;
		}
		const polynomial& operator/=(const polynomial& p) {
			if (var_name != p.var_name)
				throw(std::runtime_error("variables' names are different"));
			if (p.equal_zero())
				throw(divided_by_zero("divided by_zero"));
			int mexp = highest_exp(), pmexp = p.highest_exp();
			if (mexp < pmexp)
				return *this = 0;
			polynomial ans(mexp - pmexp + 1, var_name), tp = p;
			for (int i = mexp - pmexp; i >= 0; ++i) {

			}
			return *this;
		}
		const polynomial& operator%=(const polynomial& p) {
			if (var_name != p.var_name)
				throw(std::runtime_error("variables' names are different"));
			return *this;
		}
		const polynomial& operator+=(polynomial&& p) {
			if (var_name != p.var_name)
				throw(std::runtime_error("variables' names are different"));
			if (p.v.size() > v.size())
				v.resize(p.v.size());
			for (size_t i = 0; i < std::min(v.size(), p.v.size()); ++i) {
				v[i] += p.v[i];
			}
			return *this;
		}
		const polynomial& operator-=(polynomial&& p) {
			if (var_name != p.var_name)
				throw(std::runtime_error("variables' names are different"));
			if (p.v.size() > v.size())
				v.resize(p.v.size());
			for (size_t i = 0; i < std::min(v.size(), p.v.size()); ++i) {
				v[i] -= p.v[i];
			}
			return *this;
		}
		const polynomial& operator*=(polynomial&& p) {
			if (var_name != p.var_name)
				throw(std::runtime_error("variables' names are different"));
			polynomial ans;
			ans.max_exponent(p.v.size() + v.size() - 1);
			for (unsigned int i = 0; i < p.v.size(); ++i) {
				for (unsigned int j = 0; j < v.size(); ++j)
					ans.v[i + j] += p.v[i] * v[j];
			}
			*this = ans;
			return *this;
		}
		const polynomial& operator/=(polynomial&& p) {
			if (var_name != p.var_name)
				throw(std::runtime_error("variables' names are different"));
			return *this;
		}
		const polynomial& operator%=(polynomial&& p) {
			if (var_name != p.var_name)
				throw(std::runtime_error("variables' names are different"));
			return *this;
		}
		coefficient_type& operator[](const unsigned int exponent) const { return v[exponent]; }
		virtual polynomial operator^(const int exponent) const {
			polynomial ans = 1, a = *this;
			int ex = exponent;
			while (ex > 0) {
				if (ex & 1)
					ans *= a;
				a *= a;
				ex >>= 1;
			}
			return ans;
		}
		void max_exponent(const int exponent) { v.resize(exponent + 1); }
		void variable_name(const char variable_name) { var_name = variable_name; }
		const int max_exponent() const { return v.size() - 1; }
		const char variable_name() const { return var_name; }
		size_t highest_exp() {
			for (int i = v.size() - 1; i >= 0; --i) {
				if (v[i] != 0)return i;
			}
			return 0;
		}
		size_t lowest_exp() {
			for (int i = 0; i < v.size(); ++i) {
				if (v[i] != 0)return i;
			}
			return 0;
		}
		polynomial calculate(const polynomial& p) const {
			if (v.size() > 1) {
				polynomial ans(v.back());
				for (int i = v.size() - 1; i > 0; --i)
					ans = ans * p + v[i - 1];
				return ans;
			}
			else
				return polynomial(v.front());
		}
		coefficient_type calculate(const variable_type variable) const {
			if (v.size() > 1) {
				const v_t& var = variable;
				v_t ans = v.back();
				for (int i = v.size() - 1; i > 0; --i)
					ans = ans * var + v[i - 1];
				return ans;
			}
			else
				return v.front();
		}
		std::pair<polynomial, polynomial> dividemod(const polynomial& p) {
			if (var_name != p.var_name)
				throw(std::runtime_error("variables' names are different"));
			if (p.equal_zero())
				throw(divided_by_zero("divided by_zero"));

		}
		std::pair<polynomial, polynomial> dividemod(const polynomial&& p) {
			if (var_name != p.var_name)
				throw(std::runtime_error("variables' names are different"));
			if (p.equal_zero())
				throw(divided_by_zero("divided by_zero"));


		}
	protected:
		vector<c_t> v;
		char var_name = 'x';
		polynomial(const int exponent, const char variable_name) :v(exponent), var_name(variable_name) {}
	};
	using poly_int = polynomial<int, int>;
	using poly_double = polynomial<double, double>;
	using poly = polynomial<default_type, default_type>;
	template<typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator+(const polynomial<coefficient_type, variable_type>& p1, const polynomial<coefficient_type, variable_type>& p2) {
		polynomial<coefficient_type, variable_type> p = p1;
		p += p2;
		return p;
	}
	template<typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator-(const polynomial<coefficient_type, variable_type>& p1, const polynomial<coefficient_type, variable_type>& p2) {
		polynomial<coefficient_type, variable_type> p = p1;
		p -= p2;
		return p;
	}
	template<typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator*(const polynomial<coefficient_type, variable_type>& p1, const polynomial<coefficient_type, variable_type>& p2) {
		polynomial<coefficient_type, variable_type> p = p1;
		p *= p2;
		return p;
	}
	template<typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator/(const polynomial<coefficient_type, variable_type>& p1, const polynomial<coefficient_type, variable_type>& p2) {
		polynomial<coefficient_type, variable_type> p = p1;
		p /= p2;
		return p;
	}
	template<typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator%(const polynomial<coefficient_type, variable_type>& p1, const polynomial<coefficient_type, variable_type>& p2) {
		polynomial<coefficient_type, variable_type> p = p1;
		p %= p2;
		return p;
	}

	template<typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator+(const polynomial<coefficient_type, variable_type>& p1, const polynomial<coefficient_type, variable_type>&& p2) {
		polynomial<coefficient_type, variable_type> p = p1;
		p += p2;
		return p;
	}
	template<typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator-(const polynomial<coefficient_type, variable_type>& p1, const polynomial<coefficient_type, variable_type>&& p2) {
		polynomial<coefficient_type, variable_type> p = p1;
		p -= p2;
		return p;
	}
	template<typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator*(const polynomial<coefficient_type, variable_type>& p1, const polynomial<coefficient_type, variable_type>&& p2) {
		polynomial<coefficient_type, variable_type> p = p1;
		p *= p2;
		return p;
	}
	template<typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator/(const polynomial<coefficient_type, variable_type>& p1, const polynomial<coefficient_type, variable_type>&& p2) {
		polynomial<coefficient_type, variable_type> p = p1;
		p /= p2;
		return p;
	}
	template<typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator%(const polynomial<coefficient_type, variable_type>& p1, const polynomial<coefficient_type, variable_type>&& p2) {
		polynomial<coefficient_type, variable_type> p = p1;
		p %= p2;
		return p;
	}

	template<typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator+(const polynomial<coefficient_type, variable_type>&& p1, const polynomial<coefficient_type, variable_type>& p2) {
		polynomial<coefficient_type, variable_type> p = p1;
		p += p2;
		return p;
	}
	template<typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator-(const polynomial<coefficient_type, variable_type>&& p1, const polynomial<coefficient_type, variable_type>& p2) {
		polynomial<coefficient_type, variable_type> p = p1;
		p -= p2;
		return p;
	}
	template<typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator*(const polynomial<coefficient_type, variable_type>&& p1, const polynomial<coefficient_type, variable_type>& p2) {
		polynomial<coefficient_type, variable_type> p = p1;
		p *= p2;
		return p;
	}
	template<typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator/(const polynomial<coefficient_type, variable_type>&& p1, const polynomial<coefficient_type, variable_type>& p2) {
		polynomial<coefficient_type, variable_type> p = p1;
		p /= p2;
		return p;
	}
	template<typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator%(const polynomial<coefficient_type, variable_type>&& p1, const polynomial<coefficient_type, variable_type>& p2) {
		polynomial<coefficient_type, variable_type> p = p1;
		p %= p2;
		return p;
	}

	template<typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator+(const polynomial<coefficient_type, variable_type>&& p1, const polynomial<coefficient_type, variable_type>&& p2) {
		polynomial<coefficient_type, variable_type> p = p1;
		p += p2;
		return p;
	}
	template<typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator-(const polynomial<coefficient_type, variable_type>&& p1, const polynomial<coefficient_type, variable_type>&& p2) {
		polynomial<coefficient_type, variable_type> p = p1;
		p -= p2;
		return p;
	}
	template<typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator*(const polynomial<coefficient_type, variable_type>&& p1, const polynomial<coefficient_type, variable_type>&& p2) {
		polynomial<coefficient_type, variable_type> p = p1;
		p *= p2;
		return p;
	}
	template<typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator/(const polynomial<coefficient_type, variable_type>&& p1, const polynomial<coefficient_type, variable_type>&& p2) {
		polynomial<coefficient_type, variable_type> p = p1;
		p /= p2;
		return p;
	}
	template<typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator%(const polynomial<coefficient_type, variable_type>&& p1, const polynomial<coefficient_type, variable_type>&& p2) {
		polynomial<coefficient_type, variable_type> p = p1;
		p %= p2;
		return p;
	}

	template<typename number_type, typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator+(const polynomial<coefficient_type, variable_type>& p1, const number_type& v) {
		polynomial<coefficient_type, variable_type> p = p1;
		p += polynomial<coefficient_type, variable_type>(v);
		return p;
	}
	template<typename number_type, typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator-(const polynomial<coefficient_type, variable_type>& p1, const number_type& v) {
		polynomial<coefficient_type, variable_type> p = p1;
		p -= v;
		return p;
	}
	template<typename number_type, typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator*(const polynomial<coefficient_type, variable_type>& p1, const number_type& v) {
		polynomial<coefficient_type, variable_type> p = p1;
		p *= v;
		return p;
	}
	template<typename number_type, typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator/(const polynomial<coefficient_type, variable_type>& p1, const number_type& v) {
		polynomial<coefficient_type, variable_type> p = p1;
		p /= v;
		return p;
	}
	template<typename number_type, typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator%(const polynomial<coefficient_type, variable_type>& p1, const number_type& v) {
		polynomial<coefficient_type, variable_type> p = p1;
		p %= v;
		return p;
	}

	template<typename number_type, typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator+(const polynomial<coefficient_type, variable_type>& p1, const number_type&& v) {
		polynomial<coefficient_type, variable_type> p = p1;
		p += polynomial<coefficient_type, variable_type>(v);
		return p;
	}
	template<typename number_type, typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator-(const polynomial<coefficient_type, variable_type>& p1, const number_type&& v) {
		polynomial<coefficient_type, variable_type> p = p1;
		p -= v;
		return p;
	}
	template<typename number_type, typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator*(const polynomial<coefficient_type, variable_type>& p1, const number_type&& v) {
		polynomial<coefficient_type, variable_type> p = p1;
		p *= v;
		return p;
	}
	template<typename number_type, typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator/(const polynomial<coefficient_type, variable_type>& p1, const number_type&& v) {
		polynomial<coefficient_type, variable_type> p = p1;
		p /= v;
		return p;
	}
	template<typename number_type, typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator%(const polynomial<coefficient_type, variable_type>& p1, const number_type&& v) {
		polynomial<coefficient_type, variable_type> p = p1;
		p %= v;
		return p;
	}

	template<typename number_type, typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator+(const number_type& v, const polynomial<coefficient_type, variable_type>& p1) {
		polynomial<coefficient_type, variable_type> p = p1;
		p += polynomial<coefficient_type, variable_type>(v);
		return p;
	}
	template<typename number_type, typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator-(const number_type& v, const polynomial<coefficient_type, variable_type>& p1) {
		polynomial<coefficient_type, variable_type> p = p1;
		p -= v;
		return p;
	}
	template<typename number_type, typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator*(const number_type& v, const polynomial<coefficient_type, variable_type>& p1) {
		polynomial<coefficient_type, variable_type> p = p1;
		p *= v;
		return p;
	}
	template<typename number_type, typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator/(const number_type& v, const polynomial<coefficient_type, variable_type>& p1) {
		polynomial<coefficient_type, variable_type> p = p1;
		p /= v;
		return p;
	}
	template<typename number_type, typename coefficient_type = default_type, typename variable_type = default_type>
	inline polynomial<coefficient_type, variable_type> operator%(const number_type& v, const polynomial<coefficient_type, variable_type>& p1) {
		polynomial<coefficient_type, variable_type> p = p1;
		p %= v;
		return p;
	}
	template<typename coefficient_type, typename variable_type>
	std::istream& operator>>(std::istream& is, polynomial<coefficient_type, variable_type>& p) {
		int term = 0;
		is >> term;
		for (int i = 0; i < term; ++i)
			is >> p.v[i];
		return is;
	}
	template<typename coefficient_type, typename variable_type>
	std::ostream& operator<<(std::ostream& os, const polynomial<coefficient_type, variable_type>& p) {
		bool first = true, print = false;
		for (int i = p.v.size() - 1; i > 1; --i) {//n~2
			if (p.v[i] != 0) {
				if (first)
					first = false;
				else
					os << "+";
				if (p.v[i] != 1)
					os << p.v[i] << "*" << p.var_name << "^" << i, print = true;
				else
					os << p.var_name << "^" << i, print = true;
			}
		}
		if (p.v.size() > 1 && p.v[1] != 0) {
			if (p.v[1] != 1)
				os << (first ? first = false, "" : "+") << p.v[1] << "*" << p.var_name, print = true;
			else
				os << (first ? first = false, "" : "+") << p.var_name, print = true;
		}
		if (p.v.size() > 0 && p.v[0] != 0) {
			os << (first ? "" : "+") << p.v[0];
			print = true;
		}
		if (!print)os << "0";
		return os;
	}
}
