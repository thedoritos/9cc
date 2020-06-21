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

try 0 "0;"
try 42 "42;"
try 21 "53+76-108;"
try 99 " 123 - 8 +   32 - 48;"
try 63 "3*21;"
try 76 "98 - (76 - 54);"
try 84 "12 * (3 + 4);"
try 14 "7*(8/(16-4*3));"
try 12 "((8+4)-(3+5))*3;"
try 15 "-3*-5;"
try 30 "+3*+10;"
try 24 "-16+40;"

try 1 "12==12;"
try 0 "12==13;"
try 1 "0!=1;"
try 0 "23!=23;"
try 1 "99<100;"
try 0 "87<87;"
try 1 "87<=87;"
try 0 "6<=5;"
try 1 "33>32;"
try 0 "33>33;"
try 1 "33>=33;"
try 0 "33>=34;"

try 1 "a = 1;"
try 2 "z = 2; z;"
try 3 "a = 1; a + 2;"
try 10 "a = 1; b = 2 * 3; a * 4 + b;"

try 1 "abc = 1;"
try 2 "xyz = 2; xyz;"
try 10 "abc = 1; efg = 2 * 3; abc + efg + 3;"
try 2 "ant = 1; apple = 2; ant * apple;"

try 0 "return 0;"
try 1 "a = 1; return a; 99;"
try 2 "b = 1; return b * 2; return 99;"
try 3 "return0 = 3; return0;"

try 2 "a = 1; if (a > 0) a = a + 1; a;"
try 1 "b = 1; if (b < 0) b = b + 1; b;"
try 3 "c = 1; if (c > 0) c = c * 3; else c = c + 1; c;"
try 4 "d = 8; if (d < 0) d = d / 1; else d = d / 2; d;"

try 5 "e = 0; while (e < 5) e = e + 1; e;"
try 0 "e = 0; while (e > 5) e = e + 1; e;"

try 3 "a = 0; for (i = 0; i < 3; i = i + 1) a = a + i; a;"
try 9 "b = 0; for (; b < 9;) b = b + 1; b;"

echo OK
