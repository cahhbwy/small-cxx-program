#include <iostream>
#include <string>
#include <ctime>
using namespace std;
#define max(a,b) (((a) > (b)) ? (a) : (b))
int naive(string haystack, string needle) {
	int n = haystack.length();
	int m = needle.length();
	if (m == 0)	return 0;
	if (n == 0)	return -1;
	int count = 0;
	int i, j;
	for (i = 0; i <= n - m; ++i) {
		j = 0;
		while (j < m) {
			if (haystack[i + j] == needle[j]) {
				++j;
			} else {
				break;
			}
		}
		if (j >= m) {
			//			return i;
			cout << " " << i;
			count++;
		}
	}
	return count;
}
int kmp(string haystack, string needle) {
	int m = needle.length();
	int n = haystack.length();
	if (m == 0)	return 0;
	if (n == 0)	return -1;
	int count = 0;
	//next
	int *next = new int[m];
	int i = 0, j = -1;
	next[0] = -1;
	while (i < m - 1) {
		if (j == -1 || needle[j] == needle[i]) {
			++i;
			++j;
			next[i] = j;
		} else {
			j = next[j];
		}
	}
	//kmp
	i = 0;
	j = 0;
	while (i < n) {
		while (i < n && j < m) {
			if (j == -1 || haystack[i] == needle[j]) {
				++i;
				++j;
			} else {
				j = next[j];
			}
		}
		if (j == m) {
			cout << " " << i - j;
			++count;
			if (i < n) {
				if (m > 1) {
					--i;
				}
				j = next[j - 1];
				j = (j < 0 ? 0 : j);
			}
		}
	}
	return count;
}
int bm(string haystack, string needle) {
	int m = needle.length();
	int n = haystack.length();
	if (m == 0)	return 0;
	if (n == 0)	return -1;
	int count = 0, i, j, k;
	//bad charater
	int bmBc[256];
	for (i = 0; i < 256; ++i) {
		bmBc[i] = m;
	}
	const char *c = needle.c_str();
	i = m;
	while (i > 1) {
		bmBc[*c++] = --i;
	}
	//suffix
	int *suff = new int[m];
	suff[m - 1] = m;
	j = m - 1;
	k = 0;
	for (i = m - 2; i >= 0; --i) {
		if (i > j&&suff[i + m - 1 - k] < i - j) {
			suff[i] = suff[i + m - 1 - k];
		} else {
			if (i < j) {
				j = i;
			}
			k = i;
			while (j >= 0 && needle[j] == needle[j + m - 1 - k]) {
				--j;
			}
			suff[i] = k - j;
		}
	}
	//good suffix
	int *bmGs = new int[m];
	for (i = 0; i < m; ++i) {
		bmGs[i] = m;
	}
	j = 0;
	for (i = m - 1; i >= 0; --i) {
		if (suff[i] == i + 1) {
			while (j < m - 1 - i) {
				if (bmGs[j] == m) {
					bmGs[j] = m - 1 - i;
				}
				++j;
			}
		}
	}
	for (i = 0; i <= m - 2; ++i) {
		bmGs[m - 1 - suff[i]] = m - 1 - i;
	}
	//Boyer-Moore
	j = 0;
	while (j <= n - m) {
		for (i = m - 1; i >= 0 && needle[i] == haystack[i + j]; --i);
		if (i < 0) {
			cout << " " << j;
			++count;
			j += bmGs[0];
		} else {
			j += max(bmBc[haystack[i + j]] - m + 1 + i, bmGs[i]);
		}
	}
	return count;
}
int sunday(string haystack, string needle) {
	int m = needle.length();
	int n = haystack.length();
	if (m == 0)	return 0;
	if (n == 0)	return -1;
	int count = 0, i, j;
	//bad charater
	int bc[256];
	for (i = 0; i < 256; ++i) {
		bc[i] = m;
	}
	const char *c = needle.c_str();
	i = m;
	while (i > 0) {
		bc[*c++] = i--;
	}
	j = 0;
	while (j <= n - m) {
		i = 0;
		while (i < m&&haystack[j + i] == needle[i]) {
			++i;
		}
		if (i >= m) {
			cout << " " << j;
			++count;
		}
		if (j >= n - m) {
			break;
		}
		j += bc[haystack[j + m]];
	}
	return count;
}
string generateHaystack() {
	int length = rand() & 0xFF;
	char *haystack = new char[length + 1];
	haystack[length] = 0;
	for (int i = 0; i < length; ++i) {
		haystack[i] = 'a' + (rand() % 5);
	}
	return haystack;
}
string generateNeedle() {
	int length = rand() & 0x07;
	char *needle = new char[length + 1];
	needle[length] = 0;
	for (int i = 0; i < length; ++i) {
		needle[i] = 'a' + (rand() & 0x03);
	}
	return needle;
}
int main(int argc, char **argv) {
	srand((unsigned int)time(NULL));
	for (int i = 0; i < 100; ++i) {
		string haystack = generateHaystack();
		string needle = generateNeedle();
		cout << "haystack\t" << haystack << endl;
		cout << "needle\t\t" << needle << endl;
		cout << "naive:\t";
		cout << "\t[" << naive(haystack, needle) << "]" << endl;
		cout << "kmp:\t";
		cout << "\t[" << kmp(haystack, needle) << "]" << endl;
		cout << "bm:\t";
		cout << "\t[" << bm(haystack, needle) << "]" << endl;
		cout << "Sunday:\t";
		cout << "\t[" << sunday(haystack, needle) << "]" << endl;
	}
	return 0;
}
