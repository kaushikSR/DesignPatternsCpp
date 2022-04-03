// open closed principle

/* - Open for extension, closed for modification
   - A class is open if you can extend it, produce a subclass 
     and do whatever you want with it—add new methods or fields, 
     override base behavior, etc. Some programming languages let 
    you restrict further extension of a class with special keywords, 
    such as final . After this, the class is no longer open. 
    At the same time, the class is closed (you can also say complete) 
    if it’s 100% ready to be used by other classes—
    its interface is clearly defined and won’t be changed in the future.

    In the following example, we want to be able to filter products 
    based on some parameters like size, color etc

    We can have a product filter class that includes all of these
    filters but this is going to lead to code bloat because of the number
    of combinations possible.

    So, we use composition to solve this problem. We can make this behavior
    into an abstract class i.e the Specification 
    class below which accepts a template parameter (any item) 
    with a pure virtual function "isSatisfied" 
    that should check if the item passed in satisfies the filter or not. 

    The concrete classes representing the specific behaviours like size, color etc
    will implement this abstract class and the pure virtual function "isSatisfied"

    We also create an abstract filter class that has a pure virtual function 
    called filter that accepts a vector of templated items and the Specification that needs
    to be used to filter.

    The main concrete filter class inherits from this abstract filter class and implements
    the filter pure virtual method to iterate through the items and call isSatisfied on the
    specification thereby producing a vector of items that pass the filter test. 

    Now we are able to extend the specification behaviour to any number/combination by simply
    creating concrete classes of the Specification abstract class, instead of having to modify
    the filter class all the time.

    This is the Strategy Pattern. Where we extract out the behaviour that could
    potentially be extended in the future into a seperate class of its
    own and compose it in the other abstract class where the behavior is needed. 
    This other class is made abstract - this enables us to have multiple concrete classes
    that can have their own algorithm to handle the behavior field. - in this example, 
    we could have a different concrete filter class where the filter algorithm could 
    be different.  
*/
#include <string>
#include <vector>
#include <iostream>
using namespace std;

enum class Color { red, green, blue };
enum class Size { small, medium, large };

struct Product
{
  string name;
  Color color;
  Size size;
};

struct ProductFilter
{
  typedef vector<Product*> Items;

  Items by_color(Items items, const Color color)
  {
    Items result;
    for (auto& i : items)
      if (i->color == color)
        result.push_back(i);
    return result;
  }

  Items by_size(Items items, const Size size)
  {
    Items result;
    for (auto& i : items)
      if (i->size == size)
        result.push_back(i);
    return result;
  }

  Items by_size_and_color(Items items, const Size size, const Color color)
  {
    Items result;
    for (auto& i : items)
      if (i->size == size && i->color == color)
        result.push_back(i);
    return result;
  }
};

template <typename T> struct AndSpecification;

template <typename T> struct Specification
{
  virtual ~Specification() = default;
  virtual bool is_satisfied(T* item) const = 0;

  AndSpecification<T> operator&&(Specification<T>& other)
  {
    return AndSpecification<T>(*this, other);
  }
};

template <typename T> struct Filter
{
  virtual vector<T*> filter(vector<T*> items,
                            Specification<T>& spec) = 0;
};

struct BetterFilter : Filter<Product>
{
  vector<Product*> filter(vector<Product*> items,
                           Specification<Product> &spec) override
  {
    vector<Product*> result;
    for (auto& p : items)
      if (spec.is_satisfied(p))
        result.push_back(p);
    return result;
  }
};

/*Color specification*/

struct ColorSpecification : Specification<Product>
{
  Color color;

  ColorSpecification(Color color) : color(color) {}

  bool is_satisfied(Product *item) const override {
    return item->color == color;
  }
};

/*Size specification*/

struct SizeSpecification : Specification<Product>
{
  Size size;

  explicit SizeSpecification(const Size size)
    : size{ size }
  {
  }


  bool is_satisfied(Product* item) const override {
    return item->size == size;
  }
};

/*Combining two specifications */

template <typename T> struct AndSpecification : Specification<T>
{
  const Specification<T>& first;
  const Specification<T>& second;

  AndSpecification(const Specification<T>& first, const Specification<T>& second) 
    : first(first), second(second) {}

  bool is_satisfied(T *item) const override {
    return first.is_satisfied(item) && second.is_satisfied(item);
  }
};

int main()
{
  Product apple{"Apple", Color::green, Size::small};
  Product tree{"Tree", Color::green, Size::large};
  Product house{"House", Color::blue, Size::large};

  const vector<Product*> all { &apple, &tree, &house };

  BetterFilter bf;
  ColorSpecification green(Color::green);
  auto green_things = bf.filter(all, green);
  for (auto& x : green_things)
    cout << x->name << " is green\n";


  SizeSpecification large(Size::large);
  AndSpecification<Product> green_and_large(green, large);

  auto spec = green && large;
  for (auto& x : bf.filter(all, spec))
    cout << x->name << " is green and large\n";

  // use the operator instead
  auto spec2 = green && large;
  for (auto& x : bf.filter(all, spec2))
    cout << x->name << " is green and large\n";

  getchar();
  return 0;
}