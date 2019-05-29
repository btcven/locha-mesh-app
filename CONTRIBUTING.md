
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
        * [Include statements](#include-statements)
        * [Naming rules](#naming-rules)
        * [Indentation](#indentation)
    * [Documentation styleguide](#documentation-styleguide)




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

#### Include statements

Include statements must be located at the top of the file **only**. By default this statement will go in the .cpp files, not in header files, except when necessary and it should be sorted and grouped.

#### Naming rules

- Use a descriptive name and be consistent in style when write code
- All names should be written in English

**Macros** Use uppercase and underscore
```cpp

#define LOW_NIBBLE(x) (x & 0xf)
```

**Variable names** Use underscore, dont't group variables by types
```cpp
// GOOD, underscore
int rssi_level;
int snr_level;

// BAD, Grouped
int rssi_level, snr_level;

// BAD, CamelCase
int RssiLevel;
int SnrLevel;

// BAD, mixed case
int rssiLevel;
int snrLevel;
```

**Methods or functions**  Use descriptive verbs and mixed case starting with lower case.

 ```cpp
 int getTotalNearestNodes();
```

**Classes** Use CamelCase
```cpp
class SomeClass { 
    public:
        ...
    private:
        int foo_; // Private variables ends with an underscore
};
```
### Common statements

#### if / if-else

- Always put braces around the if-else statement or when is nested in another if statement
- Put space between `if` and `()`


```cpp
// if-else or nested if-else statements ALWAYS put braces
if (foo)
{
    if(foo == 1)
    {
        bar = UP;
    }
    else 
    {
        bar = DOWN;
    }

}
else 
{
    bar = MIDDLE;
}

// only if statement
if (foo)
    bar = UP;
```
#### while / do-while loops

- Put space between `while` and `()`
```cpp
// while statement
while (foo > 0)
{
    bar++;
}

// do-while statement
do
{
    bar++;
}
while (foo > 0);

```
### Indentation

- Do not use tabs
- Use 4 spaces as default identation

### Documentation styleguide
...

