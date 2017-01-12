#include "../vendor.h"
#include "value.h"

#ifndef PHPEXT_TYPES_CALLABLE_H
#define PHPEXT_TYPES_CALLABLE_H

namespace phpext
{
namespace types
{
	class callable: public value {
	public:
		callable(const value& w)
		:value(w)
		{
			assert(w.is_callable());
			zend_fcall_info_cache fcc_ = empty_fcall_info_cache;
			zend_fcall_info_init(val_, 0, &fci_, &fcc_, nullptr, nullptr);
		}
		callable(value&& w)
		:value(std::move(w))
		{
			assert(w.is_callable());
			zend_fcall_info_cache fcc_ = empty_fcall_info_cache;
			zend_fcall_info_init(val_, 0, &fci_, &fcc_, nullptr, nullptr);
		}
		callable(const char* name)
		:value()
		{
			ZVAL_NEW_STR(val_, zend_string_init(name, std::strlen(name), false));
			zend_make_callable(val_, nullptr);
			zend_fcall_info_cache fcc_ = empty_fcall_info_cache;
			zend_fcall_info_init(val_, 0, &fci_, &fcc_, nullptr, nullptr);
		}
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
		value operator() ()
		{
			return _call(0, nullptr);
		}

		bool is_empty()
		{
			return !zend_is_callable(val_, IS_CALLABLE_STRICT, nullptr);
		}
	private:
		zend_fcall_info       fci_;
		zend_fcall_info_cache fcc_;
		value _call(int argc, zval* argv)
		{
			value rv(nullptr);
			zend_fcall_info_argp(&fci_, argc, argv);
			zend_fcall_info_call(&fci_, &fcc_, rv.data(), nullptr);
			zend_fcall_info_args_clear(&fci_, 1);
			return std::move(rv);
		}
	};
}}

#endif // PHPEXT_TYPES_CALLABLE_H
