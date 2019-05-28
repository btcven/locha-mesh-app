
# Contributing to **Locha**
Thanks for taking the time to contribute!

The following is a set of guidelines for contributing to [**Locha**](), [**Turpial**]() or [**Harpia**]().

#### Table of contents

* [Before starting](#before-starting)

* [How can i contribute?](#how-can-i-contribute)
    * [I just have a question](#i-just-have-a-question)
    * [Suggesting **enhancements**](#suggesting-enhancements)
    * [Reporting **bugs**](#reporting-bugs)
    * [Pull request](#pull-request)

* [Styleguide](#styleguide)
    * [Commit messages](#commit-messages)
    * [Code styleguide](#code-styleguide)
        * [Include guards](#include-guards)
        * [#include in local header files](#include-in-local-header-files)
    * [Documentation styleguide](#documentation-styleguide)



>

## Before starting
...

## How can i contribute?
...

### I just have a question
...

### Suggesting enhancements
...

### Reporting bugs
...

### Pull request
...

## Styleguide

### Commit messages
...

### Code styleguide

#### Include guards
Local header files must contain an distinctly named include guard to avoid problems with including the same header multiple times, for example:
```cpp
// file: foo.h
#ifndef FOO_H
#define FOO_H
...
#endif // FOO_H
```

#### Use of #include in local header files
Don't use #include in header files if possible.

By default ```#include``` will go in the .cpp files and not in the header files, except when necessary. 



### Documentation styleguide
...

