#include "File.h"

long long Master[10];
long long totnum;

void ReadScore()
{
	std::ifstream ifs;
	ifs.open("score", std::ios::binary);
	if (!ifs.is_open())
		return;
	ifs.read(reinterpret_cast<char*>(&totnum), sizeof(totnum));
	for (int i = 0; i < totnum; i++)
	{
		ifs.read(reinterpret_cast<char*>(&Master[i]), sizeof(Master[i]));
	}
	ifs.close();
}
void WriteScoreToFile()
{
	std::ofstream ofs;
	ofs.open("score", std::ios::binary);
	if (!ofs.is_open())
	{
		std::ofstream ofsForCreating("score");
		ofsForCreating.close();
		ofs.open("score", std::ios::binary);
	}
	ofs.write(reinterpret_cast<char*>(&totnum), sizeof(totnum));
	for (int i = 0; i < totnum; i++)
	{
		ofs.write(reinterpret_cast<char*>(&Master[i]), sizeof(Master[i]));
	}
	ofs.close();
}
void UpdateScore(int newscore)
{
	if (newscore == 0)
		return;
	long long realscore = (long long)newscore;
	if (totnum < 10)
	{
		totnum++;
		Master[totnum - 1] = realscore;
		std::sort(Master, Master + totnum, [](long long a, long long b) {return a > b; });
	}
	else
	{
		std::sort(Master, Master + totnum);
		if (realscore > Master[0])
			Master[0] = realscore;
		std::sort(Master, Master + totnum, [](long long a, long long b) {return a > b; });
	}
	WriteScoreToFile();
}