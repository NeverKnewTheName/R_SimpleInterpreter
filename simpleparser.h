/**
 * @brief The SimpleParser class
 *
 * # Grammar:
 *     * PrimaryExpression:
 *         * Value
 *         * Data
 *         * ( Expression )
 *     * PostFixExpression:
 *         * PrimaryExpression
 *     * UnaryExpression:
 *         * PostFixExpression
 *         * ++ UnaryExpression
 *         * -- UnaryExpression
 *         * UnaryOperator CastExpression
 *     * UnaryOperator:
 *         * +
 *         * -
 *         * ~
 *         * !
 *     * CastExpression:
 *         * UnaryExpression
 *         * (TypeName) CastExpression
 *     * TypeName:
 *         * Integer
 *         * Double
 *         * Bool
 *         * String
 *     * MultiplicativeExpression:
 *         * CastExpression
 *         * MultiplicativeExpression * CastExpression
 *         * MultiplicativeExpression / CastExpression
 *         * MultiplicativeExpression % CastExpression
 *     * AdditiveExpression:
 *         * MultiplicativeExpression
 *         * AdditiveExpression + MultiplicativeExpression
 *         * AdditiveExpression - MultiplicativeExpression
 *     * ShiftExpression:
 *         * AdditiveExpression
 *         * ShiftExpression << AdditiveExpression
 *         * ShiftExpression >> AdditiveExpression
 *     * RelationalExpression:
 *         * ShiftExpression
 *         * RelationalExpression < ShiftExpression
 *         * RelationalExpression > ShiftExpression
 *         * RelationalExpression <= ShiftExpression
 *         * RelationalExpression >= ShiftExpression
 *     * EqualityExpression:
 *         * RelationalExpression
 *         * EqualityExpression == RelationalExpression
 *         * EqualityExpression != RelationalExpression
 *     * BitwiseANDExpression:
 *         * EqualityExpression
 *         * BitwiseANDExpression & EqualityExpression
 *     * BitwiseXORExpression:
 *         * BitwiseANDExpression
 *         * BitwiseXORExpression ^ BitwiseANDExpression
 *     * BitwiseORExpression:
 *         * BitwiseXORExpression
 *         * BitwiseORExpression | BitwiseXORExpression
 *     * LogicalANDExpression:
 *         * BitwiseORExpression
 *         * LogicalANDExpression && BitwiseORExpression
 *     * LogicalXORExpression:
 *         * LogicalANDExpression
 *         * LogicalXORExpression ^^ LogicalANDExpression
 *     * LogicalORExpression:
 *         * LogicalXORExpression
 *         * LogicalORExpression || LogicalXORExpression
 *     * ConditionalExpression:
 *         * LogicalORExpression
 *         * LogicalORExpression ? Expression : ConditionalExpression
 *     * Expresion:
 *         * ConditionalExpresion
 *
 * ## Rules:
 *     Program                  :   Function
 *     Function                 :   Func Integer TypeName "("(TypeName Variable)*")" "{" (VarDeclaration)* (Expression)* ReturnStatement "}"
 *     VarDeclaration           :   TypeName Variable ";"
 *     ReturnStatement          :   "return" ( Expression | ";" )
 *     Expression               :   AssignmentExpression ";"
 *     AssignmentExpression     :   (Variable ( "=" | "+=" | "-=" | "*=" | "/=" | "%="  | ">>="  | "<<="  | "&="  | "|="  | "^=" ))? ConditionalExpression
 *     ConditionalExpression    :   LogicalORExpression         ( "?" Expression ":" AssignmentExpression )*
 *     LogicalORExpression      :   LogicalXORExpression        ( "||" LogicalXORExpression )*
 *     LogicalXORExpression     :   LogicalANDExpression        ( "^^" LogicalANDExpression )*
 *     LogicalANDExpression     :   BitwiseORExpression         ( "&&" BitwiseORExpression )*
 *     BitwiseORExpression      :   BitwiseXORExpression        ( "|" BitwiseXORExpression )*
 *     BitwiseXORExpression     :   BitwiseANDExpression        ( "^" BitwiseANDExpression )*
 *     BitwiseANDExpression     :   EqualityExpression          ( "&" EqualityExpression )*
 *     EqualityExpression       :   RelationalExpression        ( ( "==" | "!=" ) RelationalExpression )*
 *     RelationalExpression     :   ShiftExpression             ( ( "<" | ">" | "<=" | ">=" ) ShiftExpression )*
 *     ShiftExpression          :   AdditiveExpression          ( ( "<<" | ">>" ) AdditiveExpression )*
 *     AdditiveExpression       :   MultiplicativeExpression    ( ( "+" | "-" ) MultiplicativeExpression )*
 *     MultiplicativeExpression :   UnaryExpression             ( ( "*" | "/" | "%" ) UnaryExpression )*
 *     UnaryExpression          :   ( "++" | "--" | "+" | "-" | "~" | "!" | "("TypeName")" )* PostFixExpression
 *     TypeName                 :   "Integer" | "Double" | "Bool" | "String"
 *     PostFixExpression        :   Variable ( "++" | "--" )* | PrimaryExpression
 *     PrimaryExpression        :   Value | Data | Symbol | "(" Expression ")"
 *     Symbol                   :   Data | Variable | Function
 *     Data                     :   "D" Integer                        ## Parsed by the Interpreter
 *     Variable                 :   Value
 *     Value                    :   Integer | Double | Bool | String   ## Parsed by the Interpreter
 *     Integer                  :   (0-9)+
 *     Double                   :   (0-9)+.(0-9)+
 *     Bool                     :   true | false
 *     String                   :   "(a-z | 0-9)*"
 */
#ifndef SIMPLEPARSER_H
#define SIMPLEPARSER_H

#include "simplelexer.h"
#include "simplesymboltable.h"
#include "simpleast.h"

class Msg;

class SimpleParser
{
public:
    SimpleParser(SimpleLexer *lexer);
    SimpleNode *parse();

    SymbolTable &getSymblTbl();

private:
    void eat(SimpleToken::TokenType tokenType);
    SimpleNode *Program();
    SimpleNode *Function();
    SimpleNode *VarDeclaration(SymbolTable *SymbolTableToRegisterVariableTo);
    SimpleNode *ReturnStatement();
    SimpleNode *Expression();
    SimpleNode *AssignmentExpression();
    SimpleNode *ConditionalExpression();
    SimpleNode *LogicalORExpression();
    SimpleNode *LogicalXORExpression();
    SimpleNode *LogicalANDExpression();
    SimpleNode *BitwiseORExpression();
    SimpleNode *BitwiseXORExpression();
    SimpleNode *BitwiseANDExpression();
    SimpleNode *EqualityExpression();
    SimpleNode *RelationalExpression();
    SimpleNode *ShiftExpression();
    SimpleNode *AdditiveExpression();
    SimpleNode *MultiplicativeExpression();
    SimpleNode *UnaryExpression();
    SimpleNode *PostFixExpression();
    SimpleNode *PrimaryExpression();
    SimpleNode *Symbol();

    void SyntacticError(SharedSimpleTokenPtr Token, QString details = QString());
    void TypeError(SharedSimpleTokenPtr Token, QString details = QString());
    void EOFUnexpectedError(SharedSimpleTokenPtr Token, QString details = QString());
    void EOFExpectedError(SharedSimpleTokenPtr Token, QString details = QString());

    SimpleLexer *lexer;
    SharedSimpleTokenPtr CurrentToken;
    SymbolTable SymblTbl;
    SymbolTable * CurSymblTbl;
    bool ErrorOccured;
};

#endif // SIMPLEPARSER_H
