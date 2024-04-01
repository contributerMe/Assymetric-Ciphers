#include<bits/stdc++.h>
#include <gmp.h>
#include <gmpxx.h>

using namespace std;
// mapping 
unordered_map<char, int> charMapping = {
    {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4},
    {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9},
    {' ', 10}, {'a', 11}, {'b', 12}, {'c', 13}, {'d', 14},
    {'e', 15}, {'f', 16}, {'g', 17}, {'h', 18}, {'i', 19},
    {'j', 20}, {'k', 21}, {'l', 22}, {'m', 23}, {'n', 24},
    {'o', 25}, {'p', 26}, {'q', 27}, {'r', 28}, {'s', 29},
    {'t', 30}, {'u', 31}, {'v', 32}, {'w', 33}, {'x', 34},
    {'y', 35}, {'z', 36}
};

void generateRSAKeys(mpz_class& n, mpz_class& e, mpz_class& d) {
    gmp_randstate_t st;
    gmp_randinit_default(st);

    mpz_class p, q, phi;
    mpz_urandomb(p.get_mpz_t(), st, 512);
    mpz_nextprime(p.get_mpz_t(), p.get_mpz_t());

    mpz_urandomb(q.get_mpz_t(), st, 512);
    mpz_nextprime(q.get_mpz_t(), q.get_mpz_t());

    n = p * q;

    phi = (p - 1) * (q - 1);

    mpz_class temp;
    while(temp != 1) {
        mpz_urandomm(e.get_mpz_t(), st, phi.get_mpz_t());
        mpz_gcd(temp.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t());
    } ;

    mpz_invert(d.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t());
    gmp_randclear(st);
}

string signRSA(const string& message, const mpz_class& d, const mpz_class& n) {
    mpz_class m(0);
    for (char c : message) {
        auto it = charMapping.find(c);
        if (it != charMapping.end()) {
            m = m * 37 + it->second; // Adjust range to 0-36
        }
    }

    mpz_class signature;
    mpz_powm(signature.get_mpz_t(), m.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
    return signature.get_str();
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1;
    }

    gmp_randstate_t st;
    gmp_randinit_default(st);

    mpz_class n, e, d;
    generateRSAKeys(n, e, d);

    string plaintext = "";
    for (int i = 1; i < argc; ++i) {
        plaintext += argv[i];
        if (i < argc - 1)
            plaintext += " "; 
    }
    string signature = signRSA(plaintext, d, n);

    cout << signature ;


    return 0;
}
