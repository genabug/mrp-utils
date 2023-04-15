#ifndef OBJECTS_FACTORY_H_INCLUDED
#define OBJECTS_FACTORY_H_INCLUDED

/*!
  \file ObjectsFactory.h
  \author gennadiy
  \brief Define a factory to create objects with arbitrary arguments
    in their create function. It's very similar to ObjectRegister factory
    but with a little bit simpler syntax (probably). Also a logger class is defined here.
*/

#include <string>
#include <functional>
#include <utility>
#include <stdexcept>

template<class Factory> class ObjectsFactory;

/// Factory to create objects (T) with arbitrary arguments (Args...)
template<class T, class... Args> class ObjectsFactory<T(Args...)>
{
  std::string id;
  std::function<T(Args...)> fun;

  ObjectsFactory *next = nullptr;
  inline static ObjectsFactory *begin = nullptr;

public:
  ObjectsFactory() = delete;
  ObjectsFactory(ObjectsFactory &&) = delete;
  ObjectsFactory(const ObjectsFactory &) = delete;
  ObjectsFactory& operator=(ObjectsFactory &&) = delete;
  ObjectsFactory& operator=(const ObjectsFactory &) = delete;

  ObjectsFactory(std::string name, std::function<T(Args...)> f)
    : id(std::move(name)), fun(std::move(f))
  {
    if (ObjectsFactory::find(id) != nullptr)
      throw std::logic_error(
        "ObjectFactory: id \"" + id + "\" defined more than once!");

    next = begin;
    begin = this;
  }

  ~ObjectsFactory()
  {
    auto **p = &begin;
    for (; *p != this; p = &((*p)->next));
    *p = next;
  }

  /// Find and return the object registered with label "id" or nullptr if not found.
  static ObjectsFactory* find(const std::string &name)
  {
    for (auto *p = begin; p; p = p->next)
      if (p->id == name)
        return p;
    return nullptr;
  }

  /// Build the object registered with label "id".
  template<class... Ts>
    static T build(const std::string &name, Ts&&... args)
  {
    static_assert(
      sizeof...(Args) == sizeof...(Ts) &&
      (std::is_same_v<Args, Ts> && ...), "arguments aren't match");

    auto *p = ObjectsFactory::find(name);
    if (!p)
      throw std::runtime_error(
        "ObjectsFactory: id \"" + name + "\" is not registered.");
    return p->fun(std::forward<Ts>(args)...);
  }

  template<class Cont> static void dump_names(Cont &cont)
  {
    for (auto *p = begin; p; p = p->next)
      cont.emplace_back(p->id);
  }
}; // class ObjectsFactory<T(Args...)>

template<class T, class... Args>
  ObjectsFactory(std::string, std::function<T(Args...)>) -> ObjectsFactory<T(Args...)>;


#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace IO
{

  /// Default (base) logger. Do nothing because nothing to do.
  struct FactoryLogger {};

  using FactoryLoggerF = FactoryLogger(std::ostream &out);

  /// helper function for text representation of a specific factory
  /// should be called from the factory logger's contstructor
  template<class Factory> void print_factory(
    std::ostream &message, const std::string &name, const std::string &description)
  {
    std::vector<std::string> names;
    ObjectsFactory<Factory>::dump_names(names);
    std::sort(std::begin(names), std::end(names));

    message
      << "Factory: \"" << name << "\".\n"
      << "Description: \"" << description << "\".\n"
      << "Available object(s):\n";

    int counter = 0;
    for (auto &name : names)
      message << std::setw(3) << (++counter) << ". " << name << '\n';
  }

} // namespace IO

#endif // OBJECTS_FACTORY_H_INCLUDED
