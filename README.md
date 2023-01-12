# TCP-Simple-Calculator
A simple TCP iterative server and client to evaluate arithmetic expressions

The client asks for an arithmetic expression from the user. The user enters an
arithmetic expression containing real no.s (in x.y or only x form), the binary
operators +, -, *, and /, and left and right brackets (( and )).
There may or may not be spaces before and after the
operators. For example, all of the following are valid expressions to enter: “13.0 +
42.6*5”, “10.7+2/4.6”, “5.2 + 6 - 3”. Assumed that all operators have the same
precedence and are left-associative.

The client sends the expression to the server. The arithmetic expression can be of
any length, you cannot assume any maximum length. Also, you should send only the
number of bytes needed to send the expression properly. You should not send
anything else other than the expression (‘\0’ at end if needed is ok).
The server computes the expression and sends the result (a real number) back to the
client.
The client displays the result on the screen. It then prompts the user to enter the next
expression.
The client terminates when the user enters a –1.
Assumed that the user always enters a valid expression.
