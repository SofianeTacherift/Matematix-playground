# Matematix-playground

Matematix-playground is a mathematics-oriented high-level language written in C. It is compiled to bytecode and is inspired by Python.

## Warning

This project is not ready yet and is still in development. It is a project to learn how we transform raw text into binary instructions.

## Features

* Compilation:

    * Constant numbers (`1`, `2902`, ...)
    * Assignments:

      ```python
      a = 60;
      ```
    * Binary operators:

        * Addition: `+`
        * Subtraction: `-`
        * Multiplication: `*`
        * Division: `/`
        * Power: `^`
        * Logical OR: `|`
        * Logical AND: `&`
        * Unary minus
        * Logical NOT: `!`
        * Comparison operators: `==`, `!=`, `>`, `<`, `<=`, `>=`
    * `if` statements
    * `while` loops

## How to try it

Clone the repository:

```bash
git clone https://github.com/SofianeTacherift/Matematix-playground/
cd Matematix-playground/
```

Build the project:

```bash
mkdir -p build
cmake -S . -B build
cmake --build build
```

Test the compilation:

```bash
build/compilation_test.out <file>
```

The file must be located in `resources/mxp_files/`.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
