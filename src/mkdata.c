#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OB_LIST_N 6740  //観測地点数
#define YEAR_S 1997     //1997  収集するデータの開始年
#define YEAR_E 2019     //2017  収集するデータの終了年

typedef struct{
    int ob_p_ID;    //観測地点番号
    //char ob_p_name[];
    double latitude;
    double longitude;
    //int ob_p_start;
} OB_LIST;

typedef struct{
    int ob_n;   //有感の震度を計測した観測地点数
    int year;
    int month;
    int day;
    int hour; 
    int min;
    double sec;
    double latitude;
    double longitude;
    double depth;
    double magnitude;
} Epicenter;

typedef struct{
    int ob_p_ID;   //観測地点番号

    double latitude;
    double longitude;

    int phase1_day;
    int phase1_hour;
    int phase1_min;
    double phase1_sec;

    double SeismicIntensity;
    int IntensityClass; //0 ~ 9

} OB_DATA;

int Check_Observation_Points(const OB_LIST *ob_list, OB_DATA *ob_data);
int SeismicIntensity_to_10classes(double SeismicIntensity);
void ReplaceSpaceWith0(char *string, int n);
int isSapceOnly(char *string, int n);

int main(void){

    char filename_list[] = "data/code_p.dat";   //観測地点データ
    FILE  *fp_list;

    //共通
    double latitude, longitude;
    int EarthQuake_ID = 0;
    //観測点リスト用
    OB_LIST ob_list[OB_LIST_N];
    int ob_p_ID;
    char ob_p_name[100];

    //震源データ用
    char head;
    Epicenter epic;
    char ob_n_str[10], year_str[10], month_str[10], day_str[10], hour_str[10], min_str[10], sec_str[10];
    char latitude_str[10], longitude_str[10], depth_str[10], magnitude_str[10];  

    //計測データ用
    OB_DATA ob_data;
    char ob_p_ID_str[10], phase1_day_str[10], phase1_hour_str[10], phase1_min_str[10], phase1_sec_str[10];
    char SeismicIntensity_str[10];

    if ((fp_list = fopen(filename_list, "r")) == NULL){
        printf("cannot open file:%s\n", filename_list);
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
    }
    fclose(fp_list);

    for (int i = YEAR_S; i <= YEAR_E; i++){
        char filename_data[100] = "data/i";    //.dat   気象庁からダウンロードしたデータ
        char filename_out_epic[100] = "data_shaped/shingen";    //.csv  震源データ
        char filename_out_ob_data[100] = "data_shaped/kansoku"; //.csv  観測地点データ
        FILE *fp_data, *fp_out_epic, *fp_out_ob_data;

        char file_year_str[100];
        sprintf(file_year_str, "%d", i); 

        strcat(filename_data, file_year_str);
        strcat(filename_data, ".dat");
        strcat(filename_out_epic, file_year_str);
        strcat(filename_out_epic, ".csv");
        strcat(filename_out_ob_data, file_year_str);
        strcat(filename_out_ob_data, ".csv");
        printf("%s, %s, %s\n", filename_data, filename_out_epic, filename_out_ob_data);

        if ((fp_data = fopen(filename_data, "r")) == NULL){
            printf("cannot open file:%s\n", filename_data);
            return 1;
        }
        if ((fp_out_epic = fopen(filename_out_epic, "w")) == NULL){
            printf("cannot open file:%s\n", filename_out_epic);
            return 1;
        }
        if ((fp_out_ob_data = fopen(filename_out_ob_data, "w")) == NULL){
            printf("cannot open file:%s\n", filename_out_ob_data);
            return 1;
        }

        while ((head = fgetc(fp_data)) != EOF){
            if (head == 'A'){
                //震源データ読み込み
                fscanf(fp_data, "%4[^\n]%2[^\n]%2[^\n]%2[^\n]%2[^\n]%4[^\n]%*4[^\n]%7[^\n]%*4[^\n]%8[^\n]%*4[^\n]%5[^\n]%*3[^\n]%2[^\n]%*1[^\n]", year_str, month_str, day_str, hour_str, min_str, sec_str, latitude_str, longitude_str, depth_str, magnitude_str);
                fscanf(fp_data, "%*13[^\n]%*[^0-9]");
                fscanf(fp_data, "%[0-9]", ob_n_str);
                fscanf(fp_data, "%*[^\n]%*1[\n]");
                epic.year = atoi(year_str);
                epic.month = atoi(month_str);
                epic.day = atoi(day_str);
                epic.hour = atoi(hour_str); 
                epic.min = atoi(min_str);
                epic.sec = atoi(sec_str) / 100.0;
                ReplaceSpaceWith0(latitude_str, 10);
                epic.latitude = atof(latitude_str) / 10000.0;
                ReplaceSpaceWith0(longitude_str, 10);
                epic.longitude = atof(longitude_str) / 10000.0;
                epic.ob_n = atoi(ob_n_str); //観測点数
                if (epic.latitude < 1){
                    printf("%s, %f\n", latitude_str, epic.latitude);
                }
                if (depth_str[4] == 'X'){
                    //深さ固定での場合
                    depth_str[3] = '\0';
                    epic.depth = atof(depth_str);
                }else{
                    epic.depth = atof(depth_str) / 100.0;
                }
                if (magnitude_str[0] == '-'){
                    epic.magnitude = atof(magnitude_str) / 10.0;
                }else if (magnitude_str[0] == 'A' || magnitude_str[0] == 'B' || magnitude_str[0] == 'C' || magnitude_str[0] == 'D'){
                    char c_tmp0 = magnitude_str[0];
                    char c_tmp1 = magnitude_str[1];
                    magnitude_str[0] = '-';
                    magnitude_str[1] = c_tmp0 - 'A' + 1;
                    magnitude_str[2] = c_tmp1;
                    epic.magnitude = atof(magnitude_str) / 10.0;
                }else {
                    epic.magnitude = atof(magnitude_str) / 10.0;
                }
                if (isSapceOnly(magnitude_str, 10) || isSapceOnly(depth_str, 10)){
                    //マグニチュードまたは深さの欠損時
                    printf("マグニチュードまたは深さの欠損 : %s/%s/%s, %s:%s:%s, latitude : %f, longitude : %f, depth : %s, magnitude : %s, 観測地点数 : %s\n", year_str, month_str, day_str, hour_str, min_str, sec_str, epic.latitude, epic.longitude, depth_str, magnitude_str, ob_n_str);
                }else{
                    //震度データの読み込み
                    int line_N = epic.ob_n;
                    for (int j = 0; j < line_N; j++){
                        fscanf(fp_data, "%7[^\n]%*1[^\n]%2[^\n]%2[^\n]%2[^\n]%3[^\n]%*3[^\n]%2[^\n]%*[^\n]%*1[\n]", ob_p_ID_str, phase1_day_str, phase1_hour_str, phase1_min_str, phase1_sec_str, SeismicIntensity_str);
                        if (SeismicIntensity_str[0] == '/'){
                            epic.ob_n -= 1;
                        }else{
                            ob_data.ob_p_ID = atoi(ob_p_ID_str);
                            ob_data.phase1_day = atoi(phase1_day_str);
                            ob_data.phase1_hour = atoi(phase1_hour_str);
                            ob_data.phase1_min = atoi(phase1_min_str);
                            ob_data.phase1_sec = atoi(phase1_sec_str);
                            ob_data.SeismicIntensity = atof(SeismicIntensity_str) / 10.0;
                            ob_data.IntensityClass = SeismicIntensity_to_10classes(ob_data.SeismicIntensity);
                            if(Check_Observation_Points(ob_list, &ob_data) == -1){
                                printf("不明な観測地点ID : %d\n", ob_data.ob_p_ID);
                            }
                            //震度データの書き込み
                            fprintf(fp_out_ob_data, "%d,%f,%f,%f,%d\n", EarthQuake_ID, ob_data.latitude, ob_data.longitude, ob_data.SeismicIntensity, ob_data.IntensityClass);
                        }
                    }
                    //震源データの書き込み
                    if(epic.ob_n > 0){
                        //震源データの書き込み
                        fprintf(fp_out_epic, "%d,%d,%f,%f,%f,%f\n", EarthQuake_ID, epic.ob_n, epic.latitude, epic.longitude, epic.depth, epic.magnitude);
                        EarthQuake_ID ++;
                    }
                }

            }else{
                fscanf(fp_data, "%*[^\n]%*1[\n]");
            }
        }
        fclose(fp_data);
        fclose(fp_out_epic);
        fclose(fp_out_ob_data);
    }
}

