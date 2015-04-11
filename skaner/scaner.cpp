#include "stdafx.h"

#include "grammar.h"
#include "error.h"

typedef int val;
typedef size_t refId;
typedef std::tuple<TYPES, val> entry;
typedef std::unordered_map<std::string, refId> refMap;

refMap symbolRefs;
std::vector<entry>  symbolArray;
Error e;

void scan(std::streambuf &input);
refId insert(const std::string &key, const WholeNumber &val);
//refId insert(const std::string &key, const std::string &val);

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		std::cout << "U¿ycie: " << argv[0] << " redCodeFile";
		system("pause>>null");
		return 1;
	}
	std::cout << sizeof std::string << std::endl;
	std::cout << sizeof std::tuple<ID, refId> << std::endl << sizeof (unsigned int) << std::endl << sizeof WholeNumber;

	std::filebuf buf;
	buf.open(argv[1], std::ios::in);
	e.file = argv[1];

	scan(buf);

	auto start = std::chrono::high_resolution_clock::now();
	while(buf.sbumpc() != EOF);
	auto test1 = std::chrono::high_resolution_clock::now() - start;
	system("pause>>null");
}

#define setConst(ID) result[i] = std::make_tuple(ID, -1); break;

void scan(std::streambuf &input)
{
	const int resultSize = 8096;
	std::tuple<ID, refId> result[resultSize];
	
	unsigned int line = 0, column = 0;
	std::streambuf::int_type a;
	unsigned int i = 0;
	std::string key;
	key.reserve(20);
	while((a = input.sbumpc()) != EOF && i < resultSize)
	{
		++column;
		switch(a)
		{
		case '+': setConst(ID::PLUS);
		case '-': setConst(ID::MINUS);
		case '*': setConst(ID::MUL);
		case '/': setConst(ID::DIV);
		case '%': setConst(ID::MOD);
		default:
			if(isdigit(a))
			{
				int number = 0;
				char digit;
				do
				{
					key += a;
					
					digit = '0' - a;
					if(number <= std::numeric_limits<int>::max() - digit)
						number = 10 * number + digit;
					else
						number = -1;

				} while((a = input.sbumpc()) != EOF && isdigit(a));
				input.pubseekoff(-1, std::ios_base::cur);
				if(number > 0)
					/*result[i] = std::move(std::make_tuple(ID::REF, insert(key, number)))*/;
				else
				{
					auto &errorMsg = e.mkError(ERROR::CONSTANT_TOO_BIG, key, line, column);
					//result[i] = std::move(std::make_tuple(ID::ERROR, insert(key, errorMsg)));
				}
			}
			
		}
		++i;
	}
}

refId insert(const std::string &key, WholeNumber &number)
{
	auto id = symbolRefs[key];

	if(id) return id;

	symbolArray.push_back(std::move(entry(TYPES::NUMBER, number)));
	return id = symbolArray.size() - 1;
}