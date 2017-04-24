#include "../phpext.h"

namespace php {
	void fail(const std::string& msg) {
		zend_error(E_USER_ERROR, msg.c_str());
	}
    void fail(const char *format, ...) {
        va_list args;

        va_start(args, format);
        php_verror(NULL, "", E_USER_ERROR, format, args);
        va_end(args);
	}

	void warn(const std::string& msg) {
		zend_error(E_USER_WARNING, msg.c_str());
	}
    void warn(const char *format, ...) {
        va_list args;

        va_start(args, format);
        php_verror(NULL, "", E_USER_WARNING, format, args);
        va_end(args);
	}


	void info(const std::string& msg) {
		zend_error(E_USER_NOTICE, msg.c_str());
	}
    void info(const char *format, ...) {
        va_list args;

        va_start(args, format);
        php_verror(NULL, "", E_USER_NOTICE, format, args);
        va_end(args);
	}



    const char* ini_get(char* name, uint name_len) {
        return zend_ini_string(name, name_len, 0);
    }

    std::string sha1(const unsigned char* enc_str, size_t enc_len) {
        char sha1str[41] = {0};
        PHP_SHA1_CTX context;
        unsigned char digest[20];

        PHP_SHA1Init(&context);
        PHP_SHA1Update(&context, enc_str, enc_len);
        PHP_SHA1Final(digest, &context);
        make_digest_ex(sha1str, digest, 20);

        return std::string(sha1str);
    }

    std::string md5(char* enc_str, uint32_t enc_len) {
        char md5str[33] = {0};
        PHP_MD5_CTX context;
        unsigned char digest[16];

        PHP_MD5Init(&context);
        PHP_MD5Update(&context, enc_str, enc_len);
        PHP_MD5Final(digest, &context);
        make_digest_ex(md5str, digest, 16);
        return std::string(md5str);
    }

    uint32_t crc32(unsigned char* src, uint32_t src_len) {
        register uint32_t crc32_val = 0;
        for(; --src_len; ++src) {
            crc32_val = ((crc32_val >> 8) & 0x00FFFFFF) ^ crc32tab[(crc32_val ^ (*src)) & 0xFF ];
        }
        return crc32_val;
    }

    std::shared_ptr<zend_string> base64_encode(const unsigned char* enc_str, size_t enc_len) {
        return std::shared_ptr<zend_string>(php_base64_encode(enc_str, enc_len), zend_string_release);
    }
    std::shared_ptr<zend_string> base64_decode(const unsigned char* dec_str, size_t dec_len) {
        //php_base64_decode_ex(dec_str, dec_len, 0);
        return std::shared_ptr<zend_string>(php_base64_decode_ex(dec_str, dec_len, 0), zend_string_release);

    }

    std::shared_ptr<zend_string> url_encode(const char* enc_str, size_t enc_len) {
        return std::shared_ptr<zend_string>(php_url_encode(enc_str, enc_len), zend_string_release);
    }

    std::shared_ptr<zend_string> url_decode(char* dec_str, size_t& dec_len) {
        zend_string* url = zend_string_init(dec_str, dec_len, false);
        url->len = php_url_decode(url->val, url->len);
        return std::shared_ptr<zend_string>(url, zend_string_release);
    }

    static char hexconvtab[] = "0123456789abcdef";
    static zend_string *php_bin2hex(const unsigned char *old, const size_t oldlen)
    {
        zend_string *result;
        size_t i, j;

        result = zend_string_safe_alloc(oldlen, 2 * sizeof(char), 0, 0);

        for (i = j = 0; i < oldlen; i++) {
            ZSTR_VAL(result)[j++] = hexconvtab[old[i] >> 4];
            ZSTR_VAL(result)[j++] = hexconvtab[old[i] & 15];
        }
        ZSTR_VAL(result)[j] = '\0';

        return result;
    }

    static zend_string *php_hex2bin(const unsigned char *old, const size_t oldlen)
    {
        size_t target_length = oldlen >> 1;
        zend_string *str = zend_string_alloc(target_length, 0);
        unsigned char *ret = (unsigned char *)ZSTR_VAL(str);
        size_t i, j;

        for (i = j = 0; i < target_length; i++) {
            unsigned char c = old[j++];
            unsigned char l = c & ~0x20;
            int is_letter = ((unsigned int) ((l - 'A') ^ (l - 'F' - 1))) >> (8 * sizeof(unsigned int) - 1);
            unsigned char d;

            /* basically (c >= '0' && c <= '9') || (l >= 'A' && l <= 'F') */
            if (EXPECTED((((c ^ '0') - 10) >> (8 * sizeof(unsigned int) - 1)) | is_letter)) {
                d = (l - 0x10 - 0x27 * is_letter) << 4;
            } else {
                zend_string_free(str);
                return NULL;
            }
            c = old[j++];
            l = c & ~0x20;
            is_letter = ((unsigned int) ((l - 'A') ^ (l - 'F' - 1))) >> (8 * sizeof(unsigned int) - 1);
            if (EXPECTED((((c ^ '0') - 10) >> (8 * sizeof(unsigned int) - 1)) | is_letter)) {
                d |= l - 0x10 - 0x27 * is_letter;
            } else {
                zend_string_free(str);
                return NULL;
            }
            ret[i] = d;
        }
        ret[i] = '\0';

        return str;
    }

