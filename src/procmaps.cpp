#include "./procmaps.hpp"

uint64_t ProcMap::hexToNumber (const std::string& str) {
	uint64_t acc = 0;

	for (size_t i = 0; isxdigit(static_cast<unsigned char>(str[i])); i++) {
		char c = str[i];
		acc *= 16;
		if (isdigit(c)) {
			acc += static_cast<uint64_t>(c - '0');
		} else if (isupper(c)) {
			acc += static_cast<uint64_t>(c - 'A' + 10);
		} else {
			acc += static_cast<uint64_t>(c - 'a' + 10);
		}
	}

	return acc;
}


ProcMap::MemMap::MemMap (std::string t_code) : code(t_code) {
	parse();
}

void ProcMap::MemMap::parse () {
    size_t position = 0;

    while (position < code.size()) {
        MemEntry entry;

        entry.address_start = parseAddress(position);

        if (code.at(position) != '-') {
            throw std::runtime_error("Invalid map, expected - after offset at position: " + std::to_string(position));
        }
        position++;

        entry.address_end = parseAddress(position);
        entry.address_size = entry.address_end - entry.address_start;

        position++; // skip space

        if (code.at(position) == 'r') {
            entry.perms.read = true;
        }
        position++;
        if (code.at(position) == 'w') {
            entry.perms.write = true;
        }
        position++;
        if (code.at(position) == 'x') {
            entry.perms.execute = true;
        }
        position++;
        if (code.at(position) == 'p') {
            entry.perms.priv = true;
        } else if (code.at(position) == 's') {
            entry.perms.shared = true;
        }
        position++;

        position++; // skip space

        entry.offset = parseOffset(position);

        position++; // skip space

        entry.dev = parseDev(position);

        position++; // skip space

        entry.inode = parseInode(position);

        consumeSpaces(position);

        entry.pathname = parsePathname(position);

        if (position < code.size()) {
            if (code.at(position) == '\n' || code.at(position) == '\r') {
                position++;
                if (position < code.size()) {
                    if (code.at(position) == '\n' || code.at(position) == '\r') {
                        position++;
                    }
                }
            }
        }

        entries.push_back(entry);

        if (position >= code.size()) break;
    }
}


void ProcMap::MemMap::consumeSpaces (size_t& position) {
    while (position < code.size() && code.at(position) == ' ') {
        position++;
    }
}

uint64_t ProcMap::MemMap::parseAddress (size_t& position) {
    std::string text = parseTextHex(position);
    return hexToNumber(text);
}

uint64_t ProcMap::MemMap::parseOffset (size_t& position) {
    std::string text = parseTextHex(position);
    return hexToNumber(text);
}

std::string ProcMap::MemMap::parseDev (size_t& position) {
    return parseText(position, false);
}

std::string ProcMap::MemMap::parseInode (size_t& position) {
    return parseText(position, false);
}

std::string ProcMap::MemMap::parsePathname (size_t& position) {
    return parseText(position, true);
}

std::string ProcMap::MemMap::parseTextHex (size_t& position) {
    std::string res;
    while (position < code.size()) {
        char c = code.at(position);
        if (c == '\n' || c == '\r' || c == '\0' || c == ' ' || c == '-') {
            break;
        }
        res += c;
        position++;
    }
    return res;
}

std::string ProcMap::MemMap::parseText (size_t& position, bool allow_spaces) {
    std::string res;

    while (position < code.size()) {
        char c = code.at(position);
        if (c == '\n' || c == '\r' || c == '\0' || (!allow_spaces && c == ' ')) {
            break;
        }
        res += c;
        position++;
    }

    return res;
}