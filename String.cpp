// std::string implementaion

#include <algorithm>
#include <iostream>
#include <cstring>

class String {
 public:
  char* arr;
  size_t sz;
  size_t cap;
 public:
  String(size_t n, char c): arr(new char[n + 1]), sz(n), cap(n) {
    std::fill(arr, arr + n, c);
    arr[n] = '\0';
  }

  String(const char* c) {
    if (c == nullptr) {
      arr = new char[1];
      arr[0] = '\0';
    } else {
      sz = strlen(c);
      cap = sz;
      arr = new char[sz + 1];
      //memcpy(arr, c, sz);
      std::copy(c, c + sz, arr);
      arr[sz] = '\0';
    }
  }

  String(char c): arr(new char[2]), sz(1), cap(1) {
    arr[0] = c;
    arr[sz] = '\0';
  }

  String(const String& str): arr(new char[str.sz + 1]), sz(str.sz), cap(str.sz) {
    //memcpy(arr, str.arr, sz + 1);
    std::copy(str.arr, str.arr + sz + 1, arr);
  }

  String(): arr(new char[1]), sz(0), cap(0) {
    arr[0] = '\0';
  }

  void swap(String& str) {
    std::swap(arr, str.arr);
    std::swap(sz, str.sz);
    std::swap(cap, str.cap);
  }

  String& operator=(String str) {
    swap(str);
    return *this;
  }

  ~String() {
    delete[] arr;
  }

  bool empty() {
    return sz == 0;
  }

  void clear() {
    sz = 0;
    arr[0] = '\0';
  }

  void pop_back() {
    if (sz != 0) {
      arr[sz - 1] = '\0';
      --sz;
    }
  }

  void push_back(char c) {
    if (sz != cap) {
      arr[sz] = c;
      arr[sz + 1] = '\0';
      ++sz;
    } else if (sz == 0) {
      char* arr1 = new char[3];
      delete[] arr;
      arr1[0] = c;
      arr1[1] = '\0';
      cap = 2;
      sz = 1;
      arr = arr1;
    } else {
      char* arr1 = new char[2 * sz + 1];
      //memcpy(arr1, arr, sz);
      std::copy(arr, arr + sz, arr1);
      delete[] arr;
      arr1[sz] = c;
      arr1[sz + 1] = '\0';
      arr = arr1;
      cap = 2 * sz;
      ++sz;
    }
  }

  const char& front() const {
    return arr[0];
  }

  const char& back() const {
    return arr[sz-1];
  }

  char* data() {
    return &arr[0];
  }

  const char* data() const {
    return &arr[0];
  }

  char& front() {
    return arr[0];
  }

  char& back() {
    return arr[sz-1];
  }

  size_t size() const {
    return sz;
  }

  size_t length() const {
    return sz;
  }

  size_t capacity() const {
    return cap;
  }

  const char& operator[](size_t index) const {
    return arr[index];
  }

  char& operator[](size_t index) {
    return arr[index];
  }
  //comm

  size_t find(const String& sub) const {
    size_t j = 0;
    size_t i = 0;
    while (arr[i] != '\0' && sub.arr[j] != '\0') {
      if (arr[i] == sub.arr[j]) {
        ++j;
      } else if (arr[i] == sub.arr[0]){
        j = 1;
      } else {
        j = 0;
      }
      ++i;
    }
    if (sub.arr[j] == '\0') {
      return i - j;
    }
    return sz;
  }

  size_t rfind(const String& sub) const {
    size_t j = sub.sz;
    size_t i = sz;
    while (i != 0 && j != 0) {
      if (arr[i - 1] == sub.arr[j - 1]) {
        --j;
      } else if (arr[i - 1] == sub.arr[sub.sz - 1]){
        j = sub.sz - 1;
      } else {
        j = sub.sz;
      }
      --i;
    }
    if (j == 0) {
      return i;
    } else {
      return sz;
    }
  }

  String substr(size_t start, size_t count) const {
    String s(count, '0');
    //memcpy(arr1, arr + start, count);
    std::copy(arr + start, arr + start + count, s.arr);
    return s;
  }

  void shrink_to_fit() {
    char* arr1 = new char[sz + 1];
    cap = sz;
    //memcpy(arr1, arr, sz + 1);
    std::copy(arr, arr + sz + 1, arr1);
    delete[] arr;
    arr = arr1;
  }
};

bool operator<(const String& a, const String& b) {
  size_t i = 0;
  while (a.arr[i] != '\0' && b.arr[i] != '\0') {
    if (a.arr[i] != b.arr[i]) {
      return a.arr[i] < b.arr[i];
    }
    ++i;
  }
  return a.sz < b.sz;
}

bool operator>=(const String& a, const String& b) {
  return !(a < b);
}

bool operator>(const String& a, const String& b) {
  return b < a;
}

bool operator<=(const String& a, const String& b) {
  return !(a > b);
}

bool operator==(const String& a, const String& b) {
  if (a.sz != b.sz) {
    return false;
  }
  for (size_t i = 0; i != a.sz; ++i) {
    if (a.arr[i] != b.arr[i]) {
      return false;
    }
  }
  return true;
}

bool operator!=(const String& a, const String& b) {
  return !(a == b);
}

//sdkls
//sdk
String& operator+=(String& a, const String& b) {
  if (a.cap - a.sz >= b.sz) {
    //memcpy(a.arr + a.sz, b.arr, b.sz + 1);
    std::copy(b.arr, b.arr + b.sz + 1, a.arr + a.sz);
    a.sz += b.sz;
  } else if (b.sz == 1) {
    char c = b.arr[0];
    char* arr1 = new char[2 * a.sz + 1];
    //memcpy(arr1, arr, sz);
    std::copy(a.arr, a.arr + a.sz, arr1);
    delete[] a.arr;
    arr1[a.sz] = c;
    arr1[a.sz + 1] = '\0';
    a.arr = arr1;
    a.cap = 2 * a.sz;
    ++a.sz;
  } else {
    char* arr1 = new char[2 * a.sz + 2 * b.sz + 1];
    //memcpy(arr1, a.arr, a.sz);
    //memcpy(arr1 + a.sz, b.arr, b.sz + 1);
    std::copy(a.arr, a.arr + a.sz, arr1);
    std::copy(b.arr, b.arr + b.sz + 1, arr1 + a.sz);
    delete[] a.arr;
    a.arr = arr1;
    a.cap = 2 * a.sz + 2 * b.sz;
    a.sz += b.sz;
  }
  return a;
}

String operator+(const String& a, const String& b) {
  String s("");
  s += a;
  s += b;
  return s;
}

std::istream& operator>>(std::istream& in, String& str) {
  char arr1[5000];
  in >> arr1;
  str = String(arr1);
  return in;
}


std::ostream& operator<<(std::ostream& out, const String& str) {
  out << str.arr;
  return out;
}