    std::shared_ptr<zend_string> hex2bin(unsigned char* src, size_t src_len) {
        if(src_len % 2 != 0) {
            return nullptr;
        }
        return std::shared_ptr<zend_string>(php_hex2bin(src, src_len), zend_string_release);
    }

    std::shared_ptr<zend_string> bin2hex(unsigned char* src, size_t src_len) {
        if(src_len % 2 != 0) {
            return nullptr;
        }
        return std::shared_ptr<zend_string>(php_bin2hex(src, src_len), zend_string_release);
    }

    std::shared_ptr<zend_string> json_encode(zval* val) {
        smart_str buf = {0};

        JSON_G(error_code) = PHP_JSON_ERROR_NONE;
        JSON_G(encode_max_depth) = PHP_JSON_PARSER_DEFAULT_DEPTH;
        php_json_encode(&buf, val, PHP_JSON_UNESCAPED_UNICODE);

        if (JSON_G(error_code) != PHP_JSON_ERROR_NONE && !(PHP_JSON_UNESCAPED_UNICODE & PHP_JSON_PARTIAL_OUTPUT_ON_ERROR)) {
            return nullptr;
        } else {
            ZSTR_VAL(buf.s)[ZSTR_LEN(buf.s)] = '\0';
            return std::shared_ptr<zend_string>(buf.s, zend_string_release);
        }
    }

    php::value json_decode(char* src, int32_t src_len) {
        int32_t depth = PHP_JSON_PARSER_DEFAULT_DEPTH;
        int32_t options = PHP_JSON_OBJECT_AS_ARRAY;
        // zval result;
        php::value v(nullptr);
        php_json_decode_ex(v.data(), src, src_len, options, depth);
        if(v.is_null())
            return nullptr;
        else
            return std::move(v);
    }

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

    std::shared_ptr<php_url> parse_url(const char* url, size_t url_len) {
        return std::shared_ptr<php_url>(php_url_parse_ex(url, url_len), php_url_free);
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

    /*static zend_string *php_zlib_encode(const char *in_buf, size_t in_len, int encoding, int level)*/
    //{
        //int status;
        //z_stream Z;
        //zend_string *out;

        //memset(&Z, 0, sizeof(z_stream));
        //Z.zalloc = php_zlib_alloc;
        //Z.zfree = php_zlib_free;

        //if (Z_OK == (status = deflateInit2(&Z, level, Z_DEFLATED, encoding, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY))) {
            //out = zend_string_alloc(PHP_ZLIB_BUFFER_SIZE_GUESS(in_len), 0);

            //Z.next_in = (Bytef *) in_buf;
            //Z.next_out = (Bytef *) ZSTR_VAL(out);
            //Z.avail_in = in_len;
            //Z.avail_out = ZSTR_LEN(out);

            //status = deflate(&Z, Z_FINISH);
            //deflateEnd(&Z);

            //if (Z_STREAM_END == status) {
                //[> size buffer down to actual length <]
                //out = zend_string_truncate(out, Z.total_out, 0);
                //ZSTR_VAL(out)[ZSTR_LEN(out)] = '\0';
                //return out;
            //} else {
                //zend_string_free(out);
            //}
        //}

        //php_error_docref(NULL, E_WARNING, "%s", zError(status));
        //return NULL;
    //}

    //std::shared_ptr<zend_string> gzencode(char* src, size_t src_len, int level) {
        //return std::shared_ptr<zend_string>(php_zlib_encode(src, src_len, PHP_ZLIB_ENCODING_GZIP, level), zend_string_release);
    //}

    //static int php_zlib_decode(const char *in_buf, size_t in_len, char **out_buf, size_t *out_len, int encoding, size_t max_len)
    //{
        //int status = Z_DATA_ERROR;
        //z_stream Z;

        //memset(&Z, 0, sizeof(z_stream));
        //Z.zalloc = php_zlib_alloc;
        //Z.zfree = php_zlib_free;

        //if (in_len) {
//retry_raw_inflate:
            //status = inflateInit2(&Z, encoding);
            //if (Z_OK == status) {
                //Z.next_in = (Bytef *) in_buf;
                //Z.avail_in = in_len + 1; [> NOTE: data must be zero terminated <]

                //switch (status = php_zlib_inflate_rounds(&Z, max_len, out_buf, out_len)) {
                    //case Z_STREAM_END:
                        //inflateEnd(&Z);
                        //return SUCCESS;

                    //case Z_DATA_ERROR:
                        //[> raw deflated data? <]
                        //if (PHP_ZLIB_ENCODING_ANY == encoding) {
                            //inflateEnd(&Z);
                            //encoding = PHP_ZLIB_ENCODING_RAW;
                            //goto retry_raw_inflate;
                        //}
                //}
                //inflateEnd(&Z);
            //}
        //}

        //*out_buf = NULL;
        //*out_len = 0;

        //php_error_docref(NULL, E_WARNING, "%s", zError(status));
        //return FAILURE;
    //}

    //php::value gzdecode(char* src, size_t src_len) {
        //char* dst;
        //size_t dst_len;

        //if(SUCCESS != php_zlib_decode(src, src_len, &dst, &dst_len, PHP_ZLIB_ENCODING_GZIP, 0)) {
            //return nullptr;
        //}
        //return php::value(dst, dst_len);
    /*}*/
}
