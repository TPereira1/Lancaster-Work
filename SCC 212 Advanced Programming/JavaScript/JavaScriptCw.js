"use strict";
//Used to hold the pattern being searched for
let patterns =[];
//Used to hold the sequence
let dna = '';
//table object for the frequency
let table = {};
//offset table for the offfset of each pattern
let offsetTable ={};
//offset value set to zero
let offset = 0;
//Object stores all possible combinations for each pattern
let possiblePattern = {};

//Nucleotides object used to map each nucleodide to an array of what it represents
const Nucleotides ={
    A: ['A'],
    T: ['T'],
    C: ['C'],
    G: ['G'],
    R: ['A','G','R'],
    Y: ['T','C','Y'],
    K: ['G','T','K'],
    M: ['A','C','M'],
    S: ['G','C','S'],
    W: ['A','T','W'],
    B: ['G','T','C','B'],
    D: ['G','A','T','D'],
    H: ['A','C','T','H'],
    V: ['A','G','C','V'],
    N: ['A','G','C','T','N'],
};

//Takes a pattern then creates a list of all possible combinations that could match the pattern
function getPossiblePatterns(...pattern){
    let CompareList = [];
    pattern.forEach(function(c){
        const temp = CompareList.map((x)=>x);
        CompareList = [];
        if(c!=" " && c!=undefined && c!=null){
            Nucleotides[c].forEach(function(nuc){
                if(temp.length == 0){
                    CompareList.push(nuc);
                }else{
                    CompareList= CompareList.concat(temp.map((element)=>element+nuc));
                }
            });
        }
    });
    return CompareList;
}

//Sets all table values to zero
function resetTable(){
    patterns.forEach(function (pattern) {
        table[pattern]=0;
        offsetTable[pattern] =[];
    });
}

//Check if The DNA strand matches a pattern
function compare(...pattern){
    let found = false;
    let a = possiblePattern[pattern.join("")].find((strand)=>strand==dna.slice((-pattern.length)));
    if(a != undefined){
        found = true;
    }
    return found;
}

//outputs the frequency table and the offset tables
function outputData(){
    console.log("\nFrequency Table:");
    testlib.frequencyTable(table);
    console.log("\nOffest Table:");
    testlib.frequencyTable(offsetTable);
}

const testlib = require( './testlib.js' );

//Stores patterns in memory and start the sequence read
testlib.on( 'ready', function(pat) {
	console.log( "Patterns:", pat );
    patterns = pat;
    patterns.forEach(function(pattern){
        possiblePattern[pattern]= getPossiblePatterns(...pattern);
    });
    resetTable();
	testlib.runTests();
} );

//Read nucleoide sequence one nucleoide at a time save it to dna variable and preforms a check to see if last part of dna matches a pattern
testlib.on( 'data', function( data ) {
    dna = dna+data;
    offset++;
    patterns.forEach(function (pattern) {
        if(compare(...pattern)){
            table[pattern]++;
            offsetTable[pattern].push((offset-pattern.length));
            testlib.foundMatch(pattern,offset-pattern.length);
        }
    });
} );

//outputs frequency and offset table, then resets dna and offset
testlib.on('reset', function(){
    outputData();
    console.log("\nResetting table")
    dna = '';
    resetTable();
    offset = 0;
});

//outputs the last frequency and offset table 
testlib.on('end', function(){
    outputData();
});

testlib.setup(3,0);