-module(hello).
 -export([helloWorld/0]).
 helloWorld()->io:fwrite("hello world\n").