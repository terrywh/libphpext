#include "../phpext.h"

namespace php {

	static void php_simple_ini_parser_cb(zval *arg1, zval *arg2, zval *arg3, int callback_type, zval *arr)
	{
		switch (callback_type) {

			case ZEND_INI_PARSER_ENTRY:
			if (!arg2) {
				/* bare string - nothing to do */
				break;
			}
			Z_TRY_ADDREF_P(arg2);
			zend_symtable_update(Z_ARRVAL_P(arr), Z_STR_P(arg1), arg2);
			break;

			case ZEND_INI_PARSER_POP_ENTRY:
			{
				zval hash, *find_hash;

				if (!arg2) {
					/* bare string - nothing to do */
					break;
				}

				if (!(Z_STRLEN_P(arg1) > 1 && Z_STRVAL_P(arg1)[0] == '0') && is_numeric_string(Z_STRVAL_P(arg1), Z_STRLEN_P(arg1), NULL, NULL, 0) == IS_LONG) {
					zend_ulong key = (zend_ulong) zend_atol(Z_STRVAL_P(arg1), (int)Z_STRLEN_P(arg1));
					if ((find_hash = zend_hash_index_find(Z_ARRVAL_P(arr), key)) == NULL) {
						array_init(&hash);
						find_hash = zend_hash_index_update(Z_ARRVAL_P(arr), key, &hash);
					}
				} else {
					if ((find_hash = zend_hash_find(Z_ARRVAL_P(arr), Z_STR_P(arg1))) == NULL) {
						array_init(&hash);
						find_hash = zend_hash_update(Z_ARRVAL_P(arr), Z_STR_P(arg1), &hash);
					}
				}

				if (Z_TYPE_P(find_hash) != IS_ARRAY) {
					zval_dtor(find_hash);
					array_init(find_hash);
				}

				if (!arg3 || (Z_TYPE_P(arg3) == IS_STRING && Z_STRLEN_P(arg3) == 0)) {
					Z_TRY_ADDREF_P(arg2);
					add_next_index_zval(find_hash, arg2);
				} else {
					array_set_zval_key(Z_ARRVAL_P(find_hash), arg3, arg2);
				}
			}
			break;

			case ZEND_INI_PARSER_SECTION:
			break;
		}
	}

	static void php_ini_parser_cb_with_sections(zval *arg1, zval *arg2, zval *arg3, int callback_type, zval *arr)
	{
		if (callback_type == ZEND_INI_PARSER_SECTION) {
			array_init(&BG(active_ini_file_section));
			zend_symtable_update(Z_ARRVAL_P(arr), Z_STR_P(arg1), &BG(active_ini_file_section));
		} else if (arg2) {
			zval *active_arr;

			if (Z_TYPE(BG(active_ini_file_section)) != IS_UNDEF) {
				active_arr = &BG(active_ini_file_section);
			} else {
				active_arr = arr;
			}

			php_simple_ini_parser_cb(arg1, arg2, arg3, callback_type, active_arr);
		}
	}

	php::value parse_ini_file(char* filename) {
		php::value v(nullptr);

		zend_file_handle fh;
		memset(&fh, 0x00, sizeof(fh));
		fh.filename = filename;
		fh.type     = ZEND_HANDLE_FILENAME;

		ZVAL_UNDEF(&BG(active_ini_file_section));
		if (zend_parse_ini_file(&fh, 0, ZEND_INI_SCANNER_TYPED, (zend_ini_parser_cb_t) php_ini_parser_cb_with_sections, v.data()) == FAILURE) {
			return nullptr;
		}
		return std::move(v);
	}
}
