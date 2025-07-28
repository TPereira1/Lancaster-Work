-module(leap).

-export([is_leap_year/1]).

is_leap_year(Year) ->
    if 
        (Year rem 4 == 0) ->
            if(Year rem 100 == 0) and (Year rem 400 /= 0) ->
                false;
                true -> true
            end;
        true -> false
    end.
