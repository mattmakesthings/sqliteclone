#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define True 1

struct InputBuffer_t{
	char* buffer;
	size_t buffer_length;
	ssize_t input_length;
};

typedef struct InputBuffer_t InputBuffer;

InputBuffer* new_input_buffer(){
		InputBuffer* input_buffer = malloc(sizeof(InputBuffer));
		input_buffer -> buffer = NULL;
		input_buffer -> buffer_length = 0;
		input_buffer -> input_length = 0;

		return input_buffer;
}

void print_prompt(){
	printf("db> ");
}

void read_input(InputBuffer * input_buffer){
	ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

	if (bytes_read <= 0){
		printf("Error reading input\n");
		exit(EXIT_FAILURE);
	}

	input_buffer->input_length = bytes_read-1;
	input_buffer->buffer[bytes_read -1 ] = 0;

}

enum MetaCommandResult_t {
	META_COMMAND_SUCCESS,
	META_COMMAND_UNRECOGNIZED_COMMAND
};
typedef enum MetaCommandResult_t MetaCommandResult;

MetaCommandResult do_meta_command(InputBuffer* input_buffer){
	if(strcmp(input_buffer->buffer, ".exit") == 0){
		exit(EXIT_SUCCESS);
	}
	else{
		return META_COMMAND_UNRECOGNIZED_COMMAND;
	}
}

const uint32_t COLUMN_USERNAME_SIZE = 32;
const uint32_t COLUMN_EMAIL_SIZE = 255;

struct Row_t{
	uint32_t id;
	char username[COLUMN_USERNAME_SIZE];
	char email[COLUMN_EMAIL_SIZE];
}

typedef struct Row_t Row;

#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

const uint32 ID_SIZE = size_of_attribute(Row, id);
const uint32 USERNAME_SIZE = size_of_attribute(Row, username);
const uint32 EMAIL_SIZE = size_of_attribute(Row, email);
const uint32 ID_OFFSET = 0;
const uint32 USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32 EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const uint32 ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;


enum StatementType_t { STATEMENT_INSERT , STATEMENT_SELECT };
typedef enum StatementType_t StatementType;

struct Statement_t {
	StatementType type;
	Row row_to_insert; //only used by insert statement
};

typedef struct Statement_t Statement;


enum PrepareResult_t { PREPARE_SUCCESS, PREPARE_UNRECOGNIZED_STATEMENT};
typedef enum PrepareResult_t PreparieResult;





PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement){
	if (strncmp(input_buffer->buffer, "insert",6) == 0){
		statement->type = STATEMENT_INSERT;
		int args_assigned = sscanf(
			input_buffer->buffer, "insert %d %s %s",&(statement->row_to_insert.id),
			statement->row_to_insert.username, statment->row_to_insert.email);
		if (args_assigned < 3)
			return PREPARE_SYNTAX_ERROR
		return PREPARE_SUCCESS;
	}
	if (strcmp(input_buffer->buffer, "select") == 0){
		statement-> type = STATEMENT_SELECT;
		return PREPARE_SUCCESS;
	

	return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement* statement){
	switch(statement->type){
		case(STATEMENT_INSERT):
			printf("Insert statement\n");
			break;
		case(STATEMENT_SELECT):
			printf("Select statement\n");
			break;

	}
}


int main(int argc, char* argv[]){
	InputBuffer* input_buffer = new_input_buffer();

	while(True){
		print_prompt();
		read_input(input_buffer);

		// check if meta command
		if(input_buffer->buffer[0] == '.'){
			switch(do_meta_command(input_buffer)){
				case (META_COMMAND_SUCCESS):
					continue;
				case (META_COMMAND_UNRECOGNIZED_COMMAND):
					printf("Unrecognized command '%s'\n", input_buffer->buffer);
					continue;
			}
		}

		//
		Statement statement;
		switch (prepare_statement(input_buffer, &statement)){
			case (PREPARE_SUCCESS):
				break;
			case (PREPARE_UNRECOGNIZED_STATEMENT):
				printf("Unrecognized keyword at start of '%s'.\n",input_buffer->buffer);
				continue;
		}

	

	execute_statement(&statement);
	printf("Executed.\n");

	}
}
