  
class Human {
 	init(name, age, occupation) {	// init is the name of the constructor method, it is called automatically if it exists.
 		this.name = name;
 		this.age = age;
 		this.occupation = occupation;
 	}
 
 	setName(newName) {
 		this.name = newName;	// the 'this' keyword is used to refer to the instance itself.
 	}
 
 	getName() { print this.name; } // whitespace is ignored
 	
 	getAgeIn5Years() {	// variable, function and method names are alphanumeric
 		return this.age + 5;
 	}
}

var me = Human("George", 19, "Software Engineer");

print me;
print me.name;

me.setName("sjjsjsj");

me.getName();
print me.getAgeIn5Years();
print me.name;
 

