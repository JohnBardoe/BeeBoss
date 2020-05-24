#include "utils.h"


#define ENCRYPTION_KEY 4




int main() {
	setlocale(LC_ALL, "rus");


	std::vector<std::wstring> droppedFiles = getDroppedFiles();

	for (auto i : droppedFiles)
		std::wcout << i << std::endl;



	return 0;
}