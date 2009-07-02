#include "acsetup.hpp"

#include <cstdio>
#include <cstring>

#include "nextweb/Error.hpp"
#include "nextweb/HttpUtils.hpp"

#include "details/UrlEncode.hpp"
#include "details/SystemError.hpp"

namespace nextweb {

static char const *httpDays[] = {
	"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

static char const *httpMonths[] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun", 
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

static char const *httpDateFormats[] = { 
	"%a, %d %b %Y %T GMT", "%A, %d-%b-%y %T GMT", "%a %b %d %T %Y"
};

std::time_t
parseHttpDate(char const *buffer) {

	struct tm ts;
	memset(&ts, 0, sizeof(struct tm));
	for (std::size_t i = 0; i < sizeof(httpDateFormats) / sizeof(char const*); ++i) {
		if (0 != strptime(buffer, httpDateFormats[i], &ts)) {
			return timegm(&ts);
		}
	}
	return static_cast<std::time_t>(0);
}

void
printHttpDate(char *buffer, std::size_t size, std::time_t what) {

	struct tm ts;
	memset(&ts, 0, sizeof(struct tm));
	details::SystemError::throwUnless(0 != gmtime_r(&what, &ts));
	
	int res = snprintf(buffer, size - 1, "%s, %02d %s %04d %02d:%02d:%02d GMT",
		httpDays[ts.tm_wday], ts.tm_mday, httpMonths[ts.tm_mon], 
		ts.tm_year + 1900,  ts.tm_hour, ts.tm_min, ts.tm_sec);

	details::SystemError::throwUnless(res > 0);
}

StandardSetup::StringType
urlencode(StandardSetup::StringType const &value) {
	return details::urlencode(value);
}

StandardSetup::StringType
urldecode(StandardSetup::StringType const &value) {
	return details::urldecode(value);
}

} // namespace
