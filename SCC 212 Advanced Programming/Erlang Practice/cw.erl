-module(cw).
-export([find_patterns/2]).
find_patterns(Data,Patterns) ->
    lists:map(fun(X)-> ofset(Data, X,0,[]) end, Patterns).

ofset(Data, Pattern,In,Set)->
    Index = string:str(Data,Pattern),
    if Index /= 0 ->
        NewSet = Set++[Index-1+In],
        ofset(string:substr(Data,Index+1),Pattern,Index+In,NewSet);
    true->
        Set
    end.

