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

// Funky Members
	
	const Json::Object& Json::parse()
	{
		std::stack<Json::Object*> stack;
		_json_obj = Json::Object();

		for (off_t i = 0; i < _len; ++i) {

		}

		return (_json_obj);
	}

} /* end of namespace */
