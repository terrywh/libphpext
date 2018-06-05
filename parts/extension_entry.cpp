#include "../phpext.h"

namespace php {
	extension_entry* extension_entry::self = nullptr;
	extension_entry::extension_entry()
	:module(0) {
		deps_.push_back(zend_module_dep {
			"standard", "ge", "7.0.0", MODULE_DEP_REQUIRED
		});
		deps_.push_back(zend_module_dep {
			"json", "ge", "7.0.0", MODULE_DEP_REQUIRED
		});
		deps_.push_back(zend_module_dep {
			"mysqlnd", nullptr, nullptr, MODULE_DEP_REQUIRED
		});
		deps_.push_back(zend_module_dep {
			nullptr, nullptr, nullptr, 0
		});

		self = this; // 全局实例指针
		entry_.size                  = sizeof(entry_);
		entry_.zend_api              = ZEND_MODULE_API_NO;
		entry_.zend_debug            = ZEND_DEBUG;
		entry_.zts                   = USING_ZTS;
		entry_.ini_entry             = nullptr; // 此项数据由 zend engine 填充
		entry_.deps                  = deps_.data();
		entry_.name                  = nullptr;
		entry_.functions             = nullptr; // 这里暂时设置为 nullptr 后面在所有 extension 处理完成后，统一生成
		entry_.module_startup_func   = on_module_startup_handler;
		entry_.module_shutdown_func  = on_module_shutdown_handler;
		entry_.request_startup_func  = on_request_startup_handler;
		entry_.request_shutdown_func = on_request_shutdown_handler;
		entry_.info_func             = nullptr;
		entry_.version               = nullptr;
		entry_.globals_size          = 0;
#ifdef ZTS
		entry_.globals_id_ptr        = nullptr;
#else
		entry_.globals_ptr           = nullptr;
#endif
		entry_.globals_ctor          = nullptr;
		entry_.globals_dtor          = nullptr;
		entry_.post_deactivate_func  = nullptr;
		entry_.module_started        = 0;
		entry_.type                  = 0;
		entry_.handle                = nullptr;
		entry_.module_number         = 0;
		entry_.build_id              = ZEND_MODULE_BUILD_ID;
		// 内部 class
		_register_builtin_class(*this);
	}

	void extension_entry::init(const char* name, const char* version) {
		entry_.name = name;
		entry_.version = version;
	}

	void extension_entry::done() {
		assert(entry_.name != nullptr);
		assert(entry_.version != nullptr);
		// 函数注册
		if(!function_entries_.empty()) {
			function_entries_.push_back(zend_function_entry{});
			entry_.functions = function_entries_.data();
		}
	}

	int extension_entry::on_module_startup_handler(int type, int module) {
		self->module = module;
		// ini 注册
		std::vector<zend_ini_entry_def> entries(self->ini_entries_.size() + 1);
		int i = 0;
		for(;i<self->ini_entries_.size();++i) {
			self->ini_entries_[i].fill(&entries[i]);
			zend_string* name = zend_string_init(entries[i].name, entries[i].name_length, 0);
		}
		if(i > 0) {
			// ini 注册停止条件
			entries[i].name = nullptr; // zend_register_ini_entries() -> while(entry->name) { zend_string_copy }
			zend_register_ini_entries(entries.data(), module);
		}
		entries.clear();
		// classes_entries_ 不能再完成之前清理（zend 引擎会应用其中的内存）
		// 常量注册
		for(i=0;i<self->constant_entries_.size(); ++i) {
			self->constant_entries_[i].declare(module);
		}
		// constants 可以清理了
		self->constant_entries_.clear();
		// 完成 classes 注册
		for(auto i=self->class_entries_.begin();i!=self->class_entries_.end();++i) {
			(*i)->declare();
		}
		// 正向调用
		while(!self->handler_mst_.empty()) {
			if(! self->handler_mst_.front()(*self) ) return FAILURE;
			self->handler_mst_.pop_front();
		}
		return ZEND_RESULT_CODE::SUCCESS;
	}
	int extension_entry::on_module_shutdown_handler (int type, int module) {
		if(!self->ini_entries_.empty()) {
			zend_unregister_ini_entries(module);
		}
		// 反向调用
		while(!self->handler_msd_.empty()) {
			if(! self->handler_msd_.back()(*self) ) return FAILURE;
			self->handler_msd_.pop_back();
		}
		return ZEND_RESULT_CODE::SUCCESS;
	}
	int extension_entry::on_request_startup_handler (int type, int module) {
		// 正向调用
		while(!self->handler_rst_.empty()) {
			if(! self->handler_rst_.front()(*self) ) return FAILURE;
			self->handler_rst_.pop_front();
		}
		return ZEND_RESULT_CODE::SUCCESS;
	}
	int extension_entry::on_request_shutdown_handler(int type, int module) {
		// 反向调用
		while(!self->handler_rsd_.empty()) {
			if(! self->handler_rsd_.back()(*self) ) return FAILURE;
			self->handler_rsd_.pop_back();
		}
		return ZEND_RESULT_CODE::SUCCESS;
	}

	extension_entry& extension_entry::config(const ini_entry& entry) {
		ini_entries_.push_back(entry);
		return *this;
	}
	extension_entry& extension_entry::define(const constant_entry& entry) {
		constant_entries_.push_back(entry);
		return *this;
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

	zend_module_entry* extension_entry::entry() {
		return &entry_;
	}

	extension_entry::~extension_entry() {

	}
}
