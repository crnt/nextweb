#include "acsetup.hpp"
#include "nextweb/fastcgi/HttpDate.hpp"

#include <limits>
#include <cerrno>
#include <cstdio>
#include <cassert>
#include <sstream>

#include "nextweb/Error.hpp"
#include "nextweb/utils/Range.hpp"
#include "nextweb/utils/TypeList.hpp"
#include "nextweb/utils/TypeTraits.hpp"
#include "nextweb/utils/StringUtils.hpp"
#include "nextweb/utils/SystemError.hpp"
#include "nextweb/utils/StaticAssert.hpp"

namespace nextweb { namespace fastcgi {

static char const*
DATE_FORMATS[] = { 
	"%a, %d %b %Y %T GMT", "%A, %d-%b-%y %T GMT", "%a %b %d %T %Y" 
};

static char const*
SHORT_MONTH_NAMES[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

static char const*
LONG_WEEKDAY_NAMES[] = {
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

static char const*
SHORT_WEEKDAY_NAMES[] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

HttpDate const
HttpDate::BAD(std::numeric_limits<std::time_t>::max());

namespace periods {

template <typename Char>
struct SignTraits;

template <>
struct SignTraits<char> {
	static int sign(char value);
	static bool isSign(char value);
};

template <typename Range>
class Chunk {

public:
	Chunk();
	Chunk(char letter, Range const &range);
	
	char letter() const;
	void letter(char value);
	
	int convert() const;
	void range(Range const &value);
	
private:
	char letter_;
	Range range_;
};

template <typename Range, char Letter>
struct Evaluator {
	static bool accepts(Chunk<Range> const &chunk);
};

template <typename Range, typename List>
struct EvaluatorImpl {
	static void evaluate(Chunk<Range> const &chunk, int sign, std::tm &when);
};

template <typename Range>
struct EvaluatorImpl<Range, utils::NullType> {
	static void evaluate(Chunk<Range> const &chunk, int sign, std::tm &when);
};

template <typename Range>
struct DayEvaluator : public Evaluator<Range, 'd'> {
	typedef DayEvaluator<Range> Type;
	static void evaluate(Chunk<Range> const &chunk, int sign, std::tm &when);
};

template <typename Range>
struct HourEvaluator : public Evaluator<Range, 'h'> {
	typedef HourEvaluator<Range> Type;
	static void evaluate(Chunk<Range> const &chunk, int sign, std::tm &when);
};

template <typename Range>
struct YearEvaluator : public Evaluator<Range, 'y'> {
	typedef YearEvaluator<Range> Type;
	static void evaluate(Chunk<Range> const &chunk, int sign, std::tm &when);
};

template <typename Range>
struct MonthEvaluator : public Evaluator<Range, 'M'> {
	typedef MonthEvaluator<Range> Type;
	static void evaluate(Chunk<Range> const &chunk, int sign, std::tm &when);
};

template <typename Range>
struct MinuteEvaluator : public Evaluator<Range, 'm'> {
	typedef MinuteEvaluator<Range> Type;
	static void evaluate(Chunk<Range> const &chunk, int sign, std::tm &when);
};

template <typename Range>
struct SecondEvaluator : public Evaluator<Range, 's'> {
	typedef SecondEvaluator<Range> Type;
	static void evaluate(Chunk<Range> const &chunk, int sign, std::tm &when);
};

template <typename Range>
struct EvaluatorList {
	typedef NEXTWEB_MAKE_TYPE_LIST6(DayEvaluator<Range>, HourEvaluator<Range>, YearEvaluator<Range>, MonthEvaluator<Range>, MinuteEvaluator<Range>, SecondEvaluator<Range>) Type;
};

template <typename Range> void
evaluateRange(Range const &range, std::tm &when, int majorSign);

} // namespace periods

HttpDate::HttpDate() :
	when_(time(NULL))
{
}

HttpDate::HttpDate(std::time_t t) :
	when_(t)
{
}

HttpDate::~HttpDate() {
}

HttpDate::HttpDate(HttpDate const &other) :
	when_(other.when_)
{
}

HttpDate&
HttpDate::operator = (HttpDate const &other) {
	HttpDate temp(other);
	swap(temp);
	return *this;
}

void
HttpDate::swap(HttpDate &other) throw () {
	std::swap(when_, other.when_);
} 

HttpDate
HttpDate::fromString(char const *str) {
	std::tm rep;
	for (unsigned int i = 0; i < sizeof(DATE_FORMATS) / sizeof(char const*); ++i) {
		if (NULL != strptime(str, DATE_FORMATS[i], &rep)) {
			return HttpDate(timegm(&rep));
		}
	}
	return HttpDate().add(str);
}

HttpDate
HttpDate::fromString(std::string const &str) {
	return fromString(str.c_str());
}

HttpDate
HttpDate::fromPeriod(char const *period) {
	HttpDate date;
	date.add(period);
	return date;
}

HttpDate
HttpDate::fromPeriod(std::string const &period) {
	return fromPeriod(period.c_str());
}

std::string
HttpDate::str() const {
	return asRFC1123();
}

std::string
HttpDate::asRFC1123() const {

	struct tm ts;
	if (NULL == gmtime_r(&when_, &ts)) {
		throw Error("date <%llu> format failed", static_cast<unsigned long long>(when_));
	}

	char buffer[255];
	int res = snprintf(buffer, sizeof(buffer) - 1, "%s, %02d %s %04d %02d:%02d:%02d GMT",
		SHORT_WEEKDAY_NAMES[ts.tm_wday], ts.tm_mday, SHORT_MONTH_NAMES[ts.tm_mon], ts.tm_year + 1900,
		ts.tm_hour, ts.tm_min, ts.tm_sec);
	if (res < 0) {
		throw Error("date <%llu> format failed with error code %d", 
			static_cast<unsigned long long>(when_), errno);
	}
	return std::string(buffer, buffer + res);
}

std::string
HttpDate::asRFC1036() const {

	struct tm ts;
	if (NULL == gmtime_r(&when_, &ts)) {
		throw Error("date <%llu> format failed", static_cast<unsigned long long>(when_));
	}

	char buffer[255];
	int res = snprintf(buffer, sizeof(buffer) - 1, "%s, %02d-%s-%02d %02d:%02d:%02d GMT",
		LONG_WEEKDAY_NAMES[ts.tm_wday], ts.tm_mday, SHORT_MONTH_NAMES[ts.tm_mon], ts.tm_year % 100,
		ts.tm_hour, ts.tm_min, ts.tm_sec);
	if (res < 0) {
		throw Error("date <%llu> format failed with error code %d", 
			static_cast<unsigned long long>(when_), errno);
	}
	return std::string(buffer, buffer + res);
}

std::string
HttpDate::asAsctime() const {

	struct tm ts;
	if (NULL == gmtime_r(&when_, &ts)) {
		throw Error("date <%llu> format failed", static_cast<unsigned long long>(when_));
	}

	char buffer[255];
	int res = snprintf(buffer, sizeof(buffer) - 1, "%s %s %d %02d:%02d:%02d %04d", 
		SHORT_WEEKDAY_NAMES[ts.tm_wday], SHORT_MONTH_NAMES[ts.tm_mon], ts.tm_mday, 
		ts.tm_hour, ts.tm_min, ts.tm_sec, ts.tm_year + 1900);
	if (res < 0) {
		throw Error("date <%llu> format failed with error code %d", 
			static_cast<unsigned long long>(when_), errno);
	}
	return std::string(buffer, buffer + res);
}

HttpDate&
HttpDate::add(char const *period) {
	std::tm rep;
	utils::SystemError::throwUnless(gmtime_r(&when_, &rep));
	periods::evaluateRange(utils::makeRange(period), rep, 1);
	when_ = timegm(&rep);
	return *this;
}

HttpDate&
HttpDate::sub(char const *period) {
	std::tm rep;
	utils::SystemError::throwUnless(gmtime_r(&when_, &rep));
	periods::evaluateRange(utils::makeRange(period), rep, -1);
	when_ = timegm(&rep);
	return *this;
}

namespace periods {

inline int
SignTraits<char>::sign(char value) {
	return ('-' == value) ? -1 : 1;
}

inline bool
SignTraits<char>::isSign(char value) {
	return ('-' == value) || ('+' == value);
}

template <typename Range> inline 
Chunk<Range>::Chunk() :
	letter_(), range_()
{
}

template <typename Range> inline 
Chunk<Range>::Chunk(char letter, Range const &range) :
	letter_(letter), range_(range)
{
}

template <typename Range> inline char
Chunk<Range>::letter() const {
	return letter_;
}

template <typename Range> inline void
Chunk<Range>::letter(char value) {
	letter_ = value;
}

template <typename Range> inline int
Chunk<Range>::convert() const {
	int result;
	try {
		std::basic_stringstream<typename Range::value_type> stream;
		stream.exceptions(std::ios::badbit | std::ios::failbit);
		stream << range_;
		stream >> result;
		return result;
	}
	catch (std::ios::failure const &) {
		throw Error("can not parse number in period format");
	}
}

template <typename Range> inline void
Chunk<Range>::range(Range const &value) {
	range_ = value;
}

template <typename Range, char Letter> inline bool
Evaluator<Range, Letter>::accepts(Chunk<Range> const &chunk) {
	return chunk.letter() == Letter;
}

template <typename Range> inline void
DayEvaluator<Range>::evaluate(Chunk<Range> const &chunk, int sign, std::tm &when) {
	when.tm_mday += (sign * chunk.convert());
}

template <typename Range> inline void
HourEvaluator<Range>::evaluate(Chunk<Range> const &chunk, int sign, std::tm &when) {
	when.tm_hour += (sign * chunk.convert());
}

template <typename Range> inline void
YearEvaluator<Range>::evaluate(Chunk<Range> const &chunk, int sign, std::tm &when) {
	when.tm_year += (sign * chunk.convert());
}

template <typename Range> inline void
MonthEvaluator<Range>::evaluate(Chunk<Range> const &chunk, int sign, std::tm &when) {
	when.tm_mon += (sign * chunk.convert());
}

template <typename Range> inline void
MinuteEvaluator<Range>::evaluate(Chunk<Range> const &chunk, int sign, std::tm &when) {
	when.tm_min += (sign * chunk.convert());
}

template <typename Range> inline void
SecondEvaluator<Range>::evaluate(Chunk<Range> const &chunk, int sign, std::tm &when) {
	when.tm_sec += (sign * chunk.convert());
}

template <typename Range, typename List> inline void 
EvaluatorImpl<Range, List>::evaluate(Chunk<Range> const &chunk, int sign, std::tm &when) {
	typedef typename List::Value Type;
	if (Type::accepts(chunk)) {
		Type::evaluate(chunk, sign, when);
	}
	else {
		EvaluatorImpl<Range, typename List::Next>::evaluate(chunk, sign, when);
	}
}

template <typename Range> inline void
EvaluatorImpl<Range, utils::NullType>::evaluate(Chunk<Range> const &chunk, int sign, std::tm &when) {
	(void) sign;
	(void) when;
	throw Error("bad period format specifier: %c", chunk.letter());
}

template <typename Range> std::pair<Chunk<Range>, Range>
nextChunk(Range const &range) {
	
	assert(!range.empty());
	typedef typename Range::value_type Char;
	
	Chunk<Range> chunk;
	typename Range::const_iterator i = utils::nextNotMatched(range.begin(), range.end(), utils::IsNumeric<Char>());
	if (i == range.begin() || !utils::CharTraits<typename utils::RemoveConst<Char>::Type>::isAlpha(*i)) {
		throw Error("bad period format: %s", std::string(range.begin(), range.end()).c_str());
	}
	
	chunk.range(Range(range.begin(), i));
	chunk.letter(*(i++));
	
	return std::make_pair(chunk, Range(i, range.end()));
}

template <typename Range> void
evaluateRange(Range const &range, std::tm &when, int majorSign) {
	
	typedef typename Range::value_type Char;
	
	int sign = 1;
	Range copy = utils::trim(range);
	while (!copy.empty()) {
		if (SignTraits<Char>::isSign(copy[0])) {
			sign = SignTraits<Char>::sign(copy[0]);
			copy = utils::truncate(copy, 1, 0);
		}
		std::pair<Chunk<Range>, Range> p = nextChunk(copy);
		EvaluatorImpl<Range, typename EvaluatorList<Range>::Type>::evaluate(p.first, sign * majorSign, when);
		copy = utils::trimLeft(p.second);
	}
}

}}} // namespaces

#ifdef NEXTWEB_DEBUG
#include "nextweb/inlines/fastcgi/HttpDate.hpp"
#endif

