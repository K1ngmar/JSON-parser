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

		typedef std::unordered_map<std::string_view, Object>	map_type;

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

		union val {
			map_type			obj;
			std::vector<Object>	arr;
			int32_t				nbr;
			std::string_view	str;
			bool				bln;
		};

	//////////////////////
	// member variables //
	//////////////////////
	public:

		enum valuetype value;

	};

//////////
// Json //
//////////
	
	class Json {

	//////////////////////
	// Member variables //
	//////////////////////
	private: 

		char*		src;
		off_t		len;
		// std::string output;
		// Object		json_obj;

	////////////////////
	// Roadwork ahead //
	////////////////////
	public:

		Json(const char* file);
		~Json() noexcept(false);

	};

} /* end of km namespace */
