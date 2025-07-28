-module(area).
-export([area/1]).
area({square,Side})->Side*Side;
area({circle,Side})->math:pi()*Side*Side.