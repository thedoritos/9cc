#!/bin/bash
try() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  gcc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

try 0 0
try 42 42
try 21 "53+76-108"
try 99 " 123 - 8 +   32 - 48 "
try 63 "3*21"
try 76 "98 - (76 - 54)"
try 84 "12 * (3 + 4)"
try 14 "7*(8/(16-4*3))"
try 12 "((8+4)-(3+5))*3"
try 15 "-3*-5"
try 30 "+3*+10"
try 24 "-16+40"

try 1 "12==12"
try 0 "12==13"
try 1 "0!=1"
try 0 "23!=23"
try 1 "99<100"
try 0 "87<87"
try 1 "87<=87"
try 0 "6<=5"
try 1 "33>32"
try 0 "33>33"
try 1 "33>=33"
try 0 "33>=34"

try 1 "a = 1;"
try 3 "a = 1; a + 2;"
try 10 "a = 1; b = 2 * 3; a * 4 + b;"

echo OK
