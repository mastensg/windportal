#ifndef PLATFORM_H
#define PLATFORM_H

#include <vector>

#include "sl.h"

namespace platform {

struct Internal;

//////////////////////////////////////////////////////////////////////////////

enum struct Key {
  escape,
  f1,
  f2,
  f3,
  f4,
  f5,
  f6,
  f7,
  f8,
  f9,
  f10,
  f11,
  f12,
  print_screen,
  scroll_lock,
  pause_break,

  backtick,
  one,
  two,
  three,
  four,
  five,
  six,
  seven,
  eight,
  nine,
  zero,
  minus,
  equals,
  backspace,
  insert,
  home,
  page_up,

  tab,
  q,
  w,
  e,
  r,
  t,
  y,
  u,
  i,
  o,
  p,
  bracket_left,
  bracket_right,
  backslash,
  del,
  end,
  page_down,

  caps_lock,
  a,
  s,
  d,
  f,
  g,
  h,
  j,
  k,
  l,
  semicolon,
  quote,
  carriage_return,

  shift_left,
  z,
  x,
  c,
  v,
  b,
  n,
  m,
  comma,
  period,
  slash,
  shift_right,
  left,

  control_left,
  windows_left,
  alt,
  space,
  alt_gr,
  windows_right,
  menu,
  control_right,
  right,
  up,
  down,

  unknown,
};

struct KeyDownEvent {
  Key key;
};

struct KeyRepeatEvent {
  Key key;
};

struct KeyUpEvent {
  Key key;
};

enum struct EventType {
  key_down,
  key_repeat,
  key_up,
};

struct Event {
  enum EventType type;

  union {
    KeyDownEvent key_down;
    KeyRepeatEvent key_repeat;
    KeyUpEvent key_up;
  };
};

//////////////////////////////////////////////////////////////////////////////

struct Platform {
  Internal *internal = nullptr;
  bool im_on = true;
  sl::ivec2 viewport_size = {};
  std::vector<Event> events = {};
};

void reload_init(Platform *p);
void reload_begin(Platform *p);
bool reload_end(Platform *p);

void frame_begin(Platform *p);
bool frame_end(Platform *p);

void exit(Platform *p, int status);
double time(Platform *p);

}  // namespace platform

#endif  // PLATFORM_H
