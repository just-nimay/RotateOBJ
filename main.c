#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>

/* ===================================== */
/*               МАТРИЦЫ                 */
/* ===================================== */

void transform_matrix(int rows, int colums, float matrix[][colums], float output[][colums]) {
    rows = 3;
    for (int i=0; i < rows; i++) {
        if (i==2){
            output[i][0] = 0;
        } else {
            output[i][0] = matrix[i][0];
        }

    }
}

/* Умножение матриц */

void matrix_mult(int rows_a, int column_a, float mat_a[rows_a][column_a], int rows_b, int column_b, float mat_b[rows_b][column_b], float output_matrix[rows_a][column_b])
{

    for (int k=0; k<column_b; k++){
        for (int i=0; i < rows_a; i++) {
            float sum_m = 0;
            for (int j=0; j<rows_b; j++) {
                //printf("mat_a[%d][%d]: %.2lf \t mat_b[%d][%d]: %.2lf\n",i,j, mat_a[i][j], j,k, mat_b[j][k]);
                sum_m += mat_a[i][j] * mat_b[j][k];
                //printf("sum_m = %lf\n", sum_m);
            }
            output_matrix[i][k] = sum_m;
            //printf("output_matrix[%d][%d] = %lf\n\n", i, k, sum_m);
        }
    }
}

/* Вращение вокруг оси X */

void rot_x(float input_matrix[3][1], double a, float output_matrix[3][1]) {

    float matrix_rotation[3][3] = { {1, 0, 0}, {0, cos(a), -sin(a)}, {0, sin(a), -cos(a)} };

    matrix_mult(3, 3, matrix_rotation, 3, 1, input_matrix, output_matrix);

    for (int i=0; i<3; i++) {
        for (int j=0; j<1; j++) {
            //printf("ROT X: output_matrix[%d][%d]= %lf\n", i, j, output_matrix[i][j]);
        }
    }


}

/* Вращение вокруг оси Y */

void rot_y(float input_matrix[3][1], float a, float output_matrix[3][1]) {

    float matrix_rotation[3][3] = { {cos(a), 0, sin(a)}, {0, 1, 0}, {-sin(a), 0, cos(a)} };

    matrix_mult(3, 3, matrix_rotation, 3, 1, input_matrix, output_matrix);


}

/* Вращение вокруг оси Z */

void rot_z(float input_matrix[3][1], float a, float output_matrix[3][1]) {

    float matrix_rotation[3][3] = { {cos(a), -sin(a), 0}, {sin(a), cos(a), 0}, {0, 0, 1} };

    matrix_mult(3, 3, matrix_rotation, 3, 1, input_matrix, output_matrix);


}

/* ===================================== */
/*             ОБЪЕМНЫЕ ТЕЛА             */
/* ===================================== */


struct Cube{

    float dots[8][3][1];
    int side;
    int display_side;
    float angle_rot;
    void (*initCube)(struct Cube*);

};

void initCube_Cube(struct Cube* cb) {
    int s = round(cb->side/2);

    cb->dots[0][0][0] = -s;
    cb->dots[0][1][0] = -s;
    cb->dots[0][2][0] = +s;

    cb->dots[1][0][0] = +s;
    cb->dots[1][1][0] = -s;
    cb->dots[1][2][0] = +s;

    cb->dots[2][0][0] = +s;
    cb->dots[2][1][0] = +s;
    cb->dots[2][2][0] = +s;

    cb->dots[3][0][0] = -s;
    cb->dots[3][1][0] = +s;
    cb->dots[3][2][0] = +s;

    cb->dots[4][0][0] = -s;
    cb->dots[4][1][0] = +s;
    cb->dots[4][2][0] = -s;

    cb->dots[5][0][0] = +s;
    cb->dots[5][1][0] = +s;
    cb->dots[5][2][0] = -s;

    cb->dots[6][0][0] = +s;
    cb->dots[6][1][0] = -s;
    cb->dots[6][2][0] = -s;

    cb->dots[7][0][0] = -s;
    cb->dots[7][1][0] = -s;
    cb->dots[7][2][0] = -s;

}



/* ===================================== */
/*                 ЭКРАН                 */
/* ===================================== */

struct Display {

    int width;
    int height;

    char ** data;

    char backgroud;

    void (*update)(struct Display*);
    void (*drawObj)(struct Display*, int count_dots, float object[][3][1]);
    void (*show)(struct Display*);
    void (*drawLine)(struct Display*, int x0, int y0, int x1, int y1);

};

void update_Display(struct Display* dp) {
    dp->data=(char**)malloc(sizeof(char*) * dp->width);
    for (int i=0; i< dp->width; i++) {
        dp->data[i]=(char*)malloc(sizeof(char*) * dp->height);
    }
    for (int i=0; i < dp->width; i++) {
        for (int j=0; j< dp->height; j++) {
            dp->data[i][j] = dp->backgroud;
        }
    }
}

