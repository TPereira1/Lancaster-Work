-module(lists1).
-export([min/1]).

%size([])-> 0;

%size(List) -> 
%    size(List, 0).

%size([_|Tail], Count) ->
%    size(Tail, Count + 1);

%size([], Count) ->
%    Count.

%max([H|T]) ->
%    max(T,H).

%max([],Max) ->
%    Max;

%max([H|T],Max) when Max < H->
%    max(T,H);

%max([_|T],Max) ->
%    max(T,Max).
min(List)->
MinValue = lists:min(List),
io:format("Minimum value: ~p~n", [MinValue]).

