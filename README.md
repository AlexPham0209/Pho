# Pho
This is a basic tree-walk interpreter I wrote in C++.  It has the basic programming language features such as if statements, for and while loops, functions, and variables.  However, other features such as switch statements, objects, and arrays have not been implemented yet.

## How to Run Pho
Go to the folder the Pho executable is at.

Open the command console.

Enter the following command:
./Pho Filename.txt

## Syntax

### Print
```
print "Hello World"
```

### Variable 
```
set a = 5
```

### Function Declaration
```
fun test[a] {
  ret a
}
```

### Function Call
```
test[a]
```

### If-Else Statements
```
if [a == 2]
  print "dude"
else
  print "Where"
