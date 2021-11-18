#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OB_LIST_N 6740
#define MESH_SIZE 64

#define JAPAN_LAT_S 30.0    //10.0
#define JAPAN_LAT_N 46.0    //46.0

#define JAPAN_LON_E 146.0   //154.0
#define JAPAN_LON_W 128.0   //122.0
//japan S10.0 N46.0 E154.0 W122.0
//honshu S30.0 N46.0 E146.0 W128.0

typedef struct{
    int ob_p_ID;
    //char ob_p_name[];
    double latitude;
    double longitude;
    //int ob_p_start;
} OB_LIST;

int latitude2Ycoor(double latitude);
int longitude2Xcoor(double longitude);
double degmin2_100(double tude);

int main(void){
    char filename_list[] = "data/code_p.dat";
    char filename_out[] = "data_shaped/ObservationPointsMap_honshu6464.csv";
    FILE  *fp_list, *fp_out;

    OB_LIST ob_list[OB_LIST_N];
    int ob_p_ID;
    double latitude, longitude;
    char ob_p_name[100];

    int mesh[MESH_SIZE][MESH_SIZE] = {0};
    int x, y;

    if ((fp_list = fopen(filename_list, "r")) == NULL){
        printf("cannot open file:%s\n", filename_list);
        return 1;
    }
    if ((fp_out = fopen(filename_out, "w")) == NULL){
        printf("cannot open file:%s\n", filename_out);
        return 1;
    }

    for (int i = 0; i < OB_LIST_N; i++){
        if(EOF == fscanf(fp_list, "%d %s %lf %lf%*[^\n]%*[\n]", &ob_p_ID, ob_p_name, &latitude, &longitude)){
            printf("maybe faild to load observation point list\n");
            break;
        }
        ob_list[i].ob_p_ID = ob_p_ID;
        //ob_list[i].p_ = ob_p_name;
        ob_list[i].latitude = latitude / 100.0;
        ob_list[i].longitude = longitude / 100.0;
        x = longitude2Xcoor(ob_list[i].longitude);
        y = latitude2Ycoor(ob_list[i].latitude);
        if (0 <= x && x < MESH_SIZE && 0 <= y && y < MESH_SIZE){
            mesh[x][y] ++;
        }
        else{
            //はじかれたデータの出力
            printf("座標 x, y = %d, %d, ID = %d, %s\n", x, y, ob_p_ID, ob_p_name);
        }
    }
    for (int y = 0; y < MESH_SIZE; y++){
        for (int x = 0; x < MESH_SIZE; x++){
            if (x  < MESH_SIZE - 1){
                fprintf(fp_out, "%d,", mesh[x][y]);
            }
            else{
                fprintf(fp_out, "%d\n", mesh[x][y]);
            }
        }
    }
    fclose(fp_list);
    fclose(fp_out);
    return 0;
}

int latitude2Ycoor(double latitude){
    double latitude_in_100 = degmin2_100(latitude);
    double lat_s = JAPAN_LAT_S;
    double lat_n = JAPAN_LAT_N;
    double lat_s_in_100 = degmin2_100(lat_s);
    double lat_n_in_100 = degmin2_100(lat_n);
    double lat_width_in_100;
    int MeshSize = MESH_SIZE;
    int y; //左上が 0 

    lat_width_in_100 = lat_n_in_100 - lat_s_in_100;
    //printf("latitude width %f\n", MeshSize * (latitude - lat_s) / lat_width);

    y = MeshSize * (latitude_in_100 - lat_s_in_100) / lat_width_in_100;

    return MeshSize - y;
}
int longitude2Xcoor(double longitude){
    double longitude_in_100 = degmin2_100(longitude);
    double lon_w = JAPAN_LON_W;
    double lon_e = JAPAN_LON_E;
    double lon_w_in_100 = degmin2_100(lon_w);
    double lon_e_in_100 = degmin2_100(lon_e);
    double lon_width_in_100;
    int MeshSize = MESH_SIZE;
    int x;  //左上が 0 

    lon_width_in_100 = lon_e_in_100 - lon_w_in_100;
    //printf("longitude %f, x %f\n", longitude, (longitude - lon_w) / lon_width * MeshSize);

    x = MeshSize * (longitude_in_100 - lon_w_in_100) / lon_width_in_100;

    return x;
}

double degmin2_100(double tude){
    int tude_deg = tude;
    double tude_min = tude - tude_deg;
    double tude_in_100 = tude_deg + (tude_min / 60.0) * 100;

    return tude_in_100;
}