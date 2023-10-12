// This is a comment

var name = "Erlich Bachman"; // string variable declaration
var age = 99; // number variable declaration
  
fun printCommentAboutAge(name, age) {
  // If statement
  if (age > 80) {
    var thingToPrint = name + " is old."; // string concatenation.
    print thingToPrint;	// Print statements
  } else { // else block can be omitted.
    print name + " is not old.";
  }
}

fun add(a, b) {
  var x = a + b;	// use 'return' keyword for returning from a function. (value is optional)
}
 
class Human {
 	init(name, age, occupation) {	// init is the name of the constructor method, it is called automatically if it exists.
 		this.name = name;
 		this.age = age;
 		this.occupation = occupation;
 	}
 
 	setName(newName) {
 		this.name = name;	// the 'this' keyword is used to refer to the instance itself.
 	}
 
 	getName() { return this.name; } // whitespace is ignored
 	
 	getAgeIn5Years() {	// variable, function and method names are alphanumeric
 		return add(this.age, 5);
 	}
}
 
 
var erlich = Human(name, age, "Former child"); // instantiating a class
erlich.isOld = erlich.age > 80; // property access, and new properties can be added to objects after the fact.

while (erlich.age > 80) { // while loops
	erlich.age = erlich.age - 1;
	print "Erlich is still old.";
}

print "Erlich is now young.";

for (var i = 0; i < 100; i = i + 1) {	// for loops
	print i;
}
 
print "Just counted to 100!";
 
 
