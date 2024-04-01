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
// Function to generate ElGamal key pairs
void generateElGamalKeys(mpz_class& p, mpz_class& g, mpz_class& x, mpz_class& y) {
    gmp_randstate_t state;
    gmp_randinit_default(state);

    // Generate a large prime number p
    mpz_urandomb(p.get_mpz_t(), state, 512);
    mpz_nextprime(p.get_mpz_t(), p.get_mpz_t());

    // Generate a generator g
    mpz_class temp;
    do {
        mpz_urandomm(g.get_mpz_t(), state, p.get_mpz_t());
        mpz_powm(temp.get_mpz_t(), g.get_mpz_t(), p.get_mpz_t() - 1, p.get_mpz_t());
    } while (temp != 1);

    // Choose a random private key x
    mpz_urandomm(x.get_mpz_t(), state, p.get_mpz_t());

    // Compute the public key y = g^x mod p
    mpz_powm(y.get_mpz_t(), g.get_mpz_t(), x.get_mpz_t(), p.get_mpz_t());

    // Clear memory
    gmp_randclear(state);
}

// Function to encrypt plaintext using ElGamal
void encryptElGamal(const string& plaintext, const mpz_class& p, const mpz_class& g, const mpz_class& y, vector<pair<mpz_class, mpz_class>>& ciphertext) {
    gmp_randstate_t state;
    gmp_randinit_default(state);

    // Calculate the maximum block size based on the number of bits in p
    int blockSize = (mpz_sizeinbase(p.get_mpz_t(), 2) - 1) / 8;

    // Process the plaintext in blocks
    for (size_t i = 0; i < plaintext.size(); i += blockSize) {
        // Extract a block of plaintext
        string block = plaintext.substr(i, blockSize);

        // Convert the block to its numeric representation
        mpz_class m(0);
        for (char c : block) {
            int val = charMapping[c];
            m = m * 37 + val; // Adjust range to 0-36
        }

        // Choose a random value k
        mpz_class k;
        mpz_urandomm(k.get_mpz_t(), state, p.get_mpz_t());

        // Compute the ciphertext pair (c1, c2)
        mpz_class c1, c2;
        mpz_powm(c1.get_mpz_t(), g.get_mpz_t(), k.get_mpz_t(), p.get_mpz_t());
        mpz_powm(c2.get_mpz_t(), y.get_mpz_t(), k.get_mpz_t(), p.get_mpz_t());
        c2 *= m; // Multiply by plaintext block's numeric value
        c2 %= p;

        // Store the ciphertext pair
        ciphertext.emplace_back(c1, c2);
    }

    // Clear memory
    gmp_randclear(state);
}
int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <plaintext>" << endl;
        return 1;
    }

    // Initialize GMP random state
    gmp_randstate_t state;
    gmp_randinit_default(state);

    // Generate ElGamal key pairs
    mpz_class p, g, x, y;
    generateElGamalKeys(p, g, x, y);

    string plaintext = "";
    for (int i = 1; i < argc; ++i) {
        plaintext += argv[i];
        if (i < argc - 1)
            plaintext += " ";
    }

    vector<pair<mpz_class, mpz_class>> ciphertext;
    encryptElGamal(plaintext, p, g, y, ciphertext);

    // Output ciphertext
    for (const auto& val : ciphertext) {
        cout << val.first << " " << val.second << " ";
    }
    cout << endl;

    // Clear memory
    gmp_randclear(state);

    return 0;
}
