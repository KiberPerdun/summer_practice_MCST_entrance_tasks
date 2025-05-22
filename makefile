CC = gcc

task1: TASK-1/src/main.c
	$(CC) $< -o TASK-1/main

task2:
	$(CC) -I TASK-2/include TASK-2/src/*.c -o TASK-2/main

task3: TASK-3/src/main.c
	$(CC) $< -o TASK-3/main

task4: TASK-4/src/main.c
	$(CC) $< -o TASK-4/main

task5: TASK-5/src/main.c
	$(CC) $< -o TASK-5/main

task6: TASK-6/src/main.c
	$(CC) $< -o TASK-6/main

task7: TASK-7/src/main.c
	$(CC) $< -o TASK-7/main

task8: TASK-8/src/main.c
	$(CC) $< -o TASK-8/main

task9: TASK-9/src/main.c
	$(CC) $< -o TASK-9/main

task10: TASK-10/src/main.c
	$(CC) $< -o TASK-10/main
