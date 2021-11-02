//#pragma once
//#include"polynomial.hpp"
//namespace expression {
//	template<typename coefficient_type, typename variable_type, unsigned int Variable_num>
//	class multi_polynomial
//	{
//	public:
//		multi_polynomial() = default;
//	protected:
//		vector<multi_polynomial<coefficient_type, variable_type, Variable_num - 1>> v;
//		vector<char> var_names;
//	};
//	template<typename coefficient_type, typename variable_type>
//	class multi_polynomial<coefficient_type, variable_type, 1>
//	{
//	public:
//		multi_polynomial() = default;
//	protected:
//		vector<default_type> v;
//	};
//	template<unsigned int Variable_num>
//	using multi_poly = multi_polynomial<default_type, default_type, Variable_num>;
//}