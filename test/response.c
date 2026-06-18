#include "http.h"
#include <stdio.h>

int main() 
{
	http_response response;
	init_http_response(&response);

	printf("Initialized HTTP Response:\n");
	printf("Status code: %d\n", response.status_code);
	printf("Reason Phrase; %s\n", response.reason_phrase);

}
