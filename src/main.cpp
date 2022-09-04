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
#include <iostream>

#ifdef LEAKS
	# define cout cerr
#endif

int main(int argc, char** argv)
{
	if (argc != 2)
		km::error_exit("Check your arguments");

	// parse files
	try {
		km::Json json(argv[1]);
		km::Json::Object& json_obj = json.parse();
		std::cout << json.stringify(json_obj) << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
		return (69);
	}

	// check for leaks
	#ifdef LEAKS
		std::cout << std::endl;
		std::string name(argv[0]);
		name.erase(0, name.rfind("/") + 1);
		std::string leaks(std::string("leaks ") + name + " | grep 'leaks for'");
		system(leaks.c_str());
	#endif
}
