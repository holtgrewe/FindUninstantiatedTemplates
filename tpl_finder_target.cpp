template <typename T>
struct Klass
{
  template <typename U>
  void klass_bar1();

  template <typename U>
  void klass_bar2();
};

template <typename T>
template <typename U>
void Klass<T>::klass_bar2()
{
}

template <typename T>
int foo();

template <typename T>
int foo()
{
    volatile int i = 10;
    return i;
}

template <typename T>
void bar();

template <typename T>
void bar()
{
}

void baz()
{
}

template <typename T>
void onlyDeclaration();

int main()
{
  int x = foo<int>();
  return 0;
}
