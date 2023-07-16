#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <ctype.h>

#define MAP_X 12
#define MAP_Y 26

typedef struct LOCATE
{
	int x;
	int y;
} Locate;

enum map_mark
{
	void_mark = 0, bomb_mark = -1, opened_mark = 1
};

int isValidLocate(Locate loc)
{
	return 0 <= loc.x && loc.x < MAP_X && 0 <= loc.y && loc.y < MAP_Y;
}

Locate get_locate()
{
	int x, y;
	while (1)
	{
		printf("ÁÂÇ¥ÀÔ·Â(ex: AE) : ");
		x = toupper(getchar());
		y = toupper(getchar());
		while (getchar() != '\n');

		if ('A' <= x && x <= 'Z' && 'A' <= y && y <= 'Z')
			break;
	}

	return (Locate) { x - 'A', y - 'A'};
}

void print_map(const int arr[][MAP_Y], int size)
{
	const char* void_char = ".";
	const char* bomb_char = "X";
	Locate i;

	// »ó´Ü ÁÂÇ¥ Ãâ·Â
	printf("   ");
	for (i.y = 0; i.y < MAP_Y; i.y++)
		printf(" %c", i.y + 'A');
	printf("\n___");
	for (i.y = 0; i.y < MAP_Y; i.y++)
		printf("__");
	printf("\n");

	// ÁÂÃø ÁÂÇ¥¿Í ¸Ê Ãâ·Â
	for (i.x = 0; i.x < MAP_X; i.x++, printf("\n"))
		for (i.y = -1; i.y < MAP_Y; i.y++)
			//ÁÂÃø ÁÂÇ¥ Ãâ·Â
			if (i.y == -1)
				printf("%c |", 'A' + i.x);
			//¸Ê Ãâ·Â
			else
			{
				if (arr[i.x][i.y] == bomb_mark)
					printf(" %s", bomb_char);
				else if (arr[i.x][i.y] == void_mark)
					printf(" %s", void_char);
				else
					printf(" %d", arr[i.x][i.y]);
			}
}

void show_map_for_user(const int ans[][MAP_Y], const int open[][MAP_Y], int size)
{
	const char* void_char = ".";
	const char* bomb_char = "X";
	const char* unknown_char = "v";
	Locate i;

	// »ó´Ü ÁÂÇ¥ Ãâ·Â
	printf("   ");
	for (i.y = 0; i.y < MAP_Y; i.y++)
		printf(" %c", i.y + 'A');
	printf("\n___");
	for (i.y = 0; i.y < MAP_Y; i.y++)
		printf("__");
	printf("\n");

	// ÁÂÃø ÁÂÇ¥¿Í ¸Ê Ãâ·Â
	for (i.x = 0; i.x < MAP_X; i.x++, printf("\n"))
		for (i.y = -1; i.y < MAP_Y; i.y++)
			//ÁÂÃø ÁÂÇ¥ Ãâ·Â
			if (i.y == -1)
				printf("%c |", 'A' + i.x);
	//¸Ê Ãâ·Â
			else if (open[i.x][i.y])
			{
				if (ans[i.x][i.y] == bomb_mark)
					printf(" %s", bomb_char);
				else if (ans[i.x][i.y] == void_mark)
					printf(" %s", void_char);
				else
					printf(" %d", ans[i.x][i.y]);
			}
			else
				printf(" %s", unknown_char);
}

void bomb_roundplus(int arr[][MAP_Y], Locate loc)
{
	Locate i;
	for (i.x = loc.x - 1; i.x <= loc.x + 1; i.x++)
		for (i.y = loc.y - 1; i.y <= loc.y + 1; i.y++)
			if (isValidLocate(i))
				if (arr[i.x][i.y] != bomb_mark)
					arr[i.x][i.y] += 1;
}

//¸Ê ¿ÀÇÂÀÌ ¼º°øÇÏ¸é 1, ÆøÅºÀ» ¿­¸é 0 ¹ÝÈ¯
int map_open(int ans[][MAP_Y], int open[][MAP_Y], Locate loc)
{
	if (isValidLocate(loc) && open[loc.x][loc.y] != opened_mark)
	{
		open[loc.x][loc.y] = opened_mark;
		int mark = ans[loc.x][loc.y];
		if (mark == void_mark) {
			Locate new_loc;
			for (new_loc.x = loc.x - 1; new_loc.x <= loc.x + 1; new_loc.x++)
				for (new_loc.y = loc.y - 1; new_loc.y <= loc.y + 1; new_loc.y++)
					map_open(ans, open, new_loc);
		}
		else if (mark == bomb_mark)
			return 0;
	}
	return 1;
}

int main(void)
{
	system("mode con lines=52 cols=60");

	int bomb_number;
	printf("ÆøÅº°³¼ö ÀÔ·Â: ");
	scanf("%d", &bomb_number);
	getchar();

	int ans[MAP_X][MAP_Y] = { void_mark };
	int open[MAP_X][MAP_Y] = { void_mark };
	//srand((unsigned)time(NULL));

	//Áö·Ú Èð»Ñ¸®°í Áö·Ú ÁÖº¯ ¼ýÀÚ·Î Ç¥½ÃÇÏ±â
	for (int i = 0; i < bomb_number; i++)
	{

		Locate loc = { rand() % MAP_X, rand() % MAP_Y };
		if (ans[loc.x][loc.y] == bomb_mark)
			i--;
		else
		{
			ans[loc.x][loc.y] = bomb_mark;
			bomb_roundplus(ans, loc);
		}
	}

	//¹«ÇÑ ¿­±â
	while (1)
	{
		system("cls");
		/*
		printf("ans:\n");
		print_map(ans, MAP_X);

		printf("\nopen:\n");
		print_map(open, MAP_X);

		printf("\nfor_user:\n");
		*/
		show_map_for_user(ans, open, MAP_X);

		Locate loc = get_locate();
		int result = map_open(ans, open, loc);
		if (!result)
			break;
	}

	return 0;
}