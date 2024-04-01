#include<bits/stdc++.h>
#include <gmp.h>
#include <gmpxx.h>

using namespace std;
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
    gmp_randstate_t state;
    gmp_randinit_default(state);

    mpz_class p, q, phi;
    mpz_urandomb(p.get_mpz_t(), state, 512);
    mpz_nextprime(p.get_mpz_t(), p.get_mpz_t());

    mpz_urandomb(q.get_mpz_t(), state, 512);
    mpz_nextprime(q.get_mpz_t(), q.get_mpz_t());

    n = p * q;
    phi = (p - 1) * (q - 1);
    mpz_class temp;
    do {
        mpz_urandomm(e.get_mpz_t(), state, phi.get_mpz_t());
        mpz_gcd(temp.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t());
    } while (temp != 1);
    mpz_invert(d.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t());
    gmp_randclear(state);
}
string encryptRSA(const string& plaintext, const mpz_class& e, const mpz_class& n) {
    string ciphertext = "";
    int blockSize = mpz_sizeinbase(n.get_mpz_t(), 2) / 8;

    for (size_t i = 0; i < plaintext.size(); i += blockSize) {
        string block = plaintext.substr(i, blockSize);
        mpz_class m(0);
        for (char c : block) {
            int val = charMapping[c];
            m = m * 37 + val; 
        }
        mpz_class encrypted;
        mpz_powm(encrypted.get_mpz_t(), m.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
        ciphertext += encrypted.get_str();
        if (i + blockSize < plaintext.size()) {
            ciphertext += " "; 
        }
    }
    return ciphertext;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <plaintext>" << endl;
        return 1;
    }
    gmp_randstate_t state;
    gmp_randinit_default(state);
    mpz_class n, e, d;
    generateRSAKeys(n, e, d);

    string plaintext = "";
    for (int i = 1; i < argc; ++i) {
        plaintext += argv[i];
        if (i < argc - 1)
            plaintext += " ";
    }
    string ciphertext = encryptRSA(plaintext, e, n);
    cout << ciphertext;
    gmp_randclear(state);

    return 0;
}
