#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "key.h"

#define mapY 30
#define mapX 15

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int isDead = 0;

typedef struct Map
{
    int map[mapY][mapX];
    int score;
    int blockType;
    int rot;
} Map;

void GameOver(Map *map)
{
    sleep(1);
    system("clear");
    printf("\n\n\n\n\n\t\t- G A M E O V E R -\n");
    printf("\t\tS C O R E : %d\n", map->score);
}

int PlayerMove()
{
    int getKey = key();

    if (getKey == KEY_RIGHT)
        return 1;
    else if (getKey == KEY_LEFT)
        return -1;
    else if (getKey == KEY_UP)
        return 2;
    else if (getKey == KEY_SPACEBAR)
        return 3;
    else
        return 0;
} // end PlayerMove

void ClearMap(Map *map)
{
    for (int i = 0; i < mapY; i++)
    {
        for (int j = 0; j < mapX; j++)
        {
            if (j == 0 || j == mapX - 1)
                map->map[i][j] = 1;
            else if (i == mapY - 1)
                map->map[i][j] = 1;
            else
                map->map[i][j] = 0;
        }
    }
} // end ClearMap

void CreateBlock(Map *map)
{
    srand((unsigned)time(NULL));

    int tile_model[7][4][4] = {
        {{0, 0, 3, 0}, {0, 0, 3, 0}, {0, 0, 3, 0}, {0, 0, 3, 0}},
        {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 4, 4, 0}, {0, 4, 4, 0}},
        {{0, 0, 0, 0}, {0, 0, 0, 0}, {5, 5, 0, 0}, {0, 5, 5, 0}},
        {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 6, 6}, {0, 6, 6, 0}},
        {{0, 0, 0, 0}, {0, 0, 7, 0}, {0, 0, 7, 0}, {0, 7, 7, 0}},
        {{0, 0, 0, 0}, {0, 8, 0, 0}, {0, 8, 0, 0}, {0, 8, 8, 0}},
        {{0, 0, 0, 0}, {0, 0, 0, 0}, {9, 9, 9, 0}, {0, 9, 0, 0}}};

    int ppx = 7, ppy = 0; //초기좌표
    int i, j;

    int minoType = rand() % 7;
    map->blockType = minoType;
    map->rot = 0;

    // minoType = 1;
    for (i = ppy; i < 4 + ppy; i++)
    {
        for (j = ppx; j < 4 + ppx; j++)
        {
            map->map[i][j] = tile_model[minoType][i - ppy][j - ppx];
        }
    }
} // end CreateBlock

void IceBlock(Map *map)
{
    for (int i = mapY - 1; i >= 0; i--)
    {
        for (int j = mapX - 1; j >= 0; j--)
        {
            if (map->map[i][j] != 0 && map->map[i][j] != 1)
            {
                map->map[i][j] = 2;
            }
        }
    }
    map->score += 100;
    sleep(1);
    CreateBlock(map);
} // end IceBlock

void RotBlock(Map *map)
{
    int tile_model_rotation[7][4][4][4] = {
        {{0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0}, {0, 3, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0}, {0, 3, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0}},

        {{0, 0, 0, 0, 0, 4, 4, 0, 0, 4, 4, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 4, 4, 0, 0, 4, 4, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 4, 4, 0, 0, 4, 4, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 4, 4, 0, 0, 4, 4, 0, 0, 0, 0, 0}},

        {{0, 0, 0, 0, 5, 5, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 5, 0, 0, 5, 5, 0, 0, 5, 0, 0}, {0, 0, 0, 0, 5, 5, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 5, 0, 0, 5, 5, 0, 0, 5, 0, 0}},

        {{0, 0, 0, 0, 0, 6, 6, 0, 6, 6, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 6, 0, 0, 0, 6, 6, 0, 0, 0, 6, 0, 0}, {0, 0, 0, 0, 0, 6, 6, 0, 6, 6, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 6, 0, 0, 0, 6, 6, 0, 0, 0, 6, 0, 0}},

        {{0, 0, 0, 0, 7, 0, 0, 0, 7, 7, 7, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 7, 7, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 0, 0, 0, 7, 0}, {0, 0, 0, 0, 0, 7, 7, 0, 0, 7, 0, 0, 0, 7, 0, 0}},

        {{0, 0, 0, 0, 0, 0, 8, 0, 8, 8, 8, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 8, 8, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 0, 8, 0, 0, 0}, {0, 0, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 8, 8, 0}},

        {{0, 0, 0, 0, 0, 9, 0, 0, 9, 9, 9, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 9, 0, 0, 0, 9, 9, 0, 0, 9, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9, 0, 0, 9, 0, 0}, {0, 0, 0, 0, 0, 9, 0, 0, 9, 9, 0, 0, 0, 9, 0, 0}}};
    int ppy = 0, ppx = 0;
    int exit = 0;

    map->rot = (map->rot + 5) % 4;

    for (int i = 0; i < mapY; i++)
    {
        for (int j = 0; i < mapX; j++)
        {
            if (map->map[i][j] != 0 && map->map[i][j] != 1 && map->map[i][j] != 2)
            {
                ppy = i, ppx = j;
                exit = 1;
                break;
            }
        }
        if (exit != 0)
            break;
    }

    // if(map->map[ppy][ppx + 4] == 1 || map->map[ppy][ppx + 4] == 2)
    //     return;

    for (int i = mapY - 1; i >= 0; i--)
    {
        for (int j = mapX - 1; j >= 0; j--)
        {
            if (map->map[i][j] != 0 && map->map[i][j] != 1 && map->map[i][j] != 2)
            {
                map->map[i][j] = 0; // 기존 블럭 삭제
            }
        }
    }
    for (int i = ppy; i < 4 + ppy; i++)
    {
        for (int j = ppx; j < 4 + ppx; j++)
        {
            if (tile_model_rotation[map->blockType][map->rot][i - ppy][j - ppx] != 0)
                map->map[i][j] = tile_model_rotation[map->blockType][map->rot][i - ppy][j - ppx]; //회전된 블럭 넣기
        }
    }
} // end RotBlock

