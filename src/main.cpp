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

int main(int argc, char** argv)
{
	if (argc < 2)
		km::error_exit("No files selected");

	// parse files
	for (int i = 1; i < argc; ++i) {
		try {
			km::Json json(argv[i]);
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}
}
