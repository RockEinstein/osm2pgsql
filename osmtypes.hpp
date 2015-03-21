/* Data types to hold OSM node, segment, way data */

#ifndef OSMTYPES_H
#define OSMTYPES_H

// when __cplusplus is defined, we need to define this macro as well
// to get the print format specifiers in the inttypes.h header.
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include "config.h"

#include <string>
#include <vector>
#include <cmath>

typedef int64_t osmid_t;
#define strtoosmid strtoll
#define PRIdOSMID PRId64
#define POSTGRES_OSMID_TYPE "int8"

enum OsmType { OSMTYPE_WAY, OSMTYPE_NODE, OSMTYPE_RELATION };

struct osmNode {
  double lon;
  double lat;

  osmNode() : lon(NAN), lat(NAN) {}

  osmNode(double x, double y) : lon(x), lat(y) {}
};

typedef std::vector<osmNode> nodelist_t;
typedef std::vector<nodelist_t> multinodelist_t;

struct member {
  OsmType type;
  osmid_t id;
  std::string role;

  member(OsmType t, osmid_t i, const std::string &r) : type(t), id(i), role(r) {}
};

typedef std::vector<member> memberlist_t;

struct tag {
  std::string key;
  std::string value;

  tag(const std::string &k, const std::string &v) : key(k), value(v) {}
};


class taglist_t : public std::vector<tag> {

  typedef std::vector<tag> base_t;

public:
  const tag *find(const std::string &key) const
  {
    for (base_t::const_iterator it = begin() ; it != end(); ++it)
      if (it->key == key)
        return &(*it);

    return 0;
  }

  int indexof(const std::string &key) const
  {
      for (int i = 0; i < size(); ++i)
          if (at(i).key == key)
              return i;

      return -1;
  }

  const std::string *get(const std::string &key) const
  {
    for (base_t::const_iterator it = begin() ; it != end(); ++it)
      if (it->key == key)
        return &(it->value);

    return 0;
  }

  bool get_bool(const std::string &key, bool defval) const
  {
    for (base_t::const_iterator it = begin() ; it != end(); ++it)
      if (it->key == key) {
          if (!defval &&
              (it->value == "yes" || it->value == "true" || it->value == "1"))
              return true;
          if (defval &&
              (it->value == "no" || it->value == "false" || it->value == "0"))
              return false;
          return defval;
      }

    return defval;
  }

  void push_dedupe(const tag& t)
  {
      if (find(t.key) == 0)
          push_back(t);
  }

  bool contains(const std::string &key) const { return find(key) != 0; }
};

typedef std::vector<taglist_t> multitaglist_t;

typedef std::vector<osmid_t> idlist_t;

typedef std::vector<const std::string *> rolelist_t;

#endif
