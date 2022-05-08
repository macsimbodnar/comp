package main

import (
	"bufio"
	"io"
	"unicode"
)

func check(e error) {
	if e != nil {
		panic(e)
	}
}

// Token ...
type Token int

const (
	// EOF is the end of file
	EOF = iota
	// ILLEGAL is an invalid token
	ILLEGAL

	// IDENT ...
	IDENT // [a-zA-Z]\w*
	// LITINT ...
	LITINT // [0-9]+

	// INT ...
	INT // int
	// SEMCOL ...
	SEMCOL // ;
	// RETURN ...
	RETURN // return

	// OBRA ...
	OBRA // {
	// CBRA ...
	CBRA // }
	// OPAR ...
	OPAR // (
	// CPAR ...
	CPAR // )
)

var tokens = []string{
	EOF:     "EOF",
	ILLEGAL: "ILLEGAL",
	IDENT:   "IDENT",
	LITINT:  "INTLIT",
	INT:     "int",
	SEMCOL:  ";",
	RETURN:  "return",
	OBRA:    "{",
	CBRA:    "}",
	OPAR:    "(",
	CPAR:    ")",
}

func (t Token) String() string {
	return tokens[t]
}

// Position is the struct that hold the coordinate of the token int the file
type Position struct {
	line   int
	column int
}

// Lexer is the struct that keep track of the current state
type Lexer struct {
	pos    Position
	reader *bufio.Reader
}

func newLexer(reader io.Reader) *Lexer {
	return &Lexer{
		pos:    Position{line: 1, column: 0},
		reader: bufio.NewReader(reader),
	}
}

func (l *Lexer) newLine() {
	l.pos.line++
	l.pos.column = 0
}

func (l *Lexer) backup() {
	if err := l.reader.UnreadRune(); err != nil {
		panic(err)
	}

	l.pos.column--
}

// lexInt scans the input until the end of an integer and then returns the
// literal.
func (l *Lexer) lexInt() string {
	var lit string
	for {
		r, _, err := l.reader.ReadRune()
		if err != nil {
			if err == io.EOF {
				// at the end of the int
				return lit
			}
		}

		l.pos.column++
		if unicode.IsDigit(r) {
			lit = lit + string(r)
		} else {
			// scanned something not in the integer
			l.backup()
			return lit
		}
	}
}

// lexIdent scans the input until the end of an identifier and then returns the
// literal.
func (l *Lexer) lexIdent() string {
	var lit string
	for {
		r, _, err := l.reader.ReadRune()
		if err != nil {
			if err == io.EOF {
				// at the end of the identifier
				return lit
			}
		}

		l.pos.column++
		if unicode.IsLetter(r) {
			lit = lit + string(r)
		} else {
			// scanned something not in the identifier
			l.backup()
			return lit
		}
	}
}

// Lex scans the input for the next token. It returns the position of the token,
// the token's type, and the literal value.
func (l *Lexer) Lex() (Position, Token, string) {
	// keep looping until we return a token
	for {
		r, _, err := l.reader.ReadRune()

		// Handle the error
		if err != nil {
			if err == io.EOF {
				return l.pos, EOF, ""
			}

			// at this point there isn't much we can do, and the compiler
			// should just return the raw error to the user
			panic(err)
		}

		// update the column to the position of the newly read in rune
		l.pos.column++

		switch r {
		case '\n':
			l.newLine()
		case ';':
			return l.pos, SEMCOL, "" // ;
		case '{':
			return l.pos, OBRA, "" // {
		case '}':
			return l.pos, CBRA, "" // }
		case '(':
			return l.pos, OPAR, "" // (
		case ')':
			return l.pos, CPAR, "" // )

		default:
			if unicode.IsSpace(r) {
				continue // nothing to do here, just move on
			} else if unicode.IsDigit(r) { // [0-9]+
				// backup and let lexInt rescan the beginning of the int
				startPos := l.pos
				l.backup()
				lit := l.lexInt()
				return startPos, INT, lit
			} else if unicode.IsLetter(r) {
				// backup and let lexIdent rescan the beginning of the ident
				startPos := l.pos
				l.backup()
				lit := l.lexIdent()

				// Check for keywords
				switch lit {
				case "int":
					return startPos, INT, "" // int
				case "return":
					return startPos, RETURN, "" // return

				default:
					return startPos, IDENT, lit // [a-zA-Z]\w*
				}

			} else {
				return l.pos, ILLEGAL, string(r)
			}
		}
	}
}
