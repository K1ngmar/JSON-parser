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

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>

namespace km {

///////////
// Error //
///////////

	void error_exit(const std::string& error, int error_code = 1);

//////////
// Json //
//////////
	
	class Json {

	////////////
	// Object //
	////////////
	public:
		
		struct Object {

		//////////////
		// Typedefs //
		//////////////
		public:

			typedef std::unordered_map<std::string_view, Object>				obj_type;
			typedef std::vector<Object>											arr_type;
			typedef std::string_view											str_type;
			typedef std::variant<obj_type, arr_type, int32_t, str_type, bool>	val;

		///////////
		// types //
		///////////
		public:

			enum class valuetype {
				Object,
				Array,
				Number,
				String,
				Bool,
				Null
			};

		//////////////////////
		// member variables //
		//////////////////////
		public:

			enum valuetype	value_type;
			val				value;

		}; /* end of object */

	//////////////////////
	// Member variables //
	//////////////////////
	private: 

		const char* _src;
		size_t		_len;
		Object		_json_obj;
		std::string output;

	////////////////////
	// Roadwork ahead //
	////////////////////
	private:

		Json();

	public:

		Json(const char* file);
		~Json() noexcept(false);

	/////////////
	// Parsing //
	/////////////
	private:

		std::string_view	_parse_name(size_t& i);
		void				_skip_ws(size_t& i);
		void				_parse_dispatch(Object& obj, size_t& i);
		void				_parse_object(Object& obj, size_t& i);
		void				_parse_number(Object& obj, size_t& i);
		void				_parse_array(Object& obj, size_t& i);
		void				_parse_string(Object& obj, size_t& i);

	public:

		Object& parse();

	///////////////
	// Stringify //
	///////////////
	private:

		void _stringify_object(const Object& obj, size_t depth);
		void _stringify_array(const Object& obj, size_t depth);
		void _stringify(const Object& obj, size_t depth);

	public:

		const std::string&	stringify(const Object& obj);

	};

} /* end of km namespace */
