namespace phpext {
	namespace internal {
		class constant_adder {
		public:
			constant_adder& add(const constant& c);
		protected:
			void do_register_constants(int module);
		private:
			std::deque<zend_constant> _constants;
		};
	}
}