all: 	
	gcc ./commander/commander.c -o ./bin/commander
	gcc ./process_manager/processManager.c -o ./bin/process_manager
clean: 
	rm -rf ./bin/*