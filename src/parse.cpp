/* ************************************************************************** */
/*                                                                            */
/*                         d8,                                                */
/*                        `8P                                                 */
/*                                                                            */
/*                        d88   .d888b, d8888b   88bd88b                      */
/*                        ?88   ?8b,   d8P' ?88  88P' ?8b                     */
/*                         88b    `?8b 88b  d88 d88   88P                     */
/*                         `88b`?888P' `?8888P'd88'   88b                     */
/*                          )88                                               */
/*                         ,88P                                               */
/*                      `?888P                                                */
/*                                                                            */
/*                   Kingmar  |  https://github.com/K1ngmar                   */
/*                                                                            */
/* ************************************************************************** */

#include <kmjson.hpp>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <exception>
#include <iostream>
#include <stack>

#define CASE_NBR '-': case '+': case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9'

namespace km {

//////////
// Json //
//////////

// Constructors

	Json::Json(const char* file_name)
	{
		struct stat data;
		int fd = open(file_name, O_RDONLY);

		if (fstat(fd, &data) < 0)
			throw std::runtime_error("file no workie");

		_len = data.st_size;
		_src = (char*)mmap(NULL, _len, PROT_READ, MAP_PRIVATE, fd, 0);
		if (_src == MAP_FAILED)
			throw std::runtime_error("Mapping file failed :/ unlucky");
		close(fd);
	}

	Json::~Json() noexcept(false)
	{
		if (munmap((void*)_src, _len) < 0)
			throw std::runtime_error("unmapping failed somehow lmao imagine that");
	}

// Parsing

	std::string_view Json::_parse_name(size_t& i)
	{
		_skip_ws(i);
		
		if (_src[i] != '\"')
			return std::string_view();
		size_t start_pos = i;
		++i;
		while (i < _len && _src[i] != '\"')
			++i;
		if (_src[i] != '\"')
			throw std::runtime_error("closing brace for string not found");
		return std::string_view(&_src[start_pos], i - start_pos + 1);
	}

	void Json::_skip_ws(size_t& i)
	{
		while (i < _len && isspace(_src[i]) == true)
			++i;
		if (i >= _len)
			throw std::runtime_error("Unexpected end of file");
	}

	void Json::_parse_dispatch(Json::Object& obj, size_t& i)
	{
		_skip_ws(i);
		switch(_src[i]) {
			case '{':
				return _parse_object(obj, ++i);
			// case CASE_NBR:
			// 	return _parse_nbr(obj, i);
			case '\"':
				return _parse_string(obj, i);
			default:
				throw std::runtime_error(std::string("Unsupported character found: `") + _src[i] + '`');
		}
	}

	void Json::_parse_object(Json::Object& obj, size_t& i)
	{
		obj.value_type = Json::Object::valuetype::Object;

		while (true) {
			std::string_view name = _parse_name(i);

			if (name.size() > 0) {
				_skip_ws(++i);

				if (_src[i] != ':') {
					throw std::runtime_error("attribute has no value");
				}

				auto& new_val = std::get<Object::obj_type>(obj.value);
				_parse_dispatch(new_val[name], ++i);
			}
			_skip_ws(i);
			
			switch(_src[i]) {
				case ',':
					++i;
					continue ;
				case '}':
					++i;
					return ;
				default:
					throw std::runtime_error(std::string("Uknown character found: `") + _src[i] + '`');
			}
		}
	}

	void Json::_parse_string(Json::Object& obj, size_t& i)
	{
		obj.value_type = Json::Object::valuetype::String;

		obj.value = _parse_name(i);
		++i;
	}

	// public
	Json::Object& Json::parse()
	{
		size_t i = 0;
		_parse_dispatch(_json_obj, i);
		return (_json_obj);
	}

///////////////
// Stringify //
///////////////

void Json::_stringify_object(const Json::Object& obj, size_t depth)
	{
		output.append("{\n");
		auto& values = std::get<Json::Object::obj_type>(obj.value);
		auto itr = values.begin();
		while (itr != values.end()) {
			// set depth padding
			output.insert(output.end(), depth, '\t');

			// object name
			output.append(itr->first);
			output.append(": ");

			// opbject
			_stringify(itr->second, depth);
			if (++itr == values.end())
				break ;
			output.append(",\n");
		}
		output.push_back('\n');
		output.insert(output.end(), depth - 1, '\t');
		output.append("}");
	}

	void Json::_stringify(const Json::Object& obj, size_t depth)
	{
		switch (obj.value_type) {
			case Json::Object::valuetype::Object: {
				_stringify_object(obj, depth + 1);
				break ;
			}
			case Json::Object::valuetype::String: {
				output.append(std::get<std::string_view>(obj.value));
				break ;
			}
			default:
				throw std::runtime_error("unknown object type");
		}
	}

	const std::string& Json::stringify(const Json::Object& obj)
	{
		output.reserve(_len);

		_stringify(obj, 0);

		return (output);
	}

} /* end of namespace */
