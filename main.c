#include <stdio.h>
#include <stdlib.h>


enum
{
    PUTTING_VERTICAL,
    PUTTING_HORIZONTAL
}PuttingStyle;

size_t maxCnt = 0;
size_t totalGood = 0;


size_t **CreateMatrix(size_t N, size_t M)
{
    size_t i;
    size_t **res = NULL;
    res = (size_t **)malloc(N*sizeof(size_t *));
    for(i = 0; i < N; ++i)
        res[i] = (size_t *)malloc(M*sizeof(size_t));

    return res;
}

void DeleteMatrix(size_t **A, size_t N, size_t M)
{
    size_t i;
    for(i = 0; i < N; ++i)
        free(A[i]);
    free(A);
}

size_t **CopyMatrix(size_t **A, size_t N, size_t M)
{
    size_t **res = CreateMatrix(N, M);
    size_t i, j;
    for(i = 0; i < N; ++i)
        for(j = 0; j < M; ++j)
            res[i][j] = A[i][j];

    return res;
}

void PrintMatrix(size_t **A, size_t N, size_t M)
{
    size_t i, j;
    for(i = 0; i < N; ++i)
    {
        for(j = 0; j < M; ++j)
        {
            char c;
            switch(A[i][j])
            {
            case 0:
                c = '0';
                break;
            case 1:
                c = '|';
                break;
            case 2:
                c = '-';
                break;
            }

            printf("%c ", c);
        }
        printf("\n");
    }


}

int getNexFreeCell(size_t **A, size_t N,  size_t M, size_t *k, size_t *l)
{
    int i ,j = *l;
    for(i = *k; i < N; ++i)
    {
        for(; j < M; ++j)
            if(!A[i][j])
            {
                *k = i;
                *l = j;
                return 1;
            }
        j = 0;
    }

    return 0;
}

size_t isPutable(size_t **A, size_t N,  size_t M, size_t k, size_t l, size_t putStyle)
{
    size_t res = 0;
    switch(putStyle)
    {
    case PUTTING_VERTICAL:
        if((k + 1 < N) && !A[k][l] && !A[k + 1][l])
            res = 1;
        break;
    case PUTTING_HORIZONTAL:
        if((l + 1 < M) && !A[k][l] && !A[k][l + 1])
            res = 1;
        break;
    default:
        res = 0;
    }

    return res;

}

void PutTile(size_t **A, size_t N,  size_t M, size_t k, size_t l, size_t putStyle)
{
    switch(putStyle)
    {
    case PUTTING_VERTICAL:
        A[k][l] = 1;
        A[k + 1][l] = 1;
        break;
    case PUTTING_HORIZONTAL:
        A[k][l] = 2;
        A[k][l + 1] = 2;
        break;

    }
}


void FillArea(size_t **A, size_t N,  size_t M, size_t k, size_t l, size_t cnt)
{
    if(maxCnt == cnt)
    {
        ++totalGood;
        printf("Good filling\n");
        PrintMatrix(A, N, M);

        return;
    }


//    PrintMatrix(A, N, M);
//    printf("k = %u, l = %u, cnt = %u\n\n", k, l, cnt);

    if(getNexFreeCell(A, N, M, &k, &l))
    {
        if(isPutable(A, N, M, k, l, PUTTING_VERTICAL))
        {
            size_t **AVert = CopyMatrix(A, N, M);
            PutTile(AVert, N, M, k, l, PUTTING_VERTICAL);

            FillArea(AVert, N, M, k, l, cnt + 1);
            DeleteMatrix(AVert, N, M);
        }
        if(isPutable(A, N, M, k, l, PUTTING_HORIZONTAL))
        {
            size_t **AHoriz = CopyMatrix(A, N, M);
            PutTile(AHoriz, N, M, k, l, PUTTING_HORIZONTAL);

            FillArea(AHoriz, N, M, k, l, cnt + 1);
            DeleteMatrix(AHoriz, N, M);
        }
    }
}



int main()
{
    size_t i, j;
    size_t N = 2;
    size_t M = 12;
    maxCnt = M*N/2;
    size_t **A = CreateMatrix(N, M);

    for(i = 0; i < N; ++i)
        for(j = 0; j < M; ++j)
            A[i][j] = 0;
    totalGood = 0;
    FillArea(A, N, M, 0, 0, 0);

    printf("There are %u good fillings\n", totalGood);

    DeleteMatrix(A, N, M);
    return 0;
}