void drawObj_Display(struct Display* dp, int cout_dots, float obj[cout_dots][3][1]) {

    for (int i=0; i<cout_dots; i++) {

        //printf("i: %d\n", i);

        // точка 0

        int x0 = (int) (obj[i][0][0] + round(dp->width/2));
        int y0 = (int) (obj[i][1][0] + round(dp->height/2));
        //printf("x0:%d \t y0: %d\n\n", x0, y0);

        // точка 1
        int x1 = (int) (obj[i+1][0][0] + round(dp->width/2));
        int y1 = (int) (obj[i+1][1][0] + round(dp->height/2));
        //printf("x1:%d \t y1: %d\n", x1, y1);
        dp->drawLine(dp, x0, y0, x1, y1); // линия между точками 0 и 1


        // точка 2
        int x2 = (int) (obj[cout_dots-1-i][0][0] + round(dp->width/2));
        int y2 = (int) (obj[cout_dots-1-i][1][0] + round(dp->height/2));
        //printf("x2:%d \t y2: %d\n", x2, y2);
        dp->drawLine(dp, x0, y0, x2, y2); // линия между точками 0 и 2

        // точка 3
        if (i==0 || i==4) {
            int x3 = (int) (obj[i+3][0][0] + round(dp->width/2));
            int y3 = (int) (obj[i+3][1][0] + round(dp->height/2));
            //printf("x3:%d \t y3: %d\n", x3, y3);
            dp->drawLine(dp, x0, y0, x3 , y3); // линия между точками 0 и 3
        }

        //dp->show(dp);

    }

}

void show_Display(struct Display* dp) {

    for (int i=0; i<dp->width; i++) {
        for (int j=0; j<dp->height; j++) {
            printf("%c", dp->data[i][j]);
        }
        printf("\n");
    }
}

void show_Screen(struct Display* dp) {
    system("cls");
    int w = dp->width;
    int h = dp->height;
    char screen[w*h + 1];
    screen[w*h] = '\0';

    for (int i=0; i<dp->width; i++) {
        for (int j=0; j<dp->height; j++) {
            screen[i+j*w] = ' ';
            screen[i-1+(j-17)*w]=dp->data[i][j];
            if (i==w-1 && j==w-1) {
                screen[w+j*w] = '\0';
            }

            //printf("%c", dp->data[i][j]);
        }
        //printf("\n");
    }
    printf(screen);
}

void drawLine_Display(struct Display* dp, int x0, int y0, int x1, int y1) {
    //printf("A(%d, %d) B(%d, %d)\n", x0, x0, x1, y1);
    if (x0==x1 && y0==y1) {
        return 0;
    }
    int A, B, sign;
    A=y1-y0;
    B=x0-x1;
    if (abs(A) > abs(B)) {
        sign = 1;
    } else {
        sign = -1;
    }

    int signa, signb;
    if (A < 0) {
        signa = -1;
    } else {
        signa = 1;
    }
    if (B<0) {
        signb = -1;
    } else {
        signb = 1;
    }
    int f=0;
    dp->data[y0][x0] = '*';
    int x=x0, y=y0;
    if (sign == -1) {
        do {
            f+= A * signa;
            if (f>0) {
                f-= B * signb;
                y += signa;
            }
            x-= signb;
            dp->data[y][x] = '*';
        } while (x != x1 || y!= y1);
    } else {
        do {
            f += B * signb;
            if (f > 0) {
                f -= A * signa;
                x-= signb;
            }
            y+= signa;
            dp->data[y][x] = '*';
        } while (x!=x1 || y != y1);
    }
}

// char update_display(char display, )


int main()
{
    int size_disp = 17;

    struct Display dp;
    dp.update= &update_Display;
    dp.drawObj = &drawObj_Display;
    dp.drawLine = &drawLine_Display;
    dp.show = &show_Display;

    dp.width=70;
    dp.height=50;
    dp.backgroud=' ';

    dp.update(&dp);

    struct Cube cb;
    cb.side=15;
    cb.display_side=size_disp;
    cb.angle_rot = 0.01;
    cb.initCube = &initCube_Cube;

    cb.initCube(&cb);

    float angle = 0;
    long long count = 100000000000;
    while (count!=0) {
        dp.update(&dp);
        //init_display(size_disp, disp);

        float rotated_cb[8][3][1];

        for (int pt=0; pt < 8; pt++) {
            float a[3][1] = { {cb.dots[pt][0][0]}, {cb.dots[pt][1][0]}, {cb.dots[pt][2][0]} };
            //float a[3][1] = { {3}, {6}, {0} };
            rot_z(a, angle, rotated_cb[pt]);
            float b[3][1] = { {rotated_cb[pt][0][0]}, {rotated_cb[pt][1][0]}, {rotated_cb[pt][2][0]} };
            rot_y(b, angle, rotated_cb[pt]);

        }

        dp.drawObj(&dp, 8, rotated_cb);
        //dp.show(&dp);
        show_Screen(&dp);

        /*
        for (int pt=0; pt<8; pt++){
            int x = (int) (rotated_cb[pt][0][0] + round(size_disp/2));
            int y = (int) (rotated_cb[pt][1][0] + round(size_disp/2));

        /disp[x][y] = '*';
        }
        */

        Sleep(100);
        angle += 0.1;

        // printf("Angle: %f", angle);
        //show_display(size_disp, disp);
        count--;

        }
    return 0;
}
