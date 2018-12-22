#include "coder.h"
using namespace std;

frlog log;
UCR buffer1[MAX_BLOCK_SIZE + 1], buffer2[MAX_BLOCK_SIZE + 1];
UCR iv[] = "abcdefghijklmnopqrstuvwxyz", buffer3[64];
UCR *cipher = buffer1, *plaint = buffer2, *key = buffer3;
coder ecoder, hcoder;


int main(int argc, char **argv) {
	fstream fin;
	string srcfile, frlogfile, outfile;
	if (argc < 3)help(2);
	srcfile = argv[1];
	frlogfile = argv[2];
	if (!deserializa(frlogfile, log)) help(2);
	ecoder.encryptinit(iv);
	hcoder.hashinit();

	fin.open(srcfile, ios::binary | ios::out);
	vector<ULL>::iterator p = log.ChunkOffset.begin();
	LL last = 0;
	while (p != log.ChunkOffset.end()) {
		fin.read((char *) plaint, sizeof(char) * (*p - last));
		hcoder.calhash(plaint, *p - last, key);
		log.ChunkSize.push_back(ecoder.encrypt(plaint, *p - last, cipher, key));//密文长度为什么还要记录？和原文一样长啊？
		fin.write((char *) cipher, sizeof(char) * (*p - last));
		last = *p;
		p++;
	}
	fin.close();
	return 0;
}
