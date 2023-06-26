// Avl tree implementation

#include <iostream>
#include <string>

template <typename T>
class AvlTree {
 public:
  T x;
  AvlTree<T>* left;
  AvlTree<T>* right;
  short height;
  int child;
  AvlTree(T x) {
    this->x = x;
    this->left = nullptr;
    this->right = nullptr;
    this->height = 1;
    this->child = 1;
  }
  ~AvlTree() {
    delete this->right;
    delete this->left;
  }
  int GetHeight(AvlTree<T>* t) { return t == nullptr ? 0 : t->height; }
  int CountChild(AvlTree<T>* t) { return t == nullptr ? 0 : t->child; }
  int BalanceFact(AvlTree<T>* t) {
    if (t == nullptr) {
      return 0;
    }
    return GetHeight(t->right) - GetHeight(t->left);
  }
  void FixHeight(AvlTree<T>* t) {
    int left_height = GetHeight(t->left);
    int right_height = GetHeight(t->right);
    t->height =
        (left_height > right_height ? left_height + 1 : right_height + 1);
  }
  void FixChild(AvlTree<T>* t) {
    int left_child = CountChild(t->left);
    int right_child = CountChild(t->right);
    t->child = left_child + right_child + 1;
  }
  AvlTree<T>* RotateR(AvlTree<T>* t) {
    AvlTree<T>* q = t->left;
    t->left = q->right;
    q->right = t;
    FixHeight(t);
    FixHeight(q);
    FixChild(t);
    FixChild(q);
    return q;
  }
  AvlTree<T>* RotateL(AvlTree<T>* t) {
    AvlTree<T>* q = t->right;
    t->right = q->left;
    q->left = t;
    FixHeight(t);
    FixHeight(q);
    FixChild(t);
    FixChild(q);
    return q;
  }
  AvlTree<T>* BalanceTree(AvlTree<T>* t) {
    FixHeight(t);
    FixChild(t);
    if (BalanceFact(t) == 2) {
      if (BalanceFact(t->right) < 0) {
        t->right = RotateR(t->right);
      }
      return RotateL(t);
    }
    if (BalanceFact(t) == -2) {
      if (BalanceFact(t->left) > 0) {
        t->left = RotateL(t->left);
      }
      return RotateR(t);
    }
    return t;
  }
  AvlTree<T>* Insert(AvlTree<T>* t, T& x) {
    if (t == nullptr) {
      return new AvlTree(x);
    }
    t->child += 1;
    if (x.first < (t->x).first) {
      t->left = Insert(t->left, x);
    } else {
      t->right = Insert(t->right, x);
    }
    return BalanceTree(t);
  }
  AvlTree<T>* Find(AvlTree<T>* t, std::string& x) {
    if (x < t->x.first) {
      return Find(t->left, x);
    }
    if (x > t->x.first) {
      return Find(t->right, x);
    }
    return t;
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n;
  int q;
  std::cin >> n;
  std::string x;
  std::string y;
  std::cin >> x >> y;
  AvlTree<std::pair<std::string, std::string>>* avl =
      new AvlTree<std::pair<std::string, std::string>>(std::make_pair(x, y));
  std::pair<std::string, std::string> p = std::make_pair(y, x);
  avl->Insert(avl, p);
  for (int i = 0; i != n - 1; ++i) {
    std::cin >> x >> y;
    p = std::make_pair(x, y);
    avl = avl->Insert(avl, p);
    p = std::make_pair(y, x);
    avl = avl->Insert(avl, p);
  }
  std::cin >> q;
  for (int i = 0; i != q; ++i) {
    std::cin >> x;
    std::cout << (avl->Find(avl, x)->x).second << "\n";
  }
  delete avl;
}
