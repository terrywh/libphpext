#include "extension_entry.h"

namespace php {
	extension_entry* extension_entry::self = nullptr;
	extension_entry::extension_entry(const char* name, const char* version)
	:module(0) {
		self = this; // 全局实例指针

		entry_ = (zend_module_entry*)pemalloc(sizeof(entry_), true);
		entry_->size                  = sizeof(entry_);
		entry_->zend_api              = ZEND_MODULE_API_NO;
		entry_->zend_debug            = ZEND_DEBUG;
		entry_->zts                   = USING_ZTS;
		entry_->ini_entry             = nullptr; // 此项数据由 zend engine 填充
		entry_->deps                  = nullptr;
		entry_->name                  = name;
		entry_->functions             = nullptr; // 这里暂时设置为 nullptr 后面在所有 extension 处理完成后，统一生成
		entry_->module_startup_func   = on_module_startup_handler;
		entry_->module_shutdown_func  = on_module_shutdown_handler;
		entry_->request_startup_func  = on_request_startup_handler;
		entry_->request_shutdown_func = on_request_shutdown_handler;
		entry_->info_func             = nullptr;
		entry_->version               = version;
		entry_->globals_size          = 0;
#ifdef ZTS
		entry_->globals_id_ptr        = nullptr;
#else
		entry_->globals_ptr           = nullptr;
#endif
		entry_->globals_ctor          = nullptr;
		entry_->globals_dtor          = nullptr;
		entry_->post_deactivate_func  = nullptr;
		entry_->module_started        = 0;
		entry_->type                  = 0;
		entry_->handle                = nullptr;
		entry_->module_number         = 0;
		entry_->build_id              = ZEND_MODULE_BUILD_ID;
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
		// ini 注册停止条件
		entries[i].name = nullptr; // zend_register_ini_entries() -> while(entry->name) { zend_string_copy }
		zend_register_ini_entries(entries.data(), module);
		// ini 可以清理了
		self->ini_entries_.clear();
		// 完成 classes 注册
		for(auto i=self->class_entries_.begin();i!=self->class_entries_.end();++i) {
			(*i)->declare();
		}
		// classes_entries_ 不能再完成之前清理（zend 引擎会应用其中的内存）
		// 常量注册
		zend_constant c;
		for(i=0;i<self->constant_entries_.size(); ++i) {
			self->constant_entries_[i].fill(&c, module);
			zend_register_constant(&c); // -> zend_hash_add_constant -> memcpy
		}
		// constants 可以清理了
		self->constant_entries_.clear();

		if(self->on_module_startup) {
			self->on_module_startup(self);
		}
		return ZEND_RESULT_CODE::SUCCESS;
	}
	int extension_entry::on_module_shutdown_handler (int type, int module) {
		zend_unregister_ini_entries(module);
		if(self->on_module_shutdown) {
			self->on_module_shutdown(self);
		}
		// self->class_entries_.clear();
		return ZEND_RESULT_CODE::SUCCESS;
	}
	int extension_entry::on_request_startup_handler (int type, int module) {
		if(self->on_request_startup) {
			self->on_request_shutdown(self);
		}
		return ZEND_RESULT_CODE::SUCCESS;
	}
	int extension_entry::on_request_shutdown_handler(int type, int module) {
		if(self->on_request_shutdown) {
			self->on_request_shutdown(self);
		}
		return ZEND_RESULT_CODE::SUCCESS;
	}

	extension_entry& extension_entry::add(const ini_entry& entry) {
		ini_entries_.push_back(entry);
		return *this;
	}
	extension_entry& extension_entry::add(const constant_entry& entry) {
		constant_entries_.push_back(entry);
		return *this;
	}

	extension_entry::operator zend_module_entry*() {
		// 函数注册
		function_entries_.push_back(zend_function_entry{});
		entry_->functions = function_entries_.data();
		return entry_;
	}

	extension_entry::~extension_entry() {

	}
}
