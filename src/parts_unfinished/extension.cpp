#include "../core.h"

namespace phpext {
	// _entry 结构参见 Zend/zend_modules.h:73
	extension::extension(const char* name, const char* version) {
		// 全局实例，方便访问
		_extension = this;
		_entry.size                  = sizeof(_entry);
		_entry.zend_api              = ZEND_MODULE_API_NO;
		_entry.zend_debug            = ZEND_DEBUG;
		_entry.zts                   = USING_ZTS;
		_entry.ini_entry             = nullptr; // 此项数据由 zend engine 填充
		_entry.deps                  = nullptr;
		_entry.name                  = name;
		_entry.functions             = nullptr; // 这里暂时设置为 nullptr 后面在所有 extension 处理完成后，统一生成
		_entry.module_startup_func   = extension::on_module_startup;
		_entry.module_shutdown_func  = extension::on_module_shutdown;
		_entry.request_startup_func  = extension::on_request_startup;
		_entry.request_shutdown_func = extension::on_request_shutdown;
		_entry.info_func             = nullptr;
		_entry.version               = version;
		_entry.globals_size          = 0;
#ifdef ZTS
		_entry.globals_id_ptr = nullptr;
#else
		_entry.globals_ptr = nullptr;
#endif
		_entry.globals_ctor = nullptr;
		_entry.globals_dtor = nullptr;
		_entry.post_deactivate_func = nullptr;
		_entry.module_started = 0;
		_entry.type = 0;
		_entry.handle = nullptr;
		_entry.module_number = 0;
		_entry.build_id = ZEND_MODULE_BUILD_ID;
	}
	extension* extension::_extension = nullptr;
	inline extension& extension::get() {
		return *_extension;
	}
	extension::operator zend_module_entry*() {
		_entry.functions = do_register_functions();
		// TODO 注册内部类
		return &_entry;
	}
	extension& extension::add(class_entry_base&& entry) {
		_classes.emplace_back(entry);
		return *this;
	}
	// 扩展回调函数
	int extension::on_module_startup(int type, int module) {
		// @@@@ 不能使用 auto，否则编译时会引起 ini 拷贝构造问题，原因不明，哪位大神可以给解释下？
		// auto self = extension::get();
		extension& self = extension::get();
		self._module = module;

		self.do_register_ini_entries(self._module);
		self.do_register_constants(self._module);
		// 完成 classes 注册
		for(auto i=self._classes.begin();i!=self._classes.end();++i) {
			(*i).do_register();
		}
		// TODO 调用回调 Slot
		return ZEND_RESULT_CODE::SUCCESS;
	}
	int extension::on_module_shutdown (int type, int module) {
		zend_unregister_ini_entries(module);
		// TODO 调用回调 Slot
		return ZEND_RESULT_CODE::SUCCESS;
	}
	int extension::on_request_startup (int type, int module) {
		// TODO 调用回调 Slot
		return ZEND_RESULT_CODE::SUCCESS;
	}
	int extension::on_request_shutdown(int type, int module) {
		// TODO 调用回调 Slot
		return ZEND_RESULT_CODE::SUCCESS;
	}



}
