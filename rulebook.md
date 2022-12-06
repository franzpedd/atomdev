--- LEXER --- 
Re-create lexer based on current lexer, it's been a while since I've made it
Preprocessor is missing
Post-analyze strings to better categorize them

--- PARSER MISSING ---
Create tree of variables by scopes

--- SEMANTHICS MISSING ---
Handle type mistmatch
Handle undeclared variables -> uses undoe by parser (or pause everything to )
Handle identifier misuse

--- PARSER ALTERNATIVE ---
Change verification method to graph-based

--- PARSER RULES ---
0-9
    - previous: 0-9 ( [ { ; , + - * / && || ! = > >= < <= == != %
    - next:     0-9 ) ] } ; , + - * / && || ! = > >= < <= == != % 

string
    - previous: string, i32, u32, i64, u64 f32, f64, char, import, define, ( [ { ; , = !=
    - next: string ) ] } ; , = !=

i32, u32, i64, u64, f32, f64, char
    - previous: []{}();
    - next:     a-z A-Z

while, for, if, elseif, else
    - previous: } { ;
    - next: (

case, default
    - previous: { } ;
    - next: :

switch
    - previous: } ;
    - next: (

break
    - previous: } ; {
    - next: ;

return
    - previous: string i32 u32 i64 u64 f32 f64 char ;
    - next: string ;

continue
    - previous: { } ;
    - next: ;

enum, struct
    - previous: ; { } 
    - next: string {

import, define
    - previous: string ;
    - next: string

function
    - previous: } ;
    - next: string i32 u32 i64 u64 f32 f64 char

&&, &&, !, =, +, -, *, /, %, >, >=, <, <=, ==, !=
    - previous: string, 0-9
    - next: string, 0-9
    
[
    - previous: string, []
    - next: 0-9, string, [ ]

]
    - previous: string, 0-9, [ ]
    - next: == =  [ ]

{, }
    - previous: ) ; } {}
    - next: string for while if elseif else u32 u64 i32 u64 f32 f64 char

(
    - previous: string ) ;
    - next: string, 0-9

)
    - previous: 0-9 string, ) } ;
    - next: ) ( { [ ;

;
    - previous: 0-9 string ] } )
    - next: string, i32, u32, i64, u64, f32, f64, char, for, while, default, case, switch, if, elseif, else, semicolon

:
    - previous: 0-9, case, default, string
    - next: {

,
    - previous: string, 0-9, ] ) }
    - next: string, 0-9

.
    - previous: string, 0-9
    - next: string, 0-9

'"
    - previous: string { , (
    - next: 