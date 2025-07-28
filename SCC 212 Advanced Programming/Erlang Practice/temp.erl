-module(temp).

-export([convert/1]).

convert({c,T}) -> 
    {f,(T*9/5)+32};
convert({f,T}) ->
    {c,(T-32)*5/9}.