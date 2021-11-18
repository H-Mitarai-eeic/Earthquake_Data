mkdata:./src/mkdata.c
	gcc -o ./BIN/mkdata.o ./src/mkdata.c -Wall
reshape_data:./src/reshape_data.c
	gcc -o ./BIN/reshape_data.o ./src/reshape_data.c -Wall
ObservationPoints:./src/ObservationPoints.c
	gcc -o ./BIN/ObservationPoints.o ./src/ObservationPoints.c -Wall