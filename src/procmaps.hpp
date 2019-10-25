#ifndef FILE_SEEN_PARSEPROCMAPS
#define FILE_SEEN_PARSEPROCMAPS

#include <string>
#include <cstdint>
#include <vector>
#include <stdexcept>

namespace ProcMap {

uint64_t hexToNumber (const std::string& str);

struct Permissions {
	bool read = false;
	bool write = false;
	bool execute = false;
	bool priv = false;
	bool shared = false;
};

struct MemEntry {
	uint64_t address_start;
	uint64_t address_end;
	uint64_t address_size;

	Permissions perms;
	uint64_t offset;

    // TODO: it would be nice to parse this
	std::string dev;

    // TODO: it would be nice to parse this
	std::string inode;

	std::string pathname;

	MemEntry () {}
};
class MemMap {
	public:
	std::string code;
	std::vector<MemEntry> entries;

	explicit MemMap (std::string t_code);

	void parse ();

	void consumeSpaces (size_t& position);

	uint64_t parseAddress (size_t& position);

	uint64_t parseOffset (size_t& position);

	std::string parseDev (size_t& position);

	std::string parseInode (size_t& position);

	std::string parsePathname (size_t& position);

	std::string parseTextHex (size_t& position);

	std::string parseText (size_t& position, bool allow_spaces=false);
};


};


#endif