namespace phpext { namespace internal {
	class arg_info: public zend_internal_arg_info {
	public:
		arg_info(string&& name, bool reqired = true, bool by_ref = false);
		arg_info(string&& name, zend_uchar type, bool reqired = true, bool by_ref = false);
		arg_info(string&& name, string&& class_name, bool reqired = true, bool by_ref = false);
	};
} }
