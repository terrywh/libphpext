#include "vendor.h"
#include "extension_entry.h"

#include "class_entry.h"
#include "closure.h"

namespace php {
	extension_entry* extension_entry::self;
	extension_entry::extension_entry(const std::string& name, const std::string& version)
	: name_(name)
	, version_(version) {
		self = this;
		dependencies_[0] = {"standard", "ge", "7.0.0", MODULE_DEP_REQUIRED};
		dependencies_[1] = {"json", "ge", "7.0.0", MODULE_DEP_REQUIRED};
		dependencies_[2] = {"date", "ge", "7.0.0", MODULE_DEP_REQUIRED};
		dependencies_[3] = {nullptr, nullptr, nullptr, 0};
		entry_.size                  = sizeof(entry_);
		entry_.zend_api              = ZEND_MODULE_API_NO;
		entry_.zend_debug            = ZEND_DEBUG;
		entry_.zts                   = 0;
		entry_.ini_entry             = nullptr; // 此项数据由 zend engine 填充
		entry_.deps                  = dependencies_;
		entry_.name                  = name_.c_str();
		entry_.functions             = nullptr; // 这里暂时设置为 nullptr 后面在所有 extension 处理完成后，统一生成
		entry_.module_startup_func   = on_module_startup_handler;
		entry_.module_shutdown_func  = on_module_shutdown_handler;
		entry_.request_startup_func  = on_request_startup_handler;
		entry_.request_shutdown_func = on_request_shutdown_handler;
		entry_.info_func             = on_module_info_handler;
		entry_.version               = version_.c_str();
		entry_.globals_size          = 0;
		entry_.globals_ptr           = nullptr;
		entry_.globals_ctor          = nullptr;
		entry_.globals_dtor          = nullptr;
		entry_.post_deactivate_func  = nullptr;
		entry_.module_started        = 0;
		entry_.type                  = 0;
		entry_.handle                = nullptr;
		entry_.module_number         = 0;
		entry_.build_id              = ZEND_MODULE_BUILD_ID;
	}
	extension_entry& extension_entry::ini(const ini_entry& entry) {
		ini_entries_.emplace_back(new ini_entry(entry));
		return *this;
	}
	extension_entry& extension_entry::constant(const constant_entry& entry) {
		constant_entries_.emplace_back(new constant_entry(entry));
		return *this;
	}
	extension_entry& extension_entry::desc(std::pair<std::string, std::string> kv) {
		decriptions_.push_back(kv);
		return *this;
	}
	extension_entry::operator zend_module_entry*() {
		// 函数注册
		if(!function_entries_.empty()) {
			zend_function_entry& entry = function_entries_.back();
			if(entry.fname) {
				function_entries_.push_back(zend_function_entry{});
				entry_.functions = function_entries_.data();
			}
		}
		return &entry_;
	}
	void extension_entry::on_module_startup(std::function<bool (extension_entry&)> handler) {
		handler_mst_.push_back(handler);
	}
	void extension_entry::on_module_shutdown(std::function<bool (extension_entry&)> handler) {
		handler_msd_.push_back(handler);
	}
	void extension_entry::on_request_startup(std::function<bool (extension_entry&)> handler) {
		handler_rst_.push_back(handler);
	}
	void extension_entry::on_request_shutdown(std::function<bool (extension_entry&)> handler) {
		handler_rsd_.push_back(handler);
	}
	// 扩展回调函数
	int extension_entry::on_module_startup_handler  (int type, int module) {
		// ini 注册
		if(!self->ini_entries_.empty()) {
			zend_ini_entry_def entries[self->ini_entries_.size() + 1];
			int i = 0;
			for(;i<self->ini_entries_.size();++i) {
				self->ini_entries_[i]->fill(&entries[i]);
			}
			// ini 注册停止条件
			entries[i].name = nullptr; // zend_register_ini_entries() -> while(entry->name) { zend_string_copy }
			zend_register_ini_entries(entries, module);
			// ini_entry 中持有的 php::string 拥有的 zend_string* 会被 Zend 引擎回收内存
			// 故需要提前清理
			self->ini_entries_.clear();
		}
		// 常量注册
		if(!self->constant_entries_.empty()) {
			for(auto i=self->constant_entries_.begin(); i!= self->constant_entries_.end(); ++i) {
				(*i)->declare(module);
			}
			self->constant_entries_.clear();
		}
		// 完成 classes 注册
		for(auto i=self->class_entries_.begin();i!=self->class_entries_.end();++i) {
			(*i)->declare();
		}
		// 正向调用
		for(auto i=self->handler_mst_.begin(); i!= self->handler_mst_.end(); ++i) {
			if(! (*i)(*self) ) return FAILURE;
		}
		return ZEND_RESULT_CODE::SUCCESS;
	}
	int extension_entry::on_module_shutdown_handler (int type, int module) {
		if(!self->ini_entries_.empty()) {
			zend_unregister_ini_entries(module);
		}
		// 反向调用
		for(auto i=self->handler_msd_.rbegin(); i!= self->handler_msd_.rend(); ++i) {
			if(! (*i)(*self) ) return FAILURE;
		}
		return ZEND_RESULT_CODE::SUCCESS;
	}
	int extension_entry::on_request_startup_handler (int type, int module) {
		// 正向调用
		for(auto i=self->handler_rst_.begin(); i!= self->handler_rst_.end(); ++i) {
			if(! (*i)(*self) ) return FAILURE;
		}
		return ZEND_RESULT_CODE::SUCCESS;
	}
	int extension_entry::on_request_shutdown_handler(int type, int module) {
		// 反向调用
		for(auto i=self->handler_rsd_.rbegin(); i!= self->handler_rsd_.rend(); ++i) {
			if(! (*i)(*self) ) return FAILURE;
		}
		return ZEND_RESULT_CODE::SUCCESS;
	}
	void extension_entry::on_module_info_handler(zend_module_entry *zend_module) {
		php_info_print_table_start();
		php_info_print_table_header(2, self->name_.c_str(), self->version_.c_str());
		for(auto i=self->decriptions_.begin(); i!=self->decriptions_.end(); ++i) {
			php_info_print_table_row(2, i->first.c_str(), i->second.c_str());
		}
		php_info_print_table_end();
	}
}
