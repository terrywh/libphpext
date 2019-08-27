#pragma once

#include "ini_entry.h"
#include "constant_entry.h"
#include "delegate.h"
#include "arguments.h"
#include "class_entry.h"
#include "resource_entry.h"

namespace php {
	class extension_entry {
	private:
		static extension_entry* self;
		//
		std::string                                              name_;
		std::string                                           version_;
		zend_module_entry                                       entry_;
		zend_module_dep                                  dependencies_[4];
		std::vector<std::shared_ptr<ini_entry>>           ini_entries_;
		std::vector<std::shared_ptr<constant_entry>> constant_entries_;
		std::vector<zend_function_entry>             function_entries_;
		std::vector<arguments>                              arguments_;
		std::vector<class_entry_base*>                  class_entries_;
		std::vector<resource_entry_base*>               resrc_entries_;
		std::vector<std::pair<std::string, std::string>>  decriptions_;

		std::list<std::function<bool(extension_entry&)>> handler_rsd_;
		std::list<std::function<bool(extension_entry&)>> handler_rst_;
		std::list<std::function<bool(extension_entry&)>> handler_mst_;
		std::list<std::function<bool(extension_entry&)>> handler_msd_;
	public:
		
		extension_entry(const std::string& name, const std::string& version);
		extension_entry& ini(const ini_entry& entry);
		extension_entry& constant(const constant_entry& entry);
		// 函数
		template<value FUNCTION(parameters& params)>
		extension_entry& function(const char* name, arguments&& desc) {
			arguments_.emplace_back(std::move(desc));
			arguments& argv = arguments_.back();
			function_entries_.emplace_back(zend_function_entry {
				name, // fname
				function_delegate<FUNCTION>, // handler
				argv, // arg_info,
				argv.size(), // num_args
				0,
			});
			return *this;
		}
		template<value FUNCTION(parameters& params)>
		extension_entry& function(const char* name) {
			function_entries_.emplace_back(zend_function_entry {
				name, // fname
				function_delegate<FUNCTION>, // handler
				nullptr, // arg_info,
				0, // num_args
				0,
			});
			return *this;
		}
		// 类
		template <class CLASS_TYPE>
		extension_entry& add(class_entry<CLASS_TYPE>&& entry) {
			class_entries_.emplace_back(new class_entry<CLASS_TYPE>(std::move(entry)) );
			return *this;
		}
		template <typename T>
		extension_entry& resource(resource_entry<T>&& entry) {
			resrc_entries_.emplace_back(new resource_entry<T>(std::move(entry)));
			return *this;
		}
		extension_entry& desc(std::pair<std::string, std::string> kv);
		operator zend_module_entry*();
		extension_entry& on_module_startup(std::function<bool (extension_entry&)> handler);
		extension_entry& on_module_shutdown(std::function<bool (extension_entry&)> handler);
		extension_entry& on_request_startup(std::function<bool (extension_entry&)> handler);
		extension_entry& on_request_shutdown(std::function<bool (extension_entry&)> handler);
	private:
		// 扩展回调函数
		static int on_module_startup_handler  (int type, int module);
		static int on_module_shutdown_handler (int type, int module);
		static int on_request_startup_handler (int type, int module);
		static int on_request_shutdown_handler(int type, int module);
		static void on_module_info_handler(zend_module_entry *zend_module);
	};
}
