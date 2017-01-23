#include "extension_entry.h"

namespace php {
	extension_entry::extension_entry(const char* name, const char* version)
	:module_number(0) {
		self_ = this; // 全局实例指针

		entry_ = (zend_module_entry*)pemalloc(sizeof(entry_), true);
		entry_->size                  = sizeof(entry_);
		entry_->zend_api              = ZEND_MODULE_API_NO;
		entry_->zend_debug            = ZEND_DEBUG;
		entry_->zts                   = USING_ZTS;
		entry_->ini_entry             = nullptr; // 此项数据由 zend engine 填充
		entry_->deps                  = nullptr;
		entry_->name                  = name;
		entry_->functions             = nullptr; // 这里暂时设置为 nullptr 后面在所有 extension 处理完成后，统一生成
		entry_->module_startup_func   = on_module_startup;
		entry_->module_shutdown_func  = on_module_shutdown;
		entry_->request_startup_func  = on_request_startup;
		entry_->request_shutdown_func = on_request_shutdown;
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
	int extension_entry::on_module_startup(int type, int module_number) {
		self_->module_number = module_number;
		// ini 注册
		std::std::vector<zend_ini_entry_def> defs(self_->ini_entries_.size() + 1);
		int index = 0;
		for(auto i=self_->ini_entries_.begin(); i!= self->ini_entries_.end(); ++i) {
			i->fill(defs[index++]);
		}
		// ini 注册停止条件
		defs[index].name = nullptr;
		zend_register_ini_entries(defs, module_number);
		// 常量注册
		for(auto i=self_->constant_entries_.begin(); i!= self->constant_entries_.end(); ++i) {
			zend_constant c;
			i->fill(&c);
			c.module_number = module_number;
			zend_register_constant(&c);
		}
		// self.do_register_constants(self._module);
		// 完成 classes 注册
		// for(auto i=self._classes.begin();i!=self._classes.end();++i) {
		// 	(*i).do_register();
		// }
		// TODO 调用回调 Slot
		return ZEND_RESULT_CODE::SUCCESS;
	}
	int extension_entry::on_module_shutdown (int type, int module_number) {
		zend_unregister_ini_entries(module_number);
		// TODO 调用回调 Slot
		return ZEND_RESULT_CODE::SUCCESS;
	}
	int extension_entry::on_request_startup (int type, int module_number) {
		// TODO 调用回调 Slot
		return ZEND_RESULT_CODE::SUCCESS;
	}
	int extension_entry::on_request_shutdown(int type, int module_number) {
		// TODO 调用回调 Slot
		return ZEND_RESULT_CODE::SUCCESS;
	}

	extension_entry& extension_entry::add(const ini_entry& entry) {
		ini_entries_.push_back(entry);
		return *this;
	}
	extension_entry& extension_entry::add(const constant_entry& entry) {
		constant_entries_.push(entry);
		return *this;
	}
}
