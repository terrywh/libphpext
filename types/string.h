#pragma

namespace php {
	class value;
	class string {
	private:
		zend_string* str_;
		string(zend_string* str);
	public:
		~string();
		string():str_(nullptr) {}
		static string clone(const string& str);
		string(const string& str);
		string(string&& str);
		string(std::size_t size, bool persistent=false);
		string(const char* val, std::size_t len, bool persistent=false);
		string(const std::string& str);

		inline std::size_t length() const {
			return str_->len;
		}
		inline char* data() {
			return str_->val;
		}
		inline const char* c_str() const {
			return str_->val;
		}
		inline operator zend_string*() {
			return str_;
		}
		string substr(std::size_t pos, std::size_t count = -1);
		static zend_string* concat(zend_string* s1, zend_string* s2);
		inline static string concat(const string& s1, const string& s2) {
			return string(concat(s1.str_, s2.str_));
		}
		friend class value;
	};
}
