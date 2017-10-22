all: 	
	gcc ./commander/commander.c -o ./bin/commander
	gcc ./process_manager/processManager.c -o ./bin/process_manager

commander:	
	./bin/commander ./input/processo1.txt
manager:	
	./bin/process_manager ./input/processo1.txt
clean: 
	rm -rf ./bin/*
