#include "../../core.h"

namespace phpext {
	namespace internal {
		ini_adder& ini_adder::add(const char* name, const char* dval) {
			_add(zend_string_init(name, std::strlen(name), true), zend_string_init(dval, std::strlen(dval), true));
			return *this;
		}
		ini_adder& ini_adder::add(const char* name, std::int64_t dval) {
			zend_string* d = zend_string_alloc(24, true);
			d->len = zend_sprintf(d->val, "%ld", dval);

			_add(zend_string_init(name, std::strlen(name), true), d);
			return *this;
		}
		ini_adder& ini_adder::add(const char* name, double dval) {
			zend_string* d = zend_string_alloc(32, true);
			d->len = zend_sprintf(d->val, "%f", dval);

			_add(zend_string_init(name, std::strlen(name), true), d);
			return *this;
		}
		ini_adder& ini_adder::add(const char* name, bool dval) {
			if(dval) {
				_add(zend_string_init(name, std::strlen(name), true), zend_string_init("true", 4, true));
			}else{
				_add(zend_string_init(name, std::strlen(name), true), zend_string_init("false", 5, true));
			}
			return *this;
		}
		void ini_adder::_add(zend_string* name, zend_string* value) {
			// @ 下述设置制过程，参考 Zend/zend_ini.c:233
			zend_ini_entry* entry  = (zend_ini_entry*)pemalloc(sizeof(zend_ini_entry), 1);
			entry->name            = name;
			entry->on_modify       = nullptr;
			entry->mh_arg1         = nullptr;
			entry->mh_arg2         = nullptr;
			entry->mh_arg3         = nullptr;
			if(value) {
				entry->value       = value;
			}else{
				entry->value       = nullptr;
			}
			entry->orig_value      = nullptr;
			entry->displayer       = nullptr;
			entry->modifiable      = ZEND_INI_ALL;
			entry->orig_modifiable = 0;
			entry->modified        = 0;
			// entry->module_number ---> 在 do_register_ini_entries() 中补全
			_entries.push_back(entry);
		}
		void ini_adder::do_register_ini_entries(int module) {
			// @ 下述设置制过程，参考 Zend/zend_ini.c:233
			HashTable *directives = EG(ini_directives);
			// 不确定这里是不是回因为使用 ZTS 及 dl 而发生内存竞争故障，
			// 上述参考文件，在这里的实现似是跟上述两个功能有关
			for(auto i=_entries.begin(); i!=_entries.end(); ++i) {
				(*i)->module_number = module;
				if(zend_hash_add_ptr(directives, (*i)->name, reinterpret_cast<void*>(*i)) == 0) {
					zend_error(E_ERROR, "failed to register ini entry '%.*s'", (*i)->name->len, (*i)->name->val);
					return;
				}
				zval* default_value = zend_get_configuration_directive((*i)->name);
				if(default_value != nullptr) {
					zend_string_release((*i)->value);
					(*i)->value = zend_string_copy(Z_STR_P(default_value));
				}
			}
		}
	}
}
