#ifndef OBJECTS_FACTORY_H_INCLUDED
#define OBJECTS_FACTORY_H_INCLUDED

/*!
  \file ObjectsFactory.h
  \author gennadiy (gennadiy3.14@gmail.com)
  \brief Define a factory to create objects with arbitrary arguments
    in their create function. It's very similar to ObjectRegister factory
    but with a little bit simpler syntax (probably). Also a logger class is defined here.
*/

#include <string>
#include <functional>
#include <utility>
#include <exception>

template<class Factory> class ObjectsFactory;

/// Factory to create objects (T) with arbitrary arguments (Args...)
template<class T, class... Args> class ObjectsFactory<T(Args...)>
{
  std::string id;
  std::function<T(Args...)> fun;

  ObjectsFactory<T(Args...)> *next;
  static ObjectsFactory<T(Args...)> *begin;

public:
  ObjectsFactory() = delete;
  ObjectsFactory(ObjectsFactory &&) = delete;
  ObjectsFactory(const ObjectsFactory &) = delete;
  ObjectsFactory& operator=(ObjectsFactory &&) = delete;
  ObjectsFactory& operator=(const ObjectsFactory &) = delete;

  ObjectsFactory(std::string name, std::function<T(Args...)> f)
    : id(std::move(name)), fun(std::move(f))
  {
    if (ObjectsFactory::find(name) != nullptr)
      throw std::logic_error(
        "ObjectFactory: id \"" + name + "\" defined more than once!");

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
  static ObjectsFactory<T(Args...)>* find(const std::string &name)
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
    // TODO: are_same<Args..., Ts...>

    auto *p = ObjectsFactory::find(name);
    if (!p)
      throw std::logic_error(
        "ObjectFactory: id \"" + name + "\" is not registered in program.");
    return p->fun(std::forward<Ts>(args)...);
  }

  template<class Cont> static void dump_names(Cont &cont)
  {
    for (auto *p = begin; p; p = p->next)
      cont.emplace_back(p->id);
  }
}; // class ObjectsFactory<T(Args...)>

template<class T, class... Args>
  ObjectsFactory<T(Args...)> *ObjectsFactory<T(Args...)>::begin = nullptr;

// user deduction guide since c++17
//template<class T, class... Args>
//  ObjectsFactory(std::string, std::function<T(Args...)>) -> ObjectsFactory<T(Args...)>;

#include "MARPLE_Log_System.h"
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>

extern Log::Logger MARPLE_logger; // global MARPLE logger

namespace IO
{

  /// Default (base) logger. Do nothing because nothing to do.
  struct FactoryLogger { virtual ~FactoryLogger() = default; };

  using FactoryLoggerF = FactoryLogger();

/*---------------------------------------------------------------------------------------*/

  /// prints all registered factories together with their content.
  static void print_factories()
  {
    std::vector<std::string> names;
    ObjectsFactory<FactoryLoggerF>::dump_names(names);
    std::sort(std::begin(names), std::end(names));

    // 1. print available features groups
    std::stringstream message;
    message << "Available features groups (runtime detection):\n";
    int counter = 0;
    for (const auto &name : names)
      message << std::setw(2) << (++counter) << ". " << name << '\n';
    MARPLE_logger.notification(message.str());

    // 2. print details about each feature group
    // Details are printed in the corresponding constructors of the loggers
    // using helper function 'print_objects' defined below.
    for (const auto &name : names)
      ObjectsFactory<FactoryLoggerF>::build(name);
  }

/*---------------------------------------------------------------------------------------*/

  /// helper print function for the factory.
  template<class Factory> void print_objects(
    const std::string &factory_name,
    const std::string &factory_fullname,
    const std::string &factory_description)
  {
    std::vector<std::string> names;
    ObjectsFactory<Factory>::dump_names(names);
    std::sort(std::begin(names), std::end(names));

    std::stringstream message;
    message
      << "ObjectsFactory: \"" << factory_name << "\" (\"" << factory_fullname << "\").\n"
      << "Features group description: \"" << factory_description << "\".\n"
      << "Available features in the group (runtime detection):\n";

    int counter = 0;
    for (auto &name : names)
      message << std::setw(2) << (++counter) << ". " << name << '\n';

    MARPLE_logger.notification(message.str());
  }

} // namespace IO

#endif // OBJECTS_FACTORY_H_INCLUDED
