#include<iostream>
#include"core\base64.h"
using namespace std;

int main() {
	string undecrypted = "I2luY2x1ZGUgPHN0ZGlvLmg+CgppbnQgZ3JvdXBzWzUwXTsKCmludCBtYWluKCkKewogICAgaW50IGksIGosIG51bTsKICAgIAogICAgc2NhbmYoIiVkIiwgJm51bSk7CiAgICBmb3IgKGkgPSAwOyBudW07IGkrKykgewogICAgICAgIGlmIChudW0taS0yID49IDApIHsKICAgICAgICAgICBudW0gLT0gaSsyOwogICAgICAgICAgIGdyb3Vwc1tpXSA9IGkrMjsKICAgICAgICB9ZWxzZSB7CiAgICAgICAgICAgICAgZm9yIChqID0gLS1pOyBudW07IGotLSkgewogICAgICAgICAgICAgICAgICBpZiAoaiA8IDApCiAgICAgICAgICAgICAgICAgICAgIGogPSBpOwogICAgICAgICAgICAgICAgICBncm91cHNbal0rKzsKICAgICAgICAgICAgICAgICAgbnVtLS07CiAgICAgICAgICAgICAgfQogICAgICAgIH0KICAgIH0KICAgIGZvciAoaSA9IDA7IGdyb3Vwc1tpXTsgaSsrKQogICAgICAgIHByaW50ZigiJWQgIiwgZ3JvdXBzW2ldKTsgCiAgICByZXR1cm4gMDsKfSAK";
	string result = base64_decode(undecrypted).c_str();
	cout << result.c_str();
}