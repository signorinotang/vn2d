// automatic generated by vnlex.
enum {
	PROD__0 = 0, // -> script
	PROD_array_0 = 28, //array -> '[' ']'
	PROD_array_1 = 29, //array -> '[' value_list ']'
	PROD_attribute_0 = 16, //attribute -> @identifier '=' value
	PROD_attribute_list_0 = 17, //attribute_list -> attribute
	PROD_attribute_list_1 = 18, //attribute_list -> attribute_list attribute
	PROD_attributes_0 = 19, //attributes -> '{' '}'
	PROD_attributes_1 = 20, //attributes -> '{' attribute_list '}'
	PROD_node_0 = 22, //node -> '@<' @identifier '@>' @identifier
	PROD_node_1 = 23, //node -> '@<' @identifier attributes '@>' @identifier
	PROD_node_2 = 24, //node -> '@<' @identifier node_children '@>' @identifier
	PROD_node_3 = 25, //node -> '@<' @identifier attributes node_children '@>' @identifier
	PROD_node_children_0 = 26, //node_children -> node
	PROD_node_children_1 = 27, //node_children -> node_children node
	PROD_number_0 = 10, //number -> @integer
	PROD_number_1 = 11, //number -> @long_integer
	PROD_number_2 = 12, //number -> @float
	PROD_number_3 = 13, //number -> @double
	PROD_object_0 = 21, //object -> '@' @identifier attributes
	PROD_script_0 = 30, //script -> node_children
	PROD_value_0 = 1, //value -> 'true'
	PROD_value_1 = 2, //value -> 'false'
	PROD_value_2 = 3, //value -> number
	PROD_value_3 = 4, //value -> '+' number
	PROD_value_4 = 5, //value -> '-' number
	PROD_value_5 = 6, //value -> @string_literal
	PROD_value_6 = 7, //value -> array
	PROD_value_7 = 8, //value -> object
	PROD_value_8 = 9, //value -> node
	PROD_value_list_0 = 14, //value_list -> value
	PROD_value_list_1 = 15, //value_list -> value_list value
	PROD_COUNT = 31
};
