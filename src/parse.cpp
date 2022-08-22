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
#include <exception>
#include <iostream>

namespace km {

//////////
// Json //
//////////

	Json::Json(const char* file_name)
	{
		struct stat data;
		int fd = open(file_name, O_RDONLY);

		if (fstat(fd, &data) < 0)
			throw std::runtime_error("file no workie");

		len = data.st_size;
		src = (char*)mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
		if (src == MAP_FAILED)
			throw std::runtime_error("Mapping file failed :/ unlucky");
	}

	Json::~Json() noexcept(false)
	{
		if (munmap(src, len) < 0)
			throw std::runtime_error("unmapping failed somehow lmao imagine that");
	}

} /* end of namespace */
