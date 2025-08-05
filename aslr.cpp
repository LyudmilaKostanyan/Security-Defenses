#include <iostream>
#include <cstdlib>

int global_var = 123;

void foo() {}

int main() {
	int stack_var = 456;
	void* heap_var = malloc(1);

	std::cout << "Address of main(): " << (void*)main << std::endl;
	std::cout << "Address of foo(): " << (void*)foo << std::endl;
	std::cout << "Address of global_var: " << &global_var << std::endl;
	std::cout << "Address of stack_var: " << &stack_var << std::endl;
	std::cout << "Address of heap_var: " << heap_var << std::endl << std::endl;

	free(heap_var);
	return 0;
}
