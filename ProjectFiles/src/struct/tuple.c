#include <stdio.h>

// wrap the multiple values into a struct
struct Tuple {
	int x, y;
};

// Function to return multiple values using struct
struct Tuple initialize(int x, int y)
{
	struct Tuple tuple = { x, y };

	return tuple;
}