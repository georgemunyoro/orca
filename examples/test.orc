
class Vector {
  init(length) {
    var emptyArray[length] = [];
    this.values = emptyArray;
    this.size = length;
  }

  at(index) {
    return this.values[index];
  }

  set(index, value) {
    return this.values[index] = value;
  }

  popBack() {
    var newSize = this.size - 1;
    var sizedArr[newSize] = [];

    for (var i = 0; i < newSize; i = i + 1) {
      sizedArr[i] = this.values[i];
    }

    var poppedValue = this.values[this.size - 1];

    this.values = sizedArr;
    this.size = newSize;

    return poppedValue;
  }

  pushBack(pushValue) {
    var newSize = this.size + 1;
    var sizedArr[newSize] = [];

    for (var i = 0; i < this.size; i = i + 1) {
      sizedArr[i] = this.values[i];
    }

    sizedArr[newSize - 1] = pushValue;

    this.size = newSize;
    this.values = sizedArr;
  }

  forEach(callback) {
    for (var i = 0; i < this.size; i = i + 1) {
      callback(this.values[i]);
    }
  }
}

var names = Vector(0);

for (var i = 0; i < 10; i = i + 1) {
  names.pushBack(i + 1);
}

fun printSomething(thing) {
  print thing * thing;
}

print names.size;

names.forEach(printSomething);


