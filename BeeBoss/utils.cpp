#include "utils.h"
#include <sstream>

std::wstring replaceAll(std::wstring el, std::wstring key, std::wstring str) {
	std::wstring::size_type n = 0;
	while ((n = str.find(el, n)) != std::string::npos)
	{
		str.replace(n, el.size(), key);
		n += key.size();
	}
	return str;
}

std::wstring widen(const std::string& str)
{
	std::wostringstream wstm;
	const std::ctype<wchar_t>& ctfacet =
		std::use_facet< std::ctype<wchar_t> >(wstm.getloc());
	for (size_t i = 0; i < str.size(); ++i)
		wstm << ctfacet.widen(str[i]);
	return wstm.str();
}

std::string narrow(const std::wstring& str)
{
	std::ostringstream stm;
	const std::ctype<char>& ctfacet =
		std::use_facet< std::ctype<char> >(stm.getloc());
	for (size_t i = 0; i < str.size(); ++i)
		stm << ctfacet.narrow(str[i], 0);
	return stm.str();
}

std::vector<std::wstring> getDroppedFiles() {
	std::vector< std::wstring > files;

	for (int ch = _getwch(); ch != '\r'; ch = _getwch()) {
		std::wstring file_name;
		if (ch == '\"') {
			while ((ch = _getwch()) != '\"')
				file_name += ch;
		}
		else {
			file_name += ch;
			while (_kbhit())
				file_name += _getwch();
		}
		std::wcout << file_name << std::endl;
		if (!std::experimental::filesystem::is_directory(file_name))
			files.push_back(file_name);
		else
			for (std::experimental::filesystem::recursive_directory_iterator i(file_name), end; i != end; ++i)
				if (!std::experimental::filesystem::is_directory(i->path()))
					files.push_back(replaceAll(L"/", L"\\", i->path().generic_wstring()));
	}
	return files;
}

char encryptChar(char c, int key) {
	return c ^ key;

}