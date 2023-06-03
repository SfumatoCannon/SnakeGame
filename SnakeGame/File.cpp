#include <cstdio>
#include <algorithm>
#include "File.h"

long long Master[10];
long long totnum;

void ReadScore()
{
	freopen("score.txt", "r", stdin);
	int i;
	scanf("%lld", &totnum);
	totnum = totnum / 58167;
	for (i = 0; i < totnum; i++)
	{
		scanf("%lld", &Master[i]);
		Master[i] = (Master[i] - 49285) / 36137;
		Master[i] *= 10;
	}
	fclose(stdin);
}
void WriteScoreToFile()
{
	freopen("score.txt", "w", stdout);
	int i;
	printf("%lld ", totnum * 58167);
	for (i = 0; i < totnum; i++)
		printf("%lld ", (Master[i] / 10) * 36137 + 49285);
	fclose(stdout);
}
void UpdateScore(int newscore)
{
	if (newscore == 0)
		return;
	long long realscore = (long long)newscore;
	int i;
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