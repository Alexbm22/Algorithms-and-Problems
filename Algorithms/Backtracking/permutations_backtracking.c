// -- This algorithm should take a natural value and return all the permutations of that value. --
// -- The algorithm should be implemented using backtracking. --

#include <stdio.h>
#define MAX_PERM_SIZE 25

void init(int* v, int pos) {
	v[pos] = 0;
}

int Successor(int* v, int pos, int n) {
	if (v[pos] < n) {
		v[pos]++;
		return 1;
	}

	return 0;
}

int	isValid(int* v, int pos) {
	for (int i = 0; i <= pos; i++) {
		for (int j = i + 1; j <= pos; j++) {
			if (v[i] == v[j]) {
				return 0;
			}
		}
	}
	return 1;
}

void PrintPermutation(int* v, int pos) {
	for (int i = 0; i <= pos; i++) {
		printf("%d", v[i]);
	}

	printf(" ");
}

void showPerms(int n) {
	int pos = 0, perm[MAX_PERM_SIZE];
	init(perm, pos);

	while (pos >= 0) {
		int isV = 0, isS = 0;
		do {
			isS = Successor(perm, pos, n);
			if (isS) {
				isV = isValid(perm, pos);
			}
		} while (isS && !isV);

		if (isS) {
			if (pos == n - 1) {
				PrintPermutation(perm, pos);
			}
			else {
				init(perm, ++pos);
			}
		}
		else {
			init(perm, pos);
			pos--;
		}
	}
}

int main() {

	int n;
	scanf("%d", &n);

	printf("Permutations for %d numbers: ", n);
	showPerms(n);

	return 0;
}