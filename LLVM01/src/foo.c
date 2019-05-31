int foo(int n, int m) {
  int var1 = 0;
  int c0;
  for (c0 = n; c0 > 0; c0--) {
    int c1 = m;
    for (; c1 > 0; c1--) {
      var1 += c0 > c1 ? 1 : 0;
    }
  }
  return var1;
}

int main() {
  foo(3, 4);
  return 0;
}
