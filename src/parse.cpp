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
		while (i < _len && _src[i] != '"') {
			++i;
		}
		if (_src[i] != '\"')
			throw std::runtime_error("closing quote for string not found");
		return std::string_view(&_src[start_pos], i - start_pos + 1);
	}

	void Json::_skip_ws(size_t& i)
	{
		while (i < _len && isspace(_src[i]) == true)
			++i;
		if (i >= _len)
			throw std::runtime_error("Unexpected end of file");
	}

	void Json::_parse_dispatch(Json::Object* obj, size_t& i)
	{
		_skip_ws(i);
		switch(_src[i]) {
			case '{':
				return _parse_object(obj, ++i);
			case '[':
				return _parse_array(obj, ++i);
			case CASE_NBR:
				return _parse_number(obj, i);
			case '\"':
				return _parse_string(obj, i);
			// true || false
			case 't':
			case 'f':
				return _parse_bool(obj, i);
			// null
			case 'n':
				return _parse_null(obj, i);
			default:
				throw std::runtime_error(std::string("Unsupported character found: `") + _src[i] + '`');
		}
	}

	void Json::_parse_object(Json::Object* obj, size_t& i)
	{
		obj->value_type = Json::Object::valuetype::Object;

		while (true) {
			std::string_view name = _parse_name(i);

			if (name.size() > 0) {
				_skip_ws(++i);

				if (_src[i] != ':') {
					throw std::runtime_error("attribute has no value");
				}

				auto& new_val = std::get<Object::obj_type>(obj->value);
				Json::Object* new_obj = new Object(); 
				new_val[name] = new_obj;

				_parse_dispatch(new_obj, ++i);
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
					throw std::runtime_error(std::string("Unknown character found: `") + _src[i] + '`');
			}
		}
	}

	void Json::_parse_array(Json::Object* obj, size_t& i)
	{
		obj->value_type = Json::Object::valuetype::Array;

		obj->value = Object::arr_type();
		Object::arr_type& array = std::get<Object::arr_type>(obj->value);

		_skip_ws(i);
		while (i < _len && _src[i] != ']') {
			array.push_back(new Object());
			_parse_dispatch(array.back(), i);
			_skip_ws(i);
			if (_src[i] != ',')
				break ;
			++i;
		}
		if (_src[i] != ']')
			throw std::runtime_error("missing closing `]`");
		++i;
	}

	void Json::_parse_number(Json::Object* obj, size_t& i)
    {
        obj->value_type = Json::Object::valuetype::Number;
        
        int64_t res = 0;
        int64_t sgn = 1;

        if (_src[i] == '-')
            sgn = -1;
        if (i < _len && (_src[i] == '+' || _src[i] == '-'))
            ++i;
        while (i < _len && _src[i] >= '0' && _src[i] <= '9') {
            res = res * 10 + _src[i] - '0';
            ++i;
        }
        obj->value = int32_t(sgn * res);
    }

	void Json::_parse_string(Json::Object* obj, size_t& i)
	{
		obj->value_type = Json::Object::valuetype::String;

		obj->value = _parse_name(i);
		++i;
	}
	void Json::_parse_bool(Json::Object* obj, size_t& i)
	{
		obj->value_type = Json::Object::valuetype::Bool;

		// 5 == len of false
		if (i + 5 >= _len)
			throw std::runtime_error("Unexpected end of file");

		if (_src[i] == 't' && strncmp(_src + i, "true", 4) == 0) {
			obj->value = true;
			i += 4;
		} else if (_src[i] == 'f' && strncmp(_src + i, "false", 5) == 0) {
			obj->value = false;
			i += 5;
		} else {
			throw std::runtime_error("Invalid bool value");
		}
	}

	void Json::_parse_null(Json::Object* obj, size_t& i)
	{
		obj->value_type = Json::Object::valuetype::Null;
		
		// 4 = len of null
		if (i + 4 >= _len)
			throw std::runtime_error("Unexpected end of file");
		
		if (strncmp(_src + i, "null", 4) != 0)
			throw std::runtime_error("you probably made a typo in `null`");

		i += 4;
	}

	// public
	Json::Object& Json::parse()
	{
		size_t i = 0;
		_parse_dispatch(&_json_obj, i);
		return (_json_obj);
	}

///////////////
// Stringify //
///////////////

	void Json::_stringify_object(const Json::Object* obj, size_t depth)
	{
		_output.append("{\n");
		auto& values = std::get<Json::Object::obj_type>(obj->value);
		auto itr = values.begin();
		while (itr != values.end()) {
			// set depth padding
			_output.insert(_output.end(), depth, '\t');

			// object name
			_output.append(itr->first);
			_output.append(": ");

			// opbject
			_stringify(itr->second, depth);
			if (++itr == values.end()) {
				_output.push_back('\n');
				break ;
			}
			_output.append(",\n");
		}
		_output.insert(_output.end(), depth - 1, '\t');
		_output.append("}");
	}

	void Json::_stringify_array(const Json::Object* obj, size_t depth)
	{
		_output.append("[\n");
		const Object::arr_type& arr = std::get<Json::Object::arr_type>(obj->value);

		for (size_t i = 0; i < arr.size(); ++i) {
			// set padding
			_output.insert(_output.end(), depth, '\t');

			_stringify(arr[i], depth);

			if (i + 1 < arr.size())
				_output.push_back(',');
			_output.push_back('\n');
		}
		_output.insert(_output.end(), depth - 1, '\t');
		_output.append("]");
	}

	void Json::_stringify_bool(const Json::Object* obj)
	{
		if (std::get<bool>(obj->value) == true)
			_output.append("true");
		else
			_output.append("false");
	}

	void Json::_stringify(const Json::Object* obj, size_t depth)
	{
		switch (obj->value_type) {
			case Json::Object::valuetype::Object: {
				_stringify_object(obj, depth + 1);
				break ;
			}
			case Json::Object::valuetype::Array: {
				_stringify_array(obj, depth + 1);
				break ;
			}
			case Json::Object::valuetype::Number: {
				_output.append(std::to_string(std::get<int32_t>(obj->value)));
				break ;
			}
			case Json::Object::valuetype::String: {
				_output.append(std::get<std::string_view>(obj->value));
				break ;
			}
			case Json::Object::valuetype::Bool: {
				_stringify_bool(obj);
				break ;
			}
			case Json::Object::valuetype::Null: {
				_output.append("null");
				break ;
			}
			default:
				throw std::runtime_error("unknown object type");
		}
	}

	const std::string& Json::stringify(const Json::Object& obj)
	{
		_output.reserve(_len);

		_stringify(&obj, 0);

		return (_output);
	}

} /* end of namespace */
