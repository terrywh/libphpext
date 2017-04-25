#include "../phpext.h"

namespace php {
	std::shared_ptr<php_url> parse_url(const char* url, size_t url_len) {
		return std::shared_ptr<php_url>(php_url_parse_ex(url, url_len), php_url_free);
	}

	php::value parse_str(const char sep, char* buf, int len) {
		php::value kv = php::value::array(0);
		char *kbeg,*kend,*vbeg,*vend;
		int x = 0;
		for(int i=0;i<len;++i) {
			switch(x) {
				case 0:
					if(!std::isspace(buf[i]) && buf[i] != '=') {
						kbeg = kend = buf+i;
						++x;
					}
				break;
				case 1:
					if(!std::isspace(buf[i]) && buf[i] != '=') {
						kend = buf+i;
					}else if(buf[i] == '=') {
						++x;
					}
				break;
				case 2:
					if(!std::isspace(buf[i]) && buf[i] != sep) {
						vbeg = vend = buf+i;
						++x;
					}
				break;
				case 3:
					if(!std::isspace(buf[i]) && buf[i] != sep) {
						vend = buf+i;
					}else if(buf[i] == sep) {
						php_url_decode(vbeg, vend-vbeg+1);
						php::value item = kv.item(kbeg, kend-kbeg+1);
						item = php::value(vbeg, vend-vbeg+1);
						x=0;
					}
				break;
			}
		}
		if(x==3) {
			php_url_decode(vbeg, vend-vbeg+1);
			php::value item = kv.item(kbeg, kend-kbeg+1);
			item = php::value(vbeg, vend-vbeg+1);
		}
		return kv;
	}
}
