#include <cassert>
#include <cstring>

#include "callable.h"

namespace core
{
namespace types
{

	callable::callable(const value& v)
	{
		assert(v.is_callable());
		fcl_ = (zval*)value::pool.malloc();
		ZVAL_COPY(fcl_, v.data());
		fcc_ = empty_fcall_info_cache;
		zend_fcall_info_init(fcl_, 0, &fci_, &fcc_, nullptr, nullptr);
	}

	callable::callable(value&& v)
	{
		assert(v.is_callable());
		fcl_ = v.data();
		v.data(nullptr);
		fcc_ = empty_fcall_info_cache;
		zend_fcall_info_init(fcl_, 0, &fci_, &fcc_, nullptr, nullptr);
	}

	callable::callable(const char* n)
	{
		fcl_ = (zval*)value::pool.malloc();
		ZVAL_NEW_STR(fcl_, zend_string_init(n, std::strlen(n), false));
		zend_make_callable(fcl_, nullptr);

		fcc_ = empty_fcall_info_cache;
		zend_fcall_info_init(fcl_, 0, &fci_, &fcc_, nullptr, nullptr);
	}

	callable::callable(const callable& c)
	{
		fcl_ = (zval*)value::pool.malloc();
		ZVAL_COPY(fcl_, c.fcl_);
		fcc_ = empty_fcall_info_cache;
		zend_fcall_info_init(fcl_, 0, &fci_, &fcc_, nullptr, nullptr);
	}

	callable::callable(callable&& c)
	{
		fcl_ = c.fcl_;
		c.fcl_ = nullptr;

		fcc_ = c.fcc_;
		fci_ = c.fci_;
	}

	callable::~callable() {
		if(fcl_ != nullptr)
		{
			zval_dtor(fcl_);
			if(value::pool.is_from(fcl_))
			{
				value::pool.free(fcl_);
			}
		}
	}

	value callable::operator()()
	{
		return _call(0, nullptr);
	}

	value callable::_call(int argc, zval* argv)
	{
		value rval;
		zend_fcall_info_argp(&fci_, argc, argv);
		zend_fcall_info_call(&fci_, &fcc_, rval.data(), nullptr);
		zend_fcall_info_args_clear(&fci_, 1);
		return std::move(rval);
	}

	bool callable::operator ==(const callable& c)
	{
		return Z_PTR_P(fcl_) == Z_PTR_P(c.fcl_);
	}

}}
