#include "tools.hpp"

int main()
{
	int mark = 4;
	unsigned test;
	unsigned num = 0x000002d6;
	cout << "original: " << num << endl;
	for (int bit = 0; bit <=9; bit++) {
	if (num & (1 << bit)) {
	cout << bit;
}
else {
	cout << 0;

}
}
cout << endl;

	num = num&~(1<<mark);
	cout << "new: " << num << endl;
	
for (int bit = 0; bit <=9; bit++) {
	if (num & (1 << bit)) {
	cout << bit;
}
else {
	cout << 0;

}}
cout << endl;

cout << "0x" << hex << num << dec << "" << endl;

}
