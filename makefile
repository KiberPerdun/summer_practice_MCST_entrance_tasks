CC = gcc
LIBURING_REPO = https://github.com/axboe/liburing/
LIBURING_DIR = liburing

all: task1 task2 task3 task4 task5 task6 task7 task8 task9 task10

liburing:
	git clone $(LIBURING_REPO) $(LIBURING_DIR)
	cd $(LIBURING_DIR) && ./configure && make

task1: TASK-1/src/main.c
	$(CC) $< -o TASK-1/main

task2: liburing
	$(CC) -I TASK-2/include -I $(LIBURING_DIR)/src/include -I liburing/src/include \
 			 TASK-2/src/*.c $(LIBURING_DIR)/src/liburing.a -o TASK-2/main

task3: TASK-3/src/main.c
	$(CC) $< -o TASK-3/main

task4: TASK-4/src/main.c
	$(CC) -I TASK-4/include TASK-4/src/*.c -o TASK-4/main

task5: TASK-5/src/main.c
	$(CC) -I TASK-5/include TASK-5/src/*.c -o TASK-5/main

task6: TASK-6/src/main.c
	$(CC) -I TASK-6/include TASK-6/src/*.c -o TASK-6/main

task7: TASK-7/src/main.c
	$(CC) $< -o TASK-7/main

task8: TASK-8/src/main.c
	$(CC) $< -o TASK-8/main

task9: TASK-9/src/main.c
	$(CC) -I TASK-9/include TASK-9/src/*.c -o TASK-9/main

task10: TASK-10/src/main.c
	$(CC) $< -o TASK-10/main
