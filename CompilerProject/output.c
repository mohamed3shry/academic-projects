#include <stdio.h>
int main() {
    int age;
    age = 18;
    const char* status;
    if((age >= 18)) {
    status = "Adult";
    } else {
    status = "Minor";
    }
    if((status == "Adult")) {
    int allowed;
    allowed = 1;
    } else {
    int allowed;
    allowed = 0;
    }
return 0;
}