void *FallBlock(void *param)
{
    Map *map = (Map *)param;

    while (isDead == 0)
    {
        pthread_mutex_lock(&mutex);

        for (int i = mapY - 1; i >= 0; i--)
        {
            for (int j = mapX - 1; j >= 0; j--)
            {
                if (map->map[i][j] != 0 && map->map[i][j] != 1 && map->map[i][j] != 2)
                {
                    if (map->map[i + 1][j] == 1 || map->map[i + 1][j] == 2)
                    {
                        IceBlock(map);
                        j = -1, i = -1;
                    }
                    map->map[i + 1][j] = map->map[i][j];
                    map->map[i][j] = 0;
                }
            }
        }

        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
} // end FallBlock

void *MoveBlock(void *param)
{
    Map *map = (Map *)param;
    int insert = 0;
    int exit = 0;

    while (isDead == 0)
    {
        exit = 0;
    INSERT:
        insert = PlayerMove();

        if (insert == 1)
        {
            for (int i = mapY - 1; i >= 0; i--)
            {
                for (int j = mapX - 1; j >= 0; j--)
                {
                    if (map->map[i][j] != 0 && map->map[i][j] != 1 && map->map[i][j] != 2)
                    {
                        if (map->map[i][j + insert] == 1 || map->map[i][j + insert] == 2)
                            goto INSERT;
                        pthread_mutex_lock(&mutex);
                        map->map[i][j + insert] = map->map[i][j];
                        map->map[i][j] = 0;
                        pthread_mutex_unlock(&mutex);
                    }
                }
            }
        }
        else if (insert == -1)
        {
            for (int i = 0; i < mapY; i++)
            {
                for (int j = 0; j < mapX; j++)
                {
                    if (map->map[i][j] != 0 && map->map[i][j] != 1 && map->map[i][j] != 2)
                    {
                        if (map->map[i][j + insert] == 1 || map->map[i][j + insert] == 2)
                            goto INSERT;
                        pthread_mutex_lock(&mutex);
                        map->map[i][j + insert] = map->map[i][j];
                        map->map[i][j] = 0;
                        pthread_mutex_unlock(&mutex);
                    }
                }
            }
        }
        else if (insert == 2)
        {
            pthread_mutex_lock(&mutex);
            RotBlock(map);
            pthread_mutex_unlock(&mutex);
        }
        else if (insert == 3)
        {
            while (exit == 0)
            {
                for (int i = mapY - 1; i >= 0; i--)
                {
                    for (int j = mapX - 1; j >= 0; j--)
                    {
                        if (map->map[i][j] != 0 && map->map[i][j] != 1 && map->map[i][j] != 2)
                        {
                            if (map->map[i + 1][j] == 1 || map->map[i + 1][j] == 2)
                            {
                                pthread_mutex_lock(&mutex);
                                IceBlock(map);
                                pthread_mutex_unlock(&mutex);
                                exit = 1;
                                break;
                            }
                            pthread_mutex_lock(&mutex);
                            map->map[i + 1][j] = map->map[i][j];
                            map->map[i][j] = 0;
                            pthread_mutex_unlock(&mutex);
                        }
                    }
                    if (exit != 0)
                        break;
                }
            }
        }
        else
        {
            for (int i = mapY - 1; i >= 0; i--)
            {
                for (int j = mapX - 1; j >= 0; j--)
                {
                    if (map->map[i][j] != 0 && map->map[i][j] != 1 && map->map[i][j] != 2)
                    {
                        if (map->map[i + 1][j] == 1 || map->map[i + 1][j] == 2)
                        {
                            pthread_mutex_lock(&mutex);
                            IceBlock(map);
                            pthread_mutex_unlock(&mutex);
                            exit = 1;
                            break;
                        }
                        pthread_mutex_lock(&mutex);
                        map->map[i + 1][j] = map->map[i][j];
                        map->map[i][j] = 0;
                        pthread_mutex_unlock(&mutex);
                    }
                }
                if (exit != 0)
                    break;
            }
        }
    }
} // end MoveBlock

void DownIcedBlock(Map *map)
{
    for (int i = mapY - 1; i >= 0; i--)
    {
        for (int j = mapX - 1; j >= 0; j--)
        {
            if (map->map[i][j] == 2 && map->map[i + 1][j] != 1)
            {
                map->map[i + 1][j] = 2;
                map->map[i][j] = 0;
            }
        }
    }
} // end DownIcedBlock

void ClearLine(Map *map, int line)
{
    for (int i = 1; i < mapX - 1; i++)
        map->map[line][i] = 0;

    DownIcedBlock(map);

    map->score += 1000;
} // end ClearLine

void *CheckLine(void *param)
{
    Map *map = (Map *)param;
    int count = 0;

    while (isDead == 0)
    {
        for (int i = mapY - 1; i >= 0; i--)
        {
            count = 0;
            for (int j = mapX - 1; j >= 0; j--)
            {
                if (map->map[i][j] == 2)
                    count++;
                if (map->map[3][j] == 2)
                    isDead = 1;
            }
            if (count >= mapX - 2)
            {
                pthread_mutex_lock(&mutex);
                ClearLine(map, i);
                pthread_mutex_unlock(&mutex);
            }
        }
    }
} // end CheckLine

void PrintMap(Map *map)
{
    system("clear");
    printf("\n\tS C O R E : %d\n", map->score);
    for (int i = 0; i < mapY; i++)
    {
        for (int j = 0; j < mapX; j++)
        {
            switch (map->map[i][j])
            {
            case 0:
                printf("  ");
                // printf("%d ", map->map[i][j]);
                break;
            case 1:
                printf("■ ");
                // printf("%d ", map->map[i][j]);
                break;
            case 2:
                printf("▨ ");
                // printf("%d ", map->map[i][j]);
                break;
            case 3:
                printf("🦄");
                // printf("%d ", map->map[i][j]);
                break;
            case 4:
                printf("🐶");
                // printf("%d ", map->map[i][j]);
                break;
            case 5:
                printf("🐷");
                // printf("%d ", map->map[i][j]);
                break;
            case 6:
                printf("🐹");
                // printf("%d ", map->map[i][j]);
                break;
            case 7:
                printf("🐼");
                // printf("%d ", map->map[i][j]);
                break;
            case 8:
                printf("🐻");
                // printf("%d ", map->map[i][j]);
                break;
            case 9:
                printf("🐺");
                // printf("%d ", map->map[i][j]);
                break;
            default:
                break;
            }
        }
        printf("\n");
    }
} // end PrintMap

int main(void)
{
    Map *map = (Map *)malloc(sizeof(Map));

    pthread_t tet[3];
    int id = 0;

    ClearMap(map);
    map->score = 0;

    CreateBlock(map);

    id = pthread_create(&tet[0], NULL, FallBlock, (void *)map);
    if (id < 0)
    {
        printf("에러\n");
        return 0;
    }

    id = pthread_create(&tet[1], NULL, MoveBlock, (void *)map);
    if (id < 0)
    {
        printf("에러\n");
        return 0;
    }

    id = pthread_create(&tet[2], NULL, CheckLine, (void *)map);
    if (id < 0)
    {
        printf("에러\n");
        return 0;
    }

    while (isDead == 0)
    {
        PrintMap(map);
        usleep(50000);
    }

    GameOver(map);

    for (int i = 0; i < 3; i++)
        pthread_join(tet[i], NULL);
    free(map);
    return 0;
} // end main