#ifndef IH_CURL_UTILS_H
#define IH_CURL_UTILS_H

#include <curl/curl.h>

namespace IH
{
	class CurlUtils
	{
	public:

		static int writer(char *data, size_t size, size_t nmemb, std::string *out)
		{
			int result = 0;
			if(out)
			{
				out->append(data, size * nmemb);
				result = size * nmemb;
			}
			return result;
		}

		static String urlencode(const String& data)
		{
			std::stringstream out;
			for(int i = 0; i < data.size(); ++i)
			{
				char c = data[i];
				if(!isalnum(c))
					out<<"%"<<toHex(c);
				else
					out<<c;
			}
		}

		static String toHex(char c)
		{
			std::stringstream ss;
			ss<<hex<<c;
			String out = ss.str();
			if(out.length == 1)
				out = "0" + out;
			return out;
		}

	private:

		// no instances should ever be created
		CurlUtils(){};

	}
}

#endif
