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

///////////
// Types //
///////////

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

	//////////////////
	// constructors //
	//////////////////
	public:

		Object() = default;
		Object(const char* &src);

		Object(const Object& x);
		Object& operator = (const Object& x);

	};

//////////
// Json //
//////////
	
	class Json {

	//////////////////////
	// Member variables //
	//////////////////////
	private: 

		const char* src;
		off_t		len;
		Object		json_obj;
		// std::string output;

	////////////////////
	// Roadwork ahead //
	////////////////////
	private:

		Json();

	public:

		Json(const char* file);
		~Json() noexcept(false);

	///////////////////
	// Funky members //
	///////////////////
	public:

		const Object& parse();

	};

} /* end of km namespace */