int Check_Observation_Points(const OB_LIST *ob_list, OB_DATA *ob_data){
    for (int i = 0; i < OB_LIST_N; i++){
        if (ob_list[i].ob_p_ID == ob_data->ob_p_ID){
            ob_data->latitude = ob_list[i].latitude;
            ob_data->longitude = ob_list[i].longitude;
            return 0;
        }
    }
    return -1;
}
int SeismicIntensity_to_10classes(double SeismicIntensity){
    if(SeismicIntensity < 0.5){
        return 0;
    }else if(0.5 <=SeismicIntensity && SeismicIntensity < 1.5){
        return 1;
    }else if(1.5 <=SeismicIntensity && SeismicIntensity < 2.5){
        return 2;
    }else if(2.5 <=SeismicIntensity && SeismicIntensity < 3.5){
        return 3;
    }else if(3.5 <=SeismicIntensity && SeismicIntensity < 4.5){
        return 4;
    }else if(4.5 <=SeismicIntensity && SeismicIntensity < 5.0){
        return 5;   //5-
    }else if(5.0 <=SeismicIntensity && SeismicIntensity < 5.5){
        return 6;   //5+
    }else if(5.5 <=SeismicIntensity && SeismicIntensity < 6.0){
        return 7;   //6-
    }else if(6.0 <=SeismicIntensity && SeismicIntensity < 6.5){
        return 8;   //6+
    }else if(6.5 <=SeismicIntensity){
        return 9;   //5+
    }else {
        return -1;  //error
    }
}

void ReplaceSpaceWith0(char *string, int n){
    //文字列の半角すぺーーすを0で置換
    int i;
    for(i = 0; i < n; i++){
        if(string[i] == '\0'){
            break;
        }
        else if(string[i] == ' '){
            string[i] = '0';
        }
    }
}
int isSapceOnly(char *string, int n){
    //文字列の中身がない、あるいは半角スペースだけのとき1を返す。
    int i = 0;
    if(string[0] == '\0'){
        return 1;
    }
    while(string[i] != '\0'){
        if(string[i] != ' '){
            return 0;
        }
        i++;
        if (i >= n){
            break;
        }
    }
    return 1;
}