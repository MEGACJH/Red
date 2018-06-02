##########################################################################
#                                                                                                                             #
#                                              This is comments                                                      #
#                                                                                                                             #
##########################################################################

# The real seperator of every statment is `\n`, but I've found that it easy to make program a mess
# So I add ";"symbol to resolve it

# Define a string object whose label is `name`
name = "Justin"; # Define a string object

# Define a int object whose label is `age`
age = 22;

# This echon is a keyword not a builtin function, it means `echo no new line`
# It has the ability to call the description function of the object to print the string message
# The statment after `echon` can be combination of string, int and float objects in this 0.1.1 version
# Recommend: Use<< string object or int object >>this form when you need print both string, int and float object,
# This form supported aim to make the echon statment clearer
# NOTE!! there are at least two blank space  among `<<`, name and `>>`
# In fact, you can also use blank space or comma to seperate `echon` statment
echon "My name is " << name >> ", I'm " << age >> " years old now!";

# This `echo` keyword has the same function as `echon`, but it will automatically print a new line
echo "";

# Following is the example of if elif and else statment

# A binary operator expression is available, such as a == 8, a != 8, a <= 8 or a >= 8 and so on,
# but unary operator expression is not tested, maybe it doesn't work :(.
# After the expression, a `;` and a '\n' are expected to followed. The `do` keyword should appear
if age == 10;
do;
	echo "Well, age equals 10";
elif age < 10;
do;
	echo "Well, age is less than 10";
elif age > 10;
do;
	echo "Well, age is greater than 10";
else;
do;
	echo "No, age is not a positive number";
# The end symbol of if elif and else statment is `done` keyword
done;

# Example for `+`, `-`
a = 10 + 20;
b = 20 + -10;
echo a;
echo b;
echo a + b;
# echo a - b; has bug


str1 = "Hello, "
str2 = "World!"
str3 = str1 + str2;
echo str3;

echo str1 == str2;
echo str1 == str1;

# Interact with user
echo "Please enter your name:";
name = input;
echon "";
echo "Please enter your age:";
age = input;
echon "";
# Change sstring a to int a
age = toint age;
echon "Your name is " << name >> ", and your age is " << age >>;
echo "";
echon "I will be " << age + 1 >> "in the next year";
