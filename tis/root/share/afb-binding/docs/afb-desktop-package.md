# Desktop packages for bindings development

Packages of the ***bindings*** (afb-binding) exist
for common desktop linux distributions.

- Fedora
- Ubuntu
- Debian
- Suse

Installing the development package
allows to write ***bindings*** that runs on the desktop
computer of the developer.

It is very convenient to quickly write and debug a binding.

## Retrieving compiling option with pkg-config

It is provided a configuration
file for **pkg-config**.
Typing the command

```bash
pkg-config --cflags afb-binding
```

Print flags use for compilation:

```bash
$ pkg-config --cflags afb-binding
-I/opt/local/include -I/usr/include/json-c
```

For linking, you should use

```bash
$ pkg-config --libs afb-binding
-ljson-c
```

It automatically includes the dependency to json-c.
This is activated through **Requires** keyword in pkg-config.
