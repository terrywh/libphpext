#include "value.h"

#ifndef CORE_TYPES_CALLABLE_H
#define CORE_TYPES_CALLABLE_H

namespace core
{
namespace types
{
	class callable {
	public:
		callable(const value& v);
		callable(value&& v);
		callable(const char* n);
		callable(const callable& v);
		callable(callable&& v);

		virtual ~callable();

		template <typename ...Args>
		value operator() (const Args&... argv)
		{
			value argv1[] = { static_cast<value>(argv)... };
			std::vector<zval*> argv2;
			int argc = sizeof...(Args);
			for(int i=0;i<argc;++i)
			{
				argv2.push_back(argv1[i].data());
			}
			return _call(argc, (zval*)argv2.data());
		}
		value operator() ();

		bool operator==(const callable& c);
	private:
		zval*                 fcl_;
		zend_fcall_info       fci_;
		zend_fcall_info_cache fcc_;

		value _call(int argc, zval* argv);
	};
}}

#endif // CORE_TYPES_CALLABLE_H
