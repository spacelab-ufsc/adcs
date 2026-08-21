# Contributing

Guidelines for writing embedded C++ in this project (RTEMS 6, `arm-rtems6-stm32f4` BSP).

## Toolchain

RTEMS is a build dependency, not part of this repo — don't vendor or clone RTEMS source
here. Build/install it once elsewhere and point the build at it via env vars:

```sh
export RTEMS_PREFIX=/opt/rtems/6
export RTEMS_PKG_CONFIG_PATH=/opt/rtems/6/lib/pkgconfig
cmake -S . -B out -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-arm-rtems6.cmake
cmake --build out
```

Never commit `out/` (or any build directory) — it's machine-specific and already
gitignored.

## File organization

- Headers live next to their `.cpp` (e.g. `pus/pus_crc.h` beside `pus/pus_crc.cpp`) — no
  separate `include/` tree. This is an application, not a library other repos link
  against.
- One file = one purpose. Group a cohesive subsystem's files in one directory (e.g.
  `pus/`); split unrelated things (different peripheral drivers) into their own
  subdirectories under `drivers/`.
- Every header needs an include guard:
  ```cpp
  #ifndef FOO_H
  #define FOO_H
  ...
  #endif /* FOO_H */
  ```

## `extern "C"`

Default to normal C++ linkage. Only reach for `extern "C"` where a declaration crosses a
real C boundary — something looks the symbol up *by name* from C code. The only case in
this codebase is `rtems/init.cpp`'s `Init` task: RTEMS's `confdefs.h` builds a C-linked
table that references it by name. Passing a function's address as a callback (a task
entry, an ISR handler) never needs it.

When you do need it, guard it so the header still compiles as plain C:

```cpp
#ifdef __cplusplus
extern "C" {
#endif
...
#ifdef __cplusplus
}
#endif
```

## Comments

- `/* */` for file documentation and section dividers. Use Doxygen (`@brief`, `@param`,
  `@return`) on declarations you're actually documenting — don't invent docs where none
  exist.
- `//` is fine for short inline notes and disabled code.

## Naming & error codes

- Prefix functions with their module (`pus_send_ack`, not `send_ack`) so a symbol reads
  as belonging to its subsystem.
- Never return bare magic numbers for status/error codes — name them in an enum next to
  the function's declaration (see `pus_parse_status` in `pus/pus_parser.h`).

## Formatting

A `.clang-format` is checked in — run it on new files. Don't blanket-reformat existing
files that use hand-aligned multi-line calls; it tends to make them less readable.

## Embedded C++ constraints

The build disables exceptions and RTTI (`-fno-exceptions -fno-rtti` in
`CMakeLists.txt`) — don't write code that needs either.
