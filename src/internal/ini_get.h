namespace phpext {
	// 用于获取 ini_entry 配置项；这里用 class 形式，方便进行类型转化；
	// 实际使用时类似函数使用:
	// @example
	// 	phpext::string val = ini_get("section.key");
	class ini_get {
	public:
		ini_get(const char* key);
		// 允许获取下面 3 中类型
		// -----------------------------------------------
		operator std::int64_t();
		operator double();
		operator string();
	private:
		string _key;
	};
}
