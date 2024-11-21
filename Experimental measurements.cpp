#include <iostream>
#include <cmath>
#include <random>
#include <fstream>
#include <iomanip>
#include <algorithm>
using namespace std;

bool isInCircle(long double x, long double y, long double xc, long double yc, long double r) {
  return (x - xc) * (x - xc) + (y - yc) * (y - yc) <= r * r;
}

bool isInIntersection(long double x, long double y,
  long double x1, long double y1, long double r1,
  long double x2, long double y2, long double r2,
  long double x3, long double y3, long double r3) {
  return isInCircle(x, y, x1, y1, r1) &&
    isInCircle(x, y, x2, y2, r2) &&
    isInCircle(x, y, x3, y3, r3);
}

long double monteCarlo(long double x1, long double y1, long double r1,
  long double x2, long double y2, long double r2,
  long double x3, long double y3, long double r3, int N) {
  long double xMin = min({ x1 - r1, x2 - r2, x3 - r3 });
  long double xMax = max({ x1 + r1, x2 + r2, x3 + r3 });
  long double yMin = min({ y1 - r1, y2 - r2, y3 - r3 });
  long double yMax = max({ y1 + r1, y2 + r2, y3 + r3 });
  random_device rd;
  mt19937 g(rd());
  uniform_real_distribution<> disX(xMin, xMax);
  uniform_real_distribution<> disY(yMin, yMax);
  int cnt = 0;
  for (int i = 0; i < N; ++i) {
    long double x = disX(g);
    long double y = disY(g);
    if (isInIntersection(x, y, x1, y1, r1, x2, y2, r2, x3, y3, r3)) {
      cnt++;
    }
  }
  long double rect = (xMax - xMin) * (yMax - yMin);
  return (static_cast<long double>(cnt) / N) * rect;
}

int main() {
  long double x1 = 1.0, y1 = 1.0, r1 = 1.0;
  long double x2 = 1.5, y2 = 2.0, r2 = sqrtl(5) / 2.0;
  long double x3 = 2.0, y3 = 1.5, r3 = sqrtl(5) / 2.0;

  const long double exactArea = 0.944517; // Точное значение площади

  ofstream results("results.csv");
  results << "N,EstimatedArea,RelativeError\n";

  for (int N = 100; N <= 100000; N += 500) {
    long double estimatedArea = monteCarlo(x1, y1, r1, x2, y2, r2, x3, y3, r3, N);
    long double relativeError = fabs(estimatedArea - exactArea) / exactArea;
    results << N << "," << estimatedArea << "," << relativeError << "\n";
    cout << "N = " << N << ", Area = " << estimatedArea << ", Error = " << relativeError << "\n";
  }
  results.close();
  cout << "Results saved to results.csv" << endl;
}
