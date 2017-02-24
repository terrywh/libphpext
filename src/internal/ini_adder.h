namespace phpext {
	namespace internal {
		class ini_adder {
		public:
			ini_adder& add(const char* name, const char* dval = "");
			ini_adder& add(const char* name, std::int64_t dval);
			ini_adder& add(const char* name, double dval);
			ini_adder& add(const char* name, bool dval);
		protected:
			void do_register_ini_entries(int module);
		private:
			void _add(zend_string* key, zend_string* val);
			std::vector<zend_ini_entry*> _entries;
		};
	}
}
