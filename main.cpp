#include <iostream>
#include <set>
#include <string>
#include <tuple>

using namespace std;

/* ------------------------------------------------ */
/* Tuple Helper Functions                           */
/* ------------------------------------------------ */
template <unsigned... s>
struct seq {
  typedef seq<s...> type;
};
template <unsigned max, unsigned... s>
struct make_seq : make_seq<max - 1, max - 1, s...> {};
template <unsigned... s>
struct make_seq<0, s...> : seq<s...> {};

template <unsigned... s, typename Tuple>
auto extract_tuple(seq<s...>, Tuple& tup) {
  return make_tuple(get<s>(tup)...);
}
/* ------------------------------------------------ */

template <typename... types>
struct Relation {
  using OutputType = tuple<types...>;
  array<string, sizeof...(types)> schema;
  set<OutputType> data;

  Relation(){};
  Relation(array<string, sizeof...(types)> schema)
      : schema(schema), data(set<OutputType>()){};
  Relation(array<string, sizeof...(types)> schema, set<OutputType> data)
      : schema(schema), data(data){};
};

template <typename... types>
struct Operator : public Relation<types...> {
  Operator(){};
  Operator(array<string, sizeof...(types)> schema)
      : Relation<types...>(schema){};
};

template <typename InputOperator, typename... types>
struct Project : public Operator<types...> {
  using OutputType = tuple<types...>;

  InputOperator input;
  set<pair<string, string>> projections;

  Project(InputOperator input, set<pair<string, string>> projections)
      : input(input), projections(projections) {
    array<string, sizeof...(types)> schema = {};
    set<OutputType> data;

    /* Define new schema based on given projection and keep the
       corresponding columns. */
    int i = 0;
    for (pair<string, string> projection : projections) {
      schema[i] = projection.second;
    }

    /* Set parent struct attributes according to projection. */
    this->schema = schema;
    this->data = data;
  };
};

int main() {
  Relation<int, string> users({"id", "name"}, {
                                                  make_tuple(1, "lucas"),
                                                  make_tuple(2, "holger"),
                                              });

  Relation<string> usernames =
      Project<decltype(users), string>(users, {{"name", "username"}});

  tuple<int, string, int> my_tup = make_tuple(1, "lucas", 52);
  int i = 2;
  auto skip_2nd = extract_tuple(seq<0, 2>(), my_tup);
  cout << get<1>(skip_2nd) << "\n";

  return 0;
